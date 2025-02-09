#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "utils.h"
#include "archiver.h"
#include "h_frequency_table.h"
#include "h_tree.h"
#include "huffman.h"

static char **codes;

static size_t write_freq_table(uint32_t **buf, const struct h_pq *table, size_t t_size);
static void write_header(struct huff_hdr *hdr, uint8_t flags, uint8_t ft_size);
static void write_file_header(struct f_hdr  *f_hdr,
                              int           fd,
                              const char    *f_name,
                              uint16_t      blk_cnt,
                              uint16_t      lst_blk_len,
                              uint8_t       flags);
static void encode_file(struct achv_file  *buf,
                        FILE              *file,
                        const char        *f_name,
                        uint8_t           flags);

void *create_archive(char **f_pths, const size_t f_cnt, const uint8_t flags)
{
    size_t            tbl_size;
    struct h_pq       *table;
    struct h_tree     *tree;
    struct huff_achv  *achv;

    FILE **f_lst;
    f_lst = xmalloc("create_archive", f_cnt * sizeof(*f_lst));

    for(int i = 0; i < f_cnt; i++) {
        FILE *f = fopen(f_pths[i], "r");
        if(f == NULL) {
            fprintf(stderr, "create_archive: Failed to open file '%s'", f_pths[i]);
            return NULL;
        }
        f_lst[i] = f;
    }

    tbl_size = build_freq_table(&table, f_lst, f_cnt);
    tree     = build_tree(table, tbl_size);
    h_codes_gen(&codes, tree);

    free_tree(tree);

    struct achv_file *arch_files;
    arch_files = xcalloc("create_archive", f_cnt, sizeof(*arch_files));

    for(int i = 0; i < f_cnt; i++) {
        encode_file(&arch_files[i], f_lst[i], f_pths[i], flags);
    }

    achv = xcalloc("create_archive", 1, sizeof(*achv));

    const uint8_t ft_size = write_freq_table(&achv->freq_table, table, tbl_size);
    write_header(&achv->hdr, flags, ft_size);
    achv->files = arch_files;

    for(int i = 0; i < f_cnt; i++) {
        fclose(f_lst[i]);
    }

    free(table);
    free_codes(codes);
    return achv;
}

size_t write_freq_table(uint32_t **buf, const struct h_pq *table, const size_t t_size)
{
    size_t ft_size = 0;

    for(int i = 0; i < t_size; i++)
        if(table[i].priority > 0)
            ft_size += 1;

    struct ft_record *ft_table;
    ft_table = xcalloc("write_freq_table", ft_size, sizeof(*ft_table));

    uint32_t offset = 0;
    for(int i = 0; i < t_size; i++) {
        if(table[i].priority > 0) {
            ft_table[i-offset].character = (char)i;
            ft_table[i-offset].frequency = table[i].priority;
        }else offset++;
    }

    *buf = (uint32_t *)ft_table;
    return ft_size;
}

void write_header(struct huff_hdr *hdr, const uint8_t flags, const uint8_t ft_size)
{
    memset(hdr, 0, sizeof(*hdr));
    // HFS
    hdr->magic[0]    = 0x48;
    hdr->magic[1]    = 0x1c;
    hdr->version[0]  = 0;
    hdr->version[1]  = 1;
    hdr->flags       = flags;
    hdr->ft_len      = ft_size;

    if(flags & F_CRC_HEADER_EXISTS) {
        const uint16_t crc = checksum(hdr, sizeof(*hdr));
        hdr->checksum[1]   = (uint8_t)(crc      & 255);
        hdr->checksum[0]   = (uint8_t)(crc >> 8 & 255);
    }
}

static char *filename(char *f_name, const size_t len)
{
    size_t offst = 0;
    for(size_t i = 0; i < len; i++) {
        if(f_name[i] == '/')
            offst = i+1;
    }
    return &f_name[offst];
}

void write_file_header(struct f_hdr     *f_hdr,
                       const  int       fd,
                       const  char      *f_name,
                       const  uint16_t  blk_cnt,
                       const  uint16_t  lst_blk_len,
                       const  uint8_t   flags)
{
    memset(f_hdr, 0, sizeof(*f_hdr));

    if(flags & F_FILE_ATTRIBUTES_STORED) {
        struct stat f_stat;
        fstat(fd, &f_stat);

        const char *name = filename((char *)f_name, strlen(f_name));

        memcpy(&f_hdr->f_name,     name,            strlen(name));
        memcpy(&f_hdr->f_mode,     &f_stat.st_mode, sizeof(f_hdr->f_mode));
        memcpy(&f_hdr->f_own_id,   &f_stat.st_uid,  sizeof(f_hdr->f_own_id));
        memcpy(&f_hdr->f_grp_id,   &f_stat.st_gid,  sizeof(f_hdr->f_grp_id));
        memcpy(&f_hdr->f_mtime,    &f_stat.st_mtim, sizeof(f_hdr->f_mtime));
        memcpy(&f_hdr->f_size,     &f_stat.st_size, sizeof(f_hdr->f_size));
        memcpy(&f_hdr->f_tail_len, &lst_blk_len,    sizeof(f_hdr->f_tail_len));
    }

    f_hdr->f_blk_cnt[1] = (uint8_t)(blk_cnt      & 255);
    f_hdr->f_blk_cnt[0] = (uint8_t)(blk_cnt >> 8 & 255);

    if(flags & F_CRC_FILES_EXISTS) {
        const uint16_t crc = checksum(f_hdr, sizeof(*f_hdr));
        f_hdr->checksum[1] = (uint8_t)(crc      & 255);
        f_hdr->checksum[0] = (uint8_t)(crc >> 8 & 255);
    }
}

void encode_file(struct achv_file  *buf,
                 FILE              *file,
                 const  char       *f_name,
                 const  uint8_t    flags)
{
    uint16_t      blk_cnt;
    struct f_blk  *f_blks_start;
    struct f_blk  *f_blks;
    uint16_t      f_next_bit;

    f_blks = xmalloc("encode_file", sizeof(*f_blks));

    f_blks_start  = f_blks;
    blk_cnt       = 1;
    f_next_bit    = 0;

    char ch;
    uint8_t sum = 0;
    uint8_t bit = 1;

    while((ch = fgetc(file)) != EOF) {
        const char *code = codes[(int)ch];
        const int  c_len = strlen(code);

        for(int i = 0; i < c_len; i++) {
            if(code[i] == '1')
                sum |= bit;
            if(bit < 128) {
                bit <<= 1;
            }else if(f_next_bit < BLK_SIZE) {
                f_blks->data[f_next_bit++] = sum;
                sum = 0;
                bit = 1;
            }else {
                f_blks->next = xmalloc("encode_file", sizeof(*f_blks));

                f_next_bit  = 0;
                f_blks      = f_blks->next;
                blk_cnt     = blk_cnt + 1;

                f_blks->data[f_next_bit] = sum;
                sum = 0;
                bit = 1;
            }
        }
    }

    if(bit > 1) {
        f_blks->data[f_next_bit++] = sum;
    }

    rewind(file);

    buf->f_blks = f_blks_start;
    write_file_header(&buf->f_hdr, fileno(file), f_name, blk_cnt, f_next_bit, flags);
}

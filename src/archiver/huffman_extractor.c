#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "utils.h"
#include "archiver.h"
#include "h_stack.h"
#include "h_tree.h"
#include "huffman.h"

static struct h_tree *tree;

static size_t read_freq_table(struct h_pq **buf, const uint32_t *bytes, size_t len);
static size_t decode_file(uint8_t **buf, const struct achv_file *achv_f);
static FILE *extract_file(struct achv_file *achv_f, uint8_t flags);

void *extract_archive(const struct huff_achv *achv, const size_t f_cnt)
{
    uint16_t          tbl_size;
    struct h_pq       *table;
    FILE              **f_lst;

    memcpy(&tbl_size, &achv->hdr.ft_len, sizeof(tbl_size));

    tbl_size = read_freq_table(&table, achv->freq_table, tbl_size);
    tree     = build_tree(table, tbl_size);

    f_lst = xmalloc("extract_archive", f_cnt * sizeof(*f_lst));

    for(int i = 0; i < f_cnt; i++) {
        f_lst[i] = extract_file(&achv->files[i], achv->hdr.flags);
    }

    return f_lst;
}

size_t read_freq_table(struct h_pq **buf, const uint32_t *bytes, const size_t len)
{
    struct h_pq  *table;

    table = xcalloc("read_freq_table", TABLE_SIZE, sizeof(*table));

    for(int i = 0; i < len; i++) {
        struct ft_record record;

        memcpy(&record, &bytes[i], sizeof(record));
        const int ch = record.character;

        table[ch].p_node = xcalloc("read_freq_table", 1, sizeof(struct h_tree));
        table[ch].p_node->character = (char)ch;
        table[ch].priority = record.frequency;
    }

    *buf = table;
    return TABLE_SIZE;
}

size_t decode_file(uint8_t **buf, const struct achv_file *achv_f)
{
    uint64_t      n_bytes;
    uint8_t       *bytes;
    uint16_t      blk_cnt;
    uint16_t      tail_len;
    struct f_blk  *f_blk;

    blk_cnt  = achv_f->f_hdr.f_blk_cnt[1] << 8;
    blk_cnt += achv_f->f_hdr.f_blk_cnt[0];

    tail_len  = achv_f->f_hdr.f_tail_len[1] << 8;
    tail_len += achv_f->f_hdr.f_tail_len[0];

    memcpy(&n_bytes, achv_f->f_hdr.f_size, sizeof(n_bytes));
    bytes = xmalloc("decode_file", n_bytes * sizeof(*bytes));

    size_t offst = 0;
    f_blk = achv_f->f_blks;
    const struct h_tree *dec_tree = tree;
    for(int i = 0; i < blk_cnt-1; i++) {

        for(int j = 0; j < BLK_SIZE; j++) {
            const uint8_t byte = f_blk->data[j];

            for(int bit = 1; bit <= 128; bit <<= 1) {
                if((byte & bit) == 0) {
                    dec_tree = dec_tree->bit0;
                }else {
                    dec_tree = dec_tree->bit1;
                }

                if(dec_tree->bit0 == NULL) {
                    bytes[offst++] = dec_tree->character;
                    dec_tree = tree;
                }
            }
        }

        f_blk = f_blk->next;
    }

    for(int j = 0; j < tail_len; j++) {
        const uint8_t byte = f_blk->data[j];

        for(int bit = 1; bit <= 128; bit <<= 1) {
            if((byte & bit) == 0) {
                dec_tree = dec_tree->bit0;
            }else {
                dec_tree = dec_tree->bit1;
            }

            if(dec_tree->bit0 == NULL) {
                bytes[offst++] = dec_tree->character;
                dec_tree = tree;
            }
        }
    }

    *buf = bytes;

    return n_bytes;
}

FILE *extract_file(struct achv_file *achv_f, const uint8_t flags)
{
    struct f_hdr  *hdr;
    char          f_name[4096] = "/home/yukir/";
    uint8_t       *f_bytes;

    hdr    = &achv_f->f_hdr;

    strcat(f_name, achv_f->f_hdr.f_name);

    uint16_t hdr_crc;
    hdr_crc  = hdr->checksum[1] << 8;
    hdr_crc += hdr->checksum[0];

    memset(&hdr->checksum, 0, 2);

    if(checksum(hdr, sizeof(*hdr)) != hdr_crc) {
        fprintf(stderr, "extract_file: Invalid checksum, file '%s' will be skipped", f_name);
        return NULL;
    }

    const size_t n_bytes = decode_file(&f_bytes, achv_f);
    FILE *f = fopen(f_name, "w");

    for(int i = 0; i < n_bytes; i++) {
        fwrite(&f_bytes[i], sizeof(*f_bytes), 1, f);
    }

    if(flags & F_FILE_ATTRIBUTES_STORED) {
        const int fd = fileno(f);

        uid_t own_id;
        own_id  = hdr->f_own_id[3] << 24;
        own_id += hdr->f_own_id[2] << 16;
        own_id += hdr->f_own_id[1] << 8;
        own_id += hdr->f_own_id[0];

        gid_t grp_id;
        grp_id  = hdr->f_grp_id[3] << 24;
        grp_id += hdr->f_grp_id[2] << 16;
        grp_id += hdr->f_grp_id[1] << 8;
        grp_id += hdr->f_grp_id[0];

        fchown(fd, own_id, grp_id);

        mode_t mode;
        mode  = hdr->f_mode[3] << 24;
        mode += hdr->f_mode[2] << 16;
        mode += hdr->f_mode[1] << 8;
        mode += hdr->f_mode[0];

        fchmod(fd, mode);

        struct timespec ts;
        memcpy(&ts, hdr->f_mtime, sizeof(ts));

        struct timeval tv;
        TIMESPEC_TO_TIMEVAL(&tv, &ts);

        const struct timeval tvs[2] = { tv, tv };
        futimes(fd, tvs);
    }

    return f;
}

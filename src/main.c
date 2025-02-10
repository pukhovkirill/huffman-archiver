#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "utils.h"
#include "h_stack.h"
#include "archiver.h"

static int f_cnt;
static uint8_t flags;

void write_achv(const char *dst, const struct huff_achv *src);
size_t read_achv(struct huff_achv **dst, FILE *src);

int main(void)
{
    f_cnt = 1;
    char **files;

    char file1[] = "/home/yukir/CLionProjects/huffman-archiver/tests/resources/test.txt";
    files = calloc(f_cnt, sizeof(*files));
    files[0] = file1;

    flags |= F_CRC_HEADER_EXISTS;
    flags |= F_CRC_FILES_EXISTS;
    flags |= F_FILE_ATTRIBUTES_STORED;

    const struct huff_achv *achv = create_archive(files, f_cnt, flags);

    write_achv("/home/yukir/test1.hff", achv);

    FILE *f;

    f = fopen("/home/yukir/test1.hff", "r");

    FILE **f_lst;

    struct huff_achv *eachv;
    const size_t f_cnt = read_achv(&eachv, f);

    f_lst = extract_archive(eachv, f_cnt);

    fflush(f_lst[0]);
    fclose(f_lst[0]);

    return 0;
}

void write_achv(const char *dst, const struct huff_achv *src)
{
    FILE  *f;
    int   fd;

    f = fopen(dst, "w");
    if(f == NULL) {
        perror("write_archive: Failed to create file");
        exit(EXIT_FAILURE);
    }

    fd = fileno(f);
    fchmod(fd, 0644);

    // write archive header
    fwrite(&src->hdr, sizeof(src->hdr), 1, f);

    uint16_t ft_size;
    ft_size  = src->hdr.ft_len[1] << 8;
    ft_size += src->hdr.ft_len[0];

    // write frequency table
    for(int i = 0; i < ft_size; i++) {
        fwrite(&src->freq_table[i], sizeof(*src->freq_table), 1, f);
    }

    // write 'f_cnt' files
    const struct achv_file *files = src->files;
    for(int i = 0; i < f_cnt; i++) {
        const struct achv_file *file = &files[i];

        // write 'i' file header
        fwrite(&file->f_hdr, sizeof(files[i].f_hdr), 1, f);

        uint32_t blk_cnt;
        memcpy(&blk_cnt, file->f_hdr.f_blk_cnt, sizeof(blk_cnt));

        uint16_t tail_len;
        memcpy(&tail_len, file->f_hdr.f_tail_len, sizeof(tail_len));

        const struct f_blk *blk = file->f_blks;

        if(tail_len > 0)
            blk_cnt -= 1;

        // write 'blk_cnt-1' blocks
        for(int j = 0; j < blk_cnt; j++) {
            fwrite(blk->data, 1, BLK_SIZE, f);
            blk = blk->next;
        }

        // write last block
        fwrite(blk->data, 1, tail_len, f);
    }

    fputc(EOF, f);
    fclose(f);
}

static void read_achv_hdr(uint8_t *hdr, FILE *f_achv)
{
    fread(hdr, 1, sizeof(struct huff_hdr), f_achv);

    if(hdr[0] != 0x48 || hdr[1] != 0x1c) {
        fprintf(stderr, "read_achv_hdr: Invalid file type");
        exit(EXIT_FAILURE);
    }

    if(hdr[3] & F_CRC_HEADER_EXISTS) {
        uint16_t hdr_crc;
        hdr_crc  = hdr[7] << 8;
        hdr_crc += hdr[6];

        memset(hdr+6, 0, 2);
        if(checksum(hdr, sizeof(hdr)) != hdr_crc) {
            fprintf(stderr, "read_achv_hdr: Invalid checksum");
            exit(EXIT_FAILURE);
        }
    }
}

size_t read_achv(struct huff_achv **dst, FILE *src)
{
    size_t            ft_size;
    size_t            f_cnt;
    struct huff_achv  *achv;

    achv = xcalloc("read_achv", 1, sizeof(*achv));
    read_achv_hdr((uint8_t *)&achv->hdr, src);

    ft_size = 0;
    memcpy(&ft_size, &achv->hdr.ft_len, sizeof(uint16_t));

    struct ft_record *ft_table;
    ft_table = xcalloc("write_freq_table", ft_size, sizeof(*ft_table));

    for(int i = 0; i < ft_size; i++) {
        fread(&ft_table[i], sizeof(*ft_table), 1, src);
    }

    h_stack *stack = xmalloc("read_achv", sizeof(*stack));

    const size_t f_hdr_size = sizeof(struct f_hdr);

    uint8_t h_size = 0;
    uint8_t hdr[f_hdr_size];

    int ch;
    while((ch = fgetc(src)) != EOF) {
        hdr[h_size++] = ch;

        if(h_size == f_hdr_size) {
            h_size = 0;
            uint32_t blk_cnt;
            memcpy(&blk_cnt, &hdr[136], sizeof(blk_cnt));

            uint16_t tail_len;
            memcpy(&tail_len, &hdr[140], sizeof(tail_len));

            struct f_blk *blk_start;
            struct f_blk *blk;
            blk = xcalloc("read_achv", 1, sizeof(*blk));

            blk_start = blk;

            if(tail_len > 0)
                blk_cnt -= 1;

            for(int i = 0; i < blk_cnt; i++) {
                fread(&blk->data, 1, BLK_SIZE, src);
                blk->next = xcalloc("read_achv", 1, sizeof(*blk));
                blk = blk->next;
            }

            fread(&blk->data, 1, tail_len, src);

            struct achv_file *achv_f;
            achv_f = xcalloc("read_achv", 1, sizeof(*achv_f));

            achv_f->f_blks = blk_start;
            memcpy(&achv_f->f_hdr, hdr, sizeof(achv_f->f_hdr));

            push_sck(stack, achv_f);
        }
    }

    rewind(src);

    f_cnt = stack->size;

    achv->freq_table = (uint32_t *)ft_table;
    achv->files = xmalloc("read_achv", f_cnt * sizeof(*achv->files));

    while(stack->size > 0) {
        struct achv_file *file;
        pop_sck(stack, (void **)&file);
        memcpy(&achv->files[stack->size], file, sizeof(*file));
    }

    *dst = achv;

    free(stack);
    return f_cnt;
}

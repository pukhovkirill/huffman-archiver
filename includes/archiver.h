#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <stdio.h>
#include <stdint.h>

#define BLK_SIZE 512

struct achv_file {

    struct f_hdr {
        uint8_t f_name[100];
        uint8_t f_mode[4];
        uint8_t f_own_id[4];
        uint8_t f_grp_id[4];
        uint8_t f_mtime[16];
        uint8_t f_size[8];
        uint8_t f_blk_cnt[4];
        uint8_t f_tail_len[2];
        uint8_t checksum[2];
    };

    struct f_blk {
        uint8_t data[BLK_SIZE];
        struct f_blk *next;
    };

    struct f_blk *f_blks;
    struct f_hdr f_hdr;
};

struct huff_achv {

    struct huff_hdr {
        uint8_t  magic[2];
        uint8_t  version;
        uint8_t  flags;
        uint8_t  ft_len[2];
        uint8_t  checksum[2];
    };

    struct huff_hdr   hdr;
    uint32_t          *freq_table;
    struct achv_file  *files;
};

struct ft_record{
    uint8_t   character  : 8;
    uint32_t  frequency  : 24;
};

#define F_FILE_ATTRIBUTES_STORED  32
#define F_CRC_FILES_EXISTS        64
#define F_CRC_HEADER_EXISTS       128

#endif //ARCHIVER_H

uint16_t checksum(const void *b, size_t len);
void *extract_archive(const struct huff_achv *achv, size_t f_cnt);
void *create_archive(char **f_pths, size_t f_cnt, uint8_t flags);
void *append_to_archive(FILE *f_achv, FILE **fls, size_t f_cnt);

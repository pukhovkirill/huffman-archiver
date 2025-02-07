#ifndef ARCHIVER_H
#define ARCHIVER_H
#define _GNU_SOURCE

#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

struct achv_file {

    struct f_hdr {
        uint8_t f_name[100];
        uint8_t f_mode[4];
        uint8_t f_own_id[4];
        uint8_t f_grp_id[4];
        uint8_t f_mtime[16];
        uint8_t f_size[8];
        uint8_t f_blk_cnt[2];
        uint8_t checksum[2];
    };

    struct f_blk {
        uint8_t data[512];
        struct f_blk *next;
    };

    struct f_blk *f_blks;
    struct f_hdr f_hdr;
};

struct huff_achv {

    struct huff_hdr {
        uint8_t  magic[2];
        uint8_t  version[2];
        uint8_t  flags;
        uint8_t  ft_len;
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

void *extract_archive(FILE *achv, uint8_t flags);
void *create_archive(char **f_pths, size_t f_cnt, uint8_t flags);
void *append_to_archive(FILE *achv, uint8_t flags, FILE **fls, size_t f_cnt);

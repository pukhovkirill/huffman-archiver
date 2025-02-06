#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <stdio.h>
#include <inttypes.h>

struct arc_file {

    struct f_attr {
        uint8_t f_name[100];
        uint8_t f_mode[8];
        uint8_t f_own_id[8];
        uint8_t f_grp_id[8];
        uint8_t f_m_time[12];
        uint8_t f_size[16];
        uint8_t f_blk_cnt[2];
        uint8_t checksum[2];
    };

    struct f_blk {
        uint8_t data[512];
    };

    struct f_blk *f_blks;
    struct f_attr f_attr;
};

struct huff_arc {

    struct huff_hdr {
        uint8_t magic[2];
        uint8_t version;
        uint8_t flags;
        uint8_t ct_len[2];
        uint8_t checksum[2];
    };

    struct huff_hdr hdr;
    char code_table[256][256];
    struct arc_file *files;
};

#endif //ARCHIVER_H

void *create_archive(const FILE *fls, size_t f_cnt);
void *extract_archive(const FILE *achv);
void *append_to_archive(const FILE *achv, const FILE *fls, size_t f_cnt);

#include "archiver.h"

#ifndef HUFFMAN_ARCHIVER_C
#define HUFFMAN_ARCHIVER_C

#include "h_frequency_table.h"
#include "h_priority_queue.h"
#include "h_tree.h"

#define BLK_SIZE 512

#endif //HUFFMAN_ARCHIVER_C

static uint16_t checksum(const void *b, size_t len);

static size_t write_freq_table(uint32_t **buf, const struct h_pq *table, size_t t_size);
static void create_header(struct huff_hdr *hdr, uint8_t flags, uint8_t ft_size);

static void create_file_header(const char *f_name, struct f_hdr *f_hdr, FILE *file, uint16_t blk_cnt, uint8_t flags);
void encode_file(struct achv_file *buf, FILE *file, const char *f_name, uint8_t flags, char **codes);

void *create_archive(char **f_pths, const size_t f_cnt, const uint8_t flags)
{
    size_t t_size;
    struct h_pq *table;
    struct h_tree *tree;
    char **codes;

    struct huff_achv *achv;

    FILE **fls;
    fls = malloc(f_cnt * sizeof(*fls));

    for(int i = 0; i < f_cnt; i++) {
        FILE *f = fopen(f_pths[i], "r");
        if(f == NULL) {
            fprintf(stderr, "create_archive: Failed to open file '%s'", f_pths[i]);
            return NULL;
        }
        fls[i] = f;
    }

    t_size = build_freq_table(&table, fls, f_cnt);
    tree = build_tree(table, t_size);
    h_codes_gen(&codes, tree);

    free_tree(tree);

    struct achv_file *arch_files;
    arch_files = xcalloc("create_archive", f_cnt, sizeof(*arch_files));

    for(int i = 0; i < f_cnt; i++) {
        encode_file(&arch_files[i], fls[i], f_pths[i], flags, codes);
    }

    achv = xcalloc("create_archive", 1, sizeof(*achv));

    const uint8_t ft_size = write_freq_table(&achv->freq_table, table, t_size);
    create_header(&achv->hdr, flags, ft_size);
    achv->files = arch_files;

    for(int i = 0; i < f_cnt; i++) {
        fclose(fls[i]);
    }

    free(table);
    free_codes(codes);
    return achv;
}

uint16_t checksum(const void *b, size_t len)
{
    register uint32_t sum;
    const uint16_t *buf = b;

    for(sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if(len == 1)
        sum += *buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += sum >> 16;
    return ~sum;
}

size_t write_freq_table(uint32_t **buf, const struct h_pq *table, const size_t t_size)
{
    size_t ft_size = 0;

    for(int i = 0; i < t_size; i++) {
        if(table[i].priority > 0) {
            ft_size += 1;
        }
    }

    struct ft_record *ft_table;
    ft_table = xcalloc("write_freq_table", ft_size, sizeof(*ft_table));

    uint32_t offset = 0;
    for(int i = 0; i < t_size; i++) {
        if(table[i].priority > 0) {
            ft_table[i - offset].character = (char)i;
            ft_table[i - offset].frequency = table[i].priority;
        }else offset++;
    }

    uint32_t *ptr = (uint32_t *) ft_table;

    *buf = ptr;
    return ft_size;
}

void create_header(struct huff_hdr *hdr, const uint8_t flags, const uint8_t ft_size)
{
    memset(hdr, 0, sizeof(*hdr));
    // HFS
    hdr->magic[0] = 0x48;
    hdr->magic[1] = 0x1c;

    hdr->version[0] = 0;
    hdr->version[1] = 1;

    hdr->flags = flags;
    hdr->ft_len = ft_size;

    if(flags & F_CRC_HEADER_EXISTS) {
        const uint16_t crc = checksum(hdr, sizeof(*hdr));
        hdr->checksum[1] = (uint8_t)(crc      & 255);
        hdr->checksum[0] = (uint8_t)(crc >> 8 & 255);
    }
}

void create_file_header(const char *f_name, struct f_hdr *f_hdr, FILE *file, const uint16_t blk_cnt, const uint8_t flags)
{
    memset(f_hdr, 0, sizeof(*f_hdr));

    if(flags & F_FILE_ATTRIBUTES_STORED) {
        struct stat f_stat;

        const int fd = fileno(file);
        fstat(fd, &f_stat);

        memcpy(&f_hdr->f_name, f_name, 100);
        memcpy(&f_hdr->f_mode, &f_stat.st_mode, sizeof(f_stat.st_mode));
        memcpy(&f_hdr->f_own_id, &f_stat.st_uid, sizeof(f_stat.st_uid));
        memcpy(&f_hdr->f_grp_id, &f_stat.st_gid, sizeof(f_stat.st_gid));
        memcpy(&f_hdr->f_mtime, &f_stat.st_mtim, sizeof(f_stat.st_mtim));
        memcpy(&f_hdr->f_size, &f_stat.st_size, sizeof(f_stat.st_size));
    }

    f_hdr->f_blk_cnt[1] = (uint8_t)(blk_cnt      & 255);
    f_hdr->f_blk_cnt[0] = (uint8_t)(blk_cnt >> 8 & 255);

    if(flags & F_CRC_FILES_EXISTS) {
        const uint16_t crc = checksum(f_hdr, sizeof(*f_hdr));
        f_hdr->checksum[1] = (uint8_t)(crc      & 255);
        f_hdr->checksum[0] = (uint8_t)(crc >> 8 & 255);
    }
}

void encode_file(struct achv_file *buf, FILE *file, const char *f_name, const uint8_t flags, char **codes)
{
    uint16_t blk_cnt;
    struct f_blk *f_blks_start;
    struct f_blk *f_blks;
    uint16_t f_next_bit;

    f_blks = xmalloc("encode_file", sizeof(*f_blks));

    f_blks_start = f_blks;
    blk_cnt = 1;
    f_next_bit = 0;

    char ch;
    uint8_t sum = 0;
    uint8_t bit = 1;

    while((ch = fgetc(file)) != EOF) {
        const char *code = codes[(int)ch];
        const int c_len = strlen(code);

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

                f_blks = f_blks->next;
                f_next_bit = 0;
                blk_cnt += 1;

                f_blks->data[f_next_bit] = sum;
                sum = 0;
                bit = 1;
            }
        }
    }

    rewind(file);

    buf->f_blks = f_blks_start;
    create_file_header(f_name, &buf->f_hdr, file, blk_cnt, flags);
}

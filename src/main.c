#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <getopt.h>
#include <pwd.h>
#include <grp.h>
#include "utils.h"
#include "h_stack.h"
#include "archiver.h"

#define VER 1.0

static int files_cnt;
static uint8_t flags;

void write_achv(const char *dst, const struct huff_achv *src);
size_t read_achv(struct huff_achv **dst, FILE *src);
void list_of_files(struct huff_achv *achv, size_t f_cnt);

void version(void)
{
    printf("huffer version %.1f\n", VER);
    fputs("\
Copyright (C) 2025 Free Software Foundation, Inc.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n\
\n\
", stdout);
}

void usage(int status)
{
    if (status != EXIT_SUCCESS) {
        printf("huffer: No files selected for compression.\n"
               "Try 'huffer -h' or 'huffer --help' for more information.");
    } else {
        printf("Usage: huffer [OPTION]... [FILE]...\n");
        fputs("\
Compression and decompression of FILES \
using pure Huffman algorithm.\n\
\n\
", stdout);

        fputs("\
Examples:\n\
  huffer -c foo bar -o archive.hff\
  # create archivee.hff from files foo and bar\n\
  huffer -x archive.hff -o ./\
       # extract all files from archive.hff\n\
  huffer -l archive.hff\
             # list all files in archive.hff\n\
\n\
", stdout);

        fputs("\
Keys:\n\
  -c, --create=<file1> <file2>  ... Create a new archive from files <file1>,<file2>,...\n\
  -h, --help                        Show this help\n\
  -l, --list=<file>                 List the contents of the archive <file>\n\
  -o, --output=<file>               Place the output into <file>\n\
                                    Can be a file or directory\n\
  -v, --version                     Display archiver version information\n\
  -x, --extract=<achv1> <achv2> ... Extract files from an archives <achv1>,<achv2>,...\n\
\n\
Modifiers:\n\
  --no-crc-files-check              Do not check files checksum during extraction\n\
  --no-crc-header-check             Do not check archive checksum during extraction\n\
  --no-preserve-file-attributes     Do not preserve file attributes\n\
\n\
", stdout);
    }
}

void read_file_paths(char ***buf, int argc, char **argv)
{
    h_stack *stack;
    stack = xmalloc("main", sizeof(*stack));

    optind--;
    for( ;optind < argc && *argv[optind] != '-'; optind++){
        char *path = xcalloc("main", 4096, sizeof(*path));
        memcpy(path, argv[optind], strlen(argv[optind]));
        push_sck(stack, path);
    }

    *buf = xmalloc("main", stack->size * sizeof(**buf));
    while(stack->size > 0){
        char *path;
        pop_sck(stack, (void **)&path);
        (*buf)[stack->size-1] = path;
    }

    free(stack);
}

int main(int argc, char **argv)
{
    int  command = 0;
    char **f_pths;
    char *output;
    int  c;

    output = xcalloc("main", 4096, sizeof(*output));
    memcpy(output, "./", 2);

    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
                {"create",                      1, 0, 'c'},
                {"help",                        0, 0, 'h'},
                {"list",                        1, 0, 'l'},
                {"output",                      1, 0, 'o'},
                {"version",                     0, 0, 'v'},
                {"extract",                     1, 0, 'x'},
                {"no-crc-files-check",          0, 0,  0 },
                {"no-crc-header-check",         0, 0,  0 },
                {"no-preserve-file-attributes", 0, 0,  0 },
                {0, 0, 0, 0}
        };

        c = getopt_long(argc, argv, "c:hl:o:vx:",
                        long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 0:
                if (strcmp(long_options[option_index].name, "no-crc-files-check") == 0) {
                    printf("Skipping file checksum verification\n");
                    flags |= F_CRC_HEADER_EXISTS;
                } else if (strcmp(long_options[option_index].name, "no-crc-header-check") == 0) {
                    printf("Skipping archive checksum verification\n");
                    flags |= F_CRC_FILES_EXISTS;
                } else if (strcmp(long_options[option_index].name, "no-preserve-file-attributes") == 0) {
                    printf("Not preserving file attributes\n");
                    flags |= F_FILE_ATTRIBUTES_STORED;
                }
                break;

            case 'c':
                command = 1;
                read_file_paths(&f_pths, argc, argv);
                break;

            case 'h':
                usage(EXIT_SUCCESS);
                exit(EXIT_SUCCESS);

            case 'l':
                FILE *f;
                f = fopen(optarg, "r");
                struct huff_achv *eachv;
                files_cnt = read_achv(&eachv, f);
                list_of_files(eachv, files_cnt);
                exit(EXIT_SUCCESS);

            case 'o':
                memset(output, 0, 4096*sizeof(*output));
                memcpy(output, optarg, strlen(optarg));
                break;

            case 'v':
                version();
                exit(EXIT_SUCCESS);

            case 'x':
                command = 2;
                read_file_paths(&f_pths, argc, argv);
                break;

            default:
                usage(EXIT_FAILURE);
                exit(EXIT_FAILURE);
        }
    }

    if(command == 0){
        usage(EXIT_SUCCESS);
        usage(EXIT_SUCCESS);
    }

    if(command == 1){
        if(f_pths == NULL || files_cnt == 0){
            usage(EXIT_FAILURE);
            exit(EXIT_FAILURE);
        }
        struct huff_achv *achv = create_archive(f_pths, files_cnt, flags);
        write_achv(output, achv);
    }

    if(command == 2){
        for(int i = 0; i < files_cnt; i++){
            FILE *f;

            f = fopen(f_pths[i], "r");

            FILE **f_lst;

            struct huff_achv *eachv;
            size_t af_cnt = read_achv(&eachv, f);

            f_lst = extract_archive(eachv, "/home/yukir/", af_cnt);

            for(int j = 0; j < af_cnt; j++){
                fflush(f_lst[j]);
                fclose(f_lst[j]);
            }
        }
    }

    return 0;
}

void write_achv(const char *dst, const struct huff_achv *src)
{
    FILE *f;
    int  fd;

    f = fopen(dst, "w");
    if (f == NULL) {
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
    for (int i = 0; i < ft_size; i++) {
        fwrite(&src->freq_table[i], sizeof(*src->freq_table), 1, f);
    }

    // write 'f_cnt' files
    const struct achv_file *files = src->files;
    for (int i = 0; i < files_cnt; i++) {
        const struct achv_file *file = &files[i];

        // write 'i' file header
        fwrite(&file->f_hdr, sizeof(files[i].f_hdr), 1, f);

        uint32_t blk_cnt;
        memcpy(&blk_cnt, file->f_hdr.f_blk_cnt, sizeof(blk_cnt));

        uint16_t tail_len;
        memcpy(&tail_len, file->f_hdr.f_tail_len, sizeof(tail_len));

        const struct f_blk *blk = file->f_blks;

        if (tail_len > 0)
            blk_cnt -= 1;

        // write 'blk_cnt-1' blocks
        for (int j = 0; j < blk_cnt; j++) {
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

    if (hdr[0] != 0x48 || hdr[1] != 0x1c) {
        fprintf(stderr, "read_achv_hdr: Invalid file type");
        exit(EXIT_FAILURE);
    }

    if (hdr[3] & F_CRC_HEADER_EXISTS) {
        uint16_t hdr_crc;
        hdr_crc  = hdr[7] << 8;
        hdr_crc += hdr[6];

        memset(hdr + 6, 0, 2);
        if (checksum(hdr, sizeof(hdr)) != hdr_crc) {
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

    // read archive header
    read_achv_hdr((uint8_t *) &achv->hdr, src);

    ft_size = 0;
    memcpy(&ft_size, &achv->hdr.ft_len, sizeof(uint16_t));

    struct ft_record *ft_table;
    ft_table = xcalloc("write_freq_table", ft_size, sizeof(*ft_table));

    // read frequency table
    for (int i = 0; i < ft_size; i++) {
        fread(&ft_table[i], sizeof(*ft_table), 1, src);
    }

    h_stack *stack = xmalloc("read_achv", sizeof(*stack));

    const size_t f_hdr_size = sizeof(struct f_hdr);

    uint8_t h_size = 0;
    uint8_t hdr[f_hdr_size];

    // read files
    int ch;
    while ((ch = fgetc(src)) != EOF) {
        // read file header
        hdr[h_size++] = ch;

        if (h_size == f_hdr_size) {
            h_size = 0;
            uint32_t blk_cnt;
            memcpy(&blk_cnt, &hdr[136], sizeof(blk_cnt));

            uint16_t tail_len;
            memcpy(&tail_len, &hdr[140], sizeof(tail_len));

            struct f_blk *blk_start;
            struct f_blk *blk;
            blk = xcalloc("read_achv", 1, sizeof(*blk));

            blk_start = blk;

            // read file blocks
            if (tail_len > 0)
                blk_cnt -= 1;

            for (int i = 0; i < blk_cnt; i++) {
                fread(&blk->data, 1, BLK_SIZE, src);
                blk->next = xcalloc("read_achv", 1, sizeof(*blk));
                blk = blk->next;
            }

            fread(&blk->data, 1, tail_len, src);

            struct achv_file *achv_f;
            achv_f = xcalloc("read_achv", 1, sizeof(*achv_f));

            achv_f->f_blks = blk_start;
            memcpy(&achv_f->f_hdr, hdr, sizeof(achv_f->f_hdr));

            // add file to stack
            push_sck(stack, achv_f);
        }
    }

    rewind(src);

    f_cnt = stack->size;

    achv->freq_table = (uint32_t *) ft_table;
    achv->files = xmalloc("read_achv", f_cnt * sizeof(*achv->files));

    while (stack->size > 0) {
        struct achv_file *file;
        pop_sck(stack, (void **) &file);
        memcpy(&achv->files[stack->size], file, sizeof(*file));
    }

    *dst = achv;

    free(stack);
    return f_cnt;
}

void list_of_files(struct huff_achv *achv, const size_t f_cnt)
{
    struct f_hdr  *f_hdr;
    uint32_t      f_mode;
    uint32_t      f_own_id;
    uint32_t      f_grp_id;
    uint64_t      f_size;

    for (size_t i = 0; i < f_cnt; i++) {
        f_hdr = &achv->files[i].f_hdr;

        int offst = 11;
        char mode[offst];

        memcpy(&f_mode, f_hdr->f_mode, sizeof(f_mode));
        memcpy(&f_own_id, f_hdr->f_own_id, sizeof(f_own_id));
        memcpy(&f_grp_id, f_hdr->f_grp_id, sizeof(f_grp_id));
        memcpy(&f_size, f_hdr->f_size, sizeof(f_size));

        mode[--offst] = '\0';
        for (int j = 0; j < 3; j++) {
            uint8_t rght = f_mode % 8;
            for (int bit = 1; bit < 8; bit <<= 1) {
                // ascii:
                // 'r' - 114
                // 'w' - 119
                // 'x' - 120
                // ==> 121 - 1 = 120 # (bit=1 <= its exec  mode)
                // ==> 121 - 2 = 119 # (bit=2 <= its write mode)
                // ==> 121 - 4 = 117 # (bit=4 <= its read  mode), but 117 is 'u'
                //     That's why if 121-bit=117 we subtract 3
                int ch = 121 - bit;
                mode[--offst] = rght & bit ? (ch == 117 ? ch - 3 : ch) : '-';
            }
            f_mode /= 8;
        }
        mode[--offst] = '-';

        struct passwd *pws;
        pws = getpwuid(f_own_id);

        struct group *grp;
        grp = getgrgid(f_grp_id);

        struct timespec ts;
        memcpy(&ts, f_hdr->f_mtime, sizeof(ts));

        const struct tm *t = gmtime(&ts.tv_sec);

        printf("%s %s/%s %lu ", mode, pws->pw_name, grp->gr_name, f_size);
        printf(
                "%04d-%02d-%02d %02d:%02d",
                t->tm_year + 1900,
                t->tm_mon + 1,
                t->tm_mday,
                t->tm_hour,
                t->tm_min
        );
        printf(" %s\n", f_hdr->f_name);
    }
}

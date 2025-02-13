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
    } f_hdr;

    struct f_blk {
        uint8_t data[BLK_SIZE];
        struct f_blk *next;
    } *f_blks;

};

struct huff_achv {

    struct huff_hdr {
        uint8_t  magic[2];
        uint8_t  version;
        uint8_t  flags;
        uint8_t  ft_len[2];
        uint8_t  checksum[2];
    } hdr;

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

/**
 *
 * @param b - pointer to the data buffer
 * @param len - length of the buffer in bytes
 * @return 16-bit checksum value
 *
 * function calculates the checksum as described in
 * "RFC 1071 - Computing the Internet Checksum".
 *
 * RFC 1071:
 * (1)  Adjacent octets to be checksummed are paired to form 16-bit
 *      integers, and the 1's complement sum of these 16-bit integers is
 *      formed.
 *
 * (2)  To generate a checksum, the checksum field itself is cleared,
 *      the 16-bit 1's complement sum is computed over the octets
 *      concerned, and the 1's complement of this sum is placed in the
 *      checksum field.
 *
 * (3)  To check a checksum, the 1's complement sum is computed over the
 *      same set of octets, including the checksum field.  If the result
 *      is all 1 bits (-0 in 1's complement arithmetic), the check
 *      succeeds.
 */
uint16_t checksum(const void *b, size_t len);


/**
 *
 * @param f_pths - array of file paths to be archived
 * @param f_cnt - number of files in the array
 * @param flags - flags controlling the archive creation
 * @return pointer to the created archive or NULL
 *
 * function creates a Huffman-compressed archive from the given files.
 *
 * the files specified in 'f_pths' are processed, compressed using Huffman coding,
 * and written into an archive format
 */
void *create_archive(char **f_pths, size_t f_cnt, uint8_t flags);

/**
 *
 * @param achv - pointer to the Huffman archive structure
 * @param dst - destination directory for extracted files
 * @param f_cnt - number of files to extract
 * @return pointer to extraction files buffers (FILE **) or NULL
 *
 * function extracts files from a Huffman-compressed archive.
 *
 * the function reads the archive structure 'achv', decodes the Huffman-compressed
 * data, and writes the extracted files buffers
 */
void *extract_archive(const struct huff_achv *achv, const char *dst, size_t f_cnt);

#include <stdlib.h>
#include "archiver.h"

int main(void)
{
    char **files;

    char file[] = "/home/yukir/CLionProjects/huffman-archiver/test.txt";
    files = calloc(1, sizeof(*files));
    files[0] = file;

    uint8_t flags = 0;

    flags |= F_CRC_HEADER_EXISTS;
    flags |= F_CRC_FILES_EXISTS;
    flags |= F_FILE_ATTRIBUTES_STORED;

    struct huff_achv *achv = create_archive(files, 1, flags);

    printf("frequency table: \n");
    for(int i = 0; i < achv->hdr.ft_len; i++) {
        struct ft_record record = ((struct ft_record *)achv->freq_table)[i];
        printf("%c == %d\n", record.character, record.frequency);
    }

    printf("\n");
    printf("number of occupied blocks: %d", achv->files->f_hdr.f_blk_cnt[1]);

    free(files);
    return 0;
}

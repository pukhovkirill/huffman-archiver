#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "archiver.h"

#include "../utils.h"

TEST_FUNCT(create_archive)
{
    char **files;

    char file[] = "../../tests/resources/test.txt";
    files = calloc(1, sizeof(*files));
    files[0] = file;

    uint8_t flags = 0;

    flags |= F_CRC_HEADER_EXISTS;
    flags |= F_CRC_FILES_EXISTS;
    flags |= F_FILE_ATTRIBUTES_STORED;

    struct huff_achv *achv = create_archive(files, 1, flags);

    CU_ASSERT(achv != NULL)
    CU_ASSERT(achv->hdr.ft_len[0] == 5)

    CU_ASSERT(achv->files != NULL)
    CU_ASSERT(achv->files->f_hdr.f_blk_cnt[0] == 1)
    CU_ASSERT(achv->files->f_hdr.f_tail_len[0] == 3)

    CU_ASSERT(achv->files->f_blks != NULL)
    CU_ASSERT(achv->files->f_blks->data[0] == 118)
    CU_ASSERT(achv->files->f_blks->data[1] == 21)
    CU_ASSERT(achv->files->f_blks->data[2] == 59)
    CU_ASSERT(achv->files->f_blks->next == NULL)

    free(achv->freq_table);
    free(achv->files);
    free(achv);
    free(files);
}

void runSuite(void)
{
    const CU_pSuite suite = CUnitCreateSuite("archiver_suite");
    if (suite) {
        ADD_SUITE_TEST(suite, create_archive)
    }
}

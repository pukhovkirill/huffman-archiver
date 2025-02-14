/*
Copyright (c) 2025 Kirill Pukhov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "archiver.h"

#include "../utils.h"

TEST_FUNCT(extract_archive)
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

    FILE *test_f = fopen(files[0], "r");
    FILE **f_lst;

    f_lst = extract_archive(achv, "../../tests/resources/out/", 1);

    CU_ASSERT(f_lst != NULL)
    CU_ASSERT(f_lst[0] != NULL)

    fputc(EOF, f_lst[0]);
    fflush(f_lst[0]);
    f_lst[0] = freopen("../../tests/resources/test.txt", "r", f_lst[0]);

    int ch1;
    int ch2;
    while((ch1 = fgetc(f_lst[0])) != EOF && (ch2 = fgetc(test_f)) != EOF){
        CU_ASSERT(ch1 == ch2)
    }

    ch1 = fgetc(f_lst[0]);
    ch2 = fgetc(test_f);

    CU_ASSERT(ch1 == ch2)
    CU_ASSERT(ch1 == EOF)

    fclose(test_f);
    fclose(f_lst[0]);

    free(achv->freq_table);
    free(achv->files);
    free(achv);
    free(files);
}

void runSuite(void)
{
    const CU_pSuite suite = CUnitCreateSuite("extract_suite");
    if (suite) {
        ADD_SUITE_TEST(suite, extract_archive)
    }
}
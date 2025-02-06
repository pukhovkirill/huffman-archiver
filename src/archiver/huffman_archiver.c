#include "archiver.h"
#include "h_tree.h"

static uint16_t checksum(const void *b, size_t len);

void *create_archive(const FILE *fls, size_t f_cnt)
{

}

void *extract_archive(const FILE *achv)
{

}

void *append_to_archive(const FILE *achv, const FILE *fls, size_t f_cnt)
{

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

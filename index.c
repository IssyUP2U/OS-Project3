// index.c
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "index.h"

//endian checker
static int is_bigendian(void)
{
    int x = 1;
    return ((uint8_t *)&x)[0] != 1;
}
//endian reverser
static uint64_t reverse_bytes(uint64_t x)
{
    uint8_t dest[8];
    uint8_t *src = (uint8_t *)&x;

    for (int i = 0; i < 8; i++)
        dest[i] = src[7 - i];

    uint64_t r;
    memcpy(&r, dest, 8);
    return r;
}

static uint64_t to_big(uint64_t x)
{
    return is_bigendian() ? x : reverse_bytes(x);
}

static uint64_t from_big(uint64_t x)
{
    return is_bigendian() ? x : reverse_bytes(x);
}

//IO functions (declared in index.h) 
//header read/write
int read_header(FILE *fp, index_header_t *hdr)
{
    if (fseek(fp, 0, SEEK_SET) != 0)
        return -1;

    if (fread(hdr, BLOCK_SIZE, 1, fp) != 1)
        return -1;

    hdr->rootID    = from_big(hdr->rootID);
    hdr->nextBlock = from_big(hdr->nextBlock);

    return 0;
}

int write_header(FILE *fp, index_header_t *hdr)
{
    index_header_t temp = *hdr;

    temp.rootID    = to_big(temp.rootID);
    temp.nextBlock = to_big(temp.nextBlock);

    if (fseek(fp, 0, SEEK_SET) != 0)
        return -1;

    if (fwrite(&temp, BLOCK_SIZE, 1, fp) != 1)
        return -1;

    fflush(fp);
    return 0;
}

//node read/write
void read_node(FILE *fp, uint64_t blockID, btree_node_t *node)
{
    uint64_t offset = blockID * BLOCK_SIZE;
    fseek(fp, offset, SEEK_SET);

    uint8_t buffer[BLOCK_SIZE];
    if (fread(buffer, BLOCK_SIZE, 1, fp) != 1) {
        perror("fread");
        exit(1);
    }

    uint64_t *p = (uint64_t *)buffer;

    node->blockID = from_big(p[0]);
    node->parentNode   = from_big(p[1]);
    node->keyCount = from_big(p[2]);

    for (int i = 0; i < MAX_KEYS; i++)
        node->keys[i] = from_big(p[3 + i]);

    for (int i = 0; i < MAX_KEYS; i++)
        node->values[i] = from_big(p[3 + MAX_KEYS + i]);

    for (int i = 0; i < MAX_CHILDREN; i++)
        node->children[i] =
            from_big(p[3 + 2 * MAX_KEYS + i]);
}

void write_node(FILE *fp, btree_node_t *node)
{
    uint8_t buffer[BLOCK_SIZE];
    memset(buffer, 0, BLOCK_SIZE);

    uint64_t *p = (uint64_t *)buffer;

    p[0] = to_big(node->blockID);
    p[1] = to_big(node->parentNode);
    p[2] = to_big(node->keyCount);

    for (int i = 0; i < MAX_KEYS; i++)
        p[3 + i] = to_big(node->keys[i]);

    for (int i = 0; i < MAX_KEYS; i++)
        p[3 + MAX_KEYS + i] = to_big(node->values[i]);

    for (int i = 0; i < MAX_CHILDREN; i++)
        p[3 + 2 * MAX_KEYS + i] =
            to_big(node->children[i]);

    uint64_t offset = node->blockID * BLOCK_SIZE;
    fseek(fp, offset, SEEK_SET);
    fwrite(buffer, BLOCK_SIZE, 1, fp);
    fflush(fp);
}

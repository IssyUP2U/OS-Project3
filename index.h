//Isabella Sobey 
//ims190003

#ifndef INDEX_H
#define INDEX_H

#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOCK_SIZE 512
#define BTREE_DEGREE 10 //minimal degree 10
#define MAX_KEYS (2 * BTREE_DEGREE - 1) //19 keys
#define MAX_CHILDREN (2 * BTREE_DEGREE) //20 children

typedef struct { //header
    char magicNum[8];        
    uint64_t rootID;     
    uint64_t nextBlock;  
    char unused[BLOCK_SIZE - 24];
} index_header_t;

int create_index_file(const char *filename);

//--------------------------------split header and node :)

typedef struct { //Node block
    uint64_t blockID;
    uint64_t parentNode;
    uint64_t keyCount;
    uint64_t keys[MAX_KEYS];
    uint64_t values[MAX_KEYS];
    uint64_t children[MAX_CHILDREN];
    char unused[BLOCK_SIZE - (8 + 8 + 8 + MAX_KEYS*8 + MAX_KEYS*8 + MAX_CHILDREN*8)];
}btree_node_t;


int btree_insert(const char *filename, uint64_t key, uint64_t value);
int btree_search(const char *filename, uint64_t key);
int btree_load_csv(const char *filename, const char *csvfile);
int btree_print(const char *filename);
int btree_extract(const char *filename, const char *outfile);


//--------------------------------split other functions @_@

//function declarations
int write_header(FILE *fp, index_header_t *);
int read_header(FILE *fp, index_header_t *);
void read_node(FILE *, uint64_t, btree_node_t *);
void write_node(FILE *, btree_node_t *);

//big <-> little endian conversion declaration
uint64_t to_big_endian(uint64_t x);
uint64_t from_big_endian(uint64_t x);

#endif
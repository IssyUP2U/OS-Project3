//Isabella Sobey 
//ims190003

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

typedef struct { //Node block
    uint64_t blockID;
    uint64_t parentNode;
    uint64_t keyCount;
    uint64_t keys[MAX_KEYS];
    uint64_t values[MAX_KEYS];
    uint64_t children[MAX_CHILDREN];
    char unused[BLOCK_SIZE - (8 + 8 + 8 + MAX_KEYS*8 + MAX_KEYS*8 + MAX_CHILDREN*8)];
}btree_node_t;

//function declarations
int write_header(FILE *fp, index_header_t *hdr);
int read_header(FILE *fp, index_header_t *hdr);

//big <-> little endian conversion declaration
uint64_t to_big_endian(uint64_t x);
uint64_t from_big_endian(uint64_t x);

#endif
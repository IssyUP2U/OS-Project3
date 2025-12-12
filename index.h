//Isabella Sobey 
//ims190003

#define BLOCK_SIZE 512
#define BTREE_DEGREE 10 //minimal degree 10
#define MAX_KEYS (2 * BTREE_DEGREE - 1) //19 keys
#define MAX_CHILDREN (2 * BTREE_DEGREE) //20 children
typedef struct {
    char magicNum[8];        
    uint64_t root_id;     
    uint64_t next_block;  
    char unused[BLOCK_SIZE - 24];
} index_header_t;
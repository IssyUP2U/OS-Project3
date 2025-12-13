//ims190003
#include "index.h"
int btree_search(const char *filename, uint64_t key);
int btree_insert(const char *filename, uint64_t key, uint64_t value);
int btree_print(const char *filename);
int btree_extract(const char *filename, const char *outname);
int btree_load_csv(const char *filename, const char *csvfile);


//create command function
static int cmd_create(const char *filename)
{
    if (access(filename, F_OK) == 0) {
        fprintf(stderr, "File already exists\n");
        return 1;
    }

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("open");
        return 1;
    }

    index_header_t hdr;
    memset(&hdr, 0, sizeof(hdr));

    memcpy(hdr.magicNum, "4348PRJ3", 8);
    hdr.rootID = 0;
    hdr.nextBlock = 1;  // block 0 is header

    if (write_header(fp, &hdr) != 0) {
        fclose(fp);
        return 1;
    }

    fclose(fp);
    return 0;
}


//main function ---------------------------------- o_o

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *cmd = argv[1]; //accept command line argument
    const char *index = argv[2]; //file name argument
    //create function
    if (strcmp(cmd, "create") == 0) {
        if (argc != 3) { //error checking
            fprintf(stderr, "Usage: %s create <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

        return cmd_create(index);
    }

    //insert function
    if (strcmp(cmd, "insert") == 0) {
        if (argc != 5) { //error checking
            fprintf(stderr, "Usage: %s insert <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

        uint64_t key = strtoull(argv[3], NULL, 10);
        uint64_t value = strtoull(argv[4], NULL, 10);
        return btree_insert(index, key, value);
    }

    //search function
    if (strcmp(cmd, "search") == 0) {
        if (argc != 4) { //error checking
            fprintf(stderr, "Usage: %s search <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

        uint64_t key = strtoull(argv[3], NULL, 10);
        return btree_search(index, key);
    } 

    //load function
    if (strcmp(cmd, "load") == 0) {
        if (argc != 4) { //error checking
            fprintf(stderr, "Usage: %s load <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

        return btree_load_csv(index, argv[3]);
    }

    //print function
    if (strcmp(cmd, "print") == 0) {
        if (argc != 3) { //error checking
            fprintf(stderr, "Usage: %s print <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

        return btree_print(index);
    }  

    //extract function
    if (strcmp(cmd, "extract") == 0) {
        if (argc != 4) { //error checking
            fprintf(stderr, "Usage: %s extract <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

        //const char *idxfile = argv[2];
        //const char *outfile = argv[3];

        return btree_extract(index, argv[3]);
    }

    fprintf(stderr, "The command %s does not exist.\n", cmd);
    return 1;
}
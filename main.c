//ims190003
#include "index.h"
#include "btree.c"

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *cmd = argv[1]; //accept command line argument

    //create function
    if (strcmp(cmd, "create") == 0) {
        if (argc != 3) { //error checking
            fprintf(stderr, "Usage: %s create <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

        const char *idxfile = argv[2];
        return create_index_file(idxfile);
    }

    //insert function
    if (strcmp(cmd, "insert") == 0) {
        if (argc != 5) { //error checking
            fprintf(stderr, "Usage: %s insert <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

        const char *idxfile = argv[2];
        uint64_t key = strtoull(argv[3], NULL, 10);
        uint64_t value = strtoull(argv[4], NULL, 10);

        return btree_insert(idxfile, key, value);
    }

    //search function
    if (strcmp(cmd, "search") == 0) {
        if (argc != 4) { //error checking
            fprintf(stderr, "Usage: %s search <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

        const char *idxfile = argv[2];
        uint64_t key = strtoull(argv[3], NULL, 10);

        return btree_search(idxfile, key);
    } 

    //load function
    if (strcmp(cmd, "load") == 0) {
        if (argc != 4) { //error checking
            fprintf(stderr, "Usage: %s load <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

        const char *idxfile = argv[2];
        const char *csvfile = argv[3];

        return btree_load_csv(idxfile, csvfile);
    }

    //print function
    if (strcmp(cmd, "print") == 0) {
        if (argc != 3) { //error checking
            fprintf(stderr, "Usage: %s print <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

        const char *idxfile = argv[2];

        return btree_print(idxfile);
    }  

    //extract function
    if (strcmp(cmd, "extract") == 0) {
        if (argc != 4) { //error checking
            fprintf(stderr, "Usage: %s extract <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }

         const char *idxfile = argv[2];
        const char *outfile = argv[3];

        return btree_extract(idxfile, outfile);
    }

    return 0;
}
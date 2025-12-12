//ims190003
#include "index.h"

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
    }

    //insert function
    if (strcmp(cmd, "insert") == 0) {
        if (argc != 5) { //error checking
            fprintf(stderr, "Usage: %s insert <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    //search function
    if (strcmp(cmd, "search") == 0) {
        if (argc != 4) { //error checking
            fprintf(stderr, "Usage: %s search <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }
    } 

    //load function
    if (strcmp(cmd, "load") == 0) {
        if (argc != 4) { //error checking
            fprintf(stderr, "Usage: %s load <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    //print function
    if (strcmp(cmd, "print") == 0) {
        if (argc != 3) { //error checking
            fprintf(stderr, "Usage: %s print <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }
    }  

    //extract function
    if (strcmp(cmd, "extract") == 0) {
        if (argc != 4) { //error checking
            fprintf(stderr, "Usage: %s extract <index_file>\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    return 0;
}
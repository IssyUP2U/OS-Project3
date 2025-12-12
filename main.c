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
        
    }

    //insert function
    if (strcmp(cmd, "insert") == 0) {

    }

    //search function
    if (strcmp(cmd, "search") == 0) {

    } 

    //load function
    if (strcmp(cmd, "load") == 0) {

    }

    //print function
    if (strcmp(cmd, "print") == 0) {

    }  

    //extract function
    if (strcmp(cmd, "extract") == 0) {

    }

    return 0;
}
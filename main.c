#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archive_definition.h"

int main(int argc, char *argv[]) {
    int size[argc - 1];
    unsigned char **data = read_data(argc - 1, argv, size);
    if (data == NULL) {
	    exit(1);
    }
    arch_def(argc - 1, data, argv, size);
    free_mem(argc - 1, data);
    return 0;
}

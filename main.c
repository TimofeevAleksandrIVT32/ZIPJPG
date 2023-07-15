#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "archive_definition.h"

int main(int argc, char *argv[]) {
    int res = read_data(argc - 1, argv);
    if (res != 0) {
	    exit(1);
    }
    return 0;
}

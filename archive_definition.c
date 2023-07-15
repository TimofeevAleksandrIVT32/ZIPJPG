#define FIX_PART_STRUC 30

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "archive_definition.h"

//считываем данные из файла в строку data
unsigned int read_data(int amount, char *files[]) {
    if (amount < 1) {
        printf("File name not entered\n");
        return 1;
    }
    unsigned char *data;
    for (int i = 0; i < amount; i++) {
        FILE *file = fopen(files[i + 1], "rb");
        if (!file) {
            printf("Failed to open file\n");
	        return 1;
        }

        int size = file_size(file);
        data = str_memory_allocation(size);
        if (data == NULL) {
            fclose(file);
		    return 1;
    	}

        int nbytes = fread(data, 1, size, file);
        if (nbytes != size) {
            printf("Failed to read data from file\n");
            free(data);
            fclose(file);
	        return 1;
        }
        
        fclose(file);
        arch_def(data, files[i+1], size);
        free(data);
    }
    return 0;
}

//выделяем память для строки
unsigned char *str_memory_allocation(int size) {
    unsigned char *str = (unsigned char*)malloc((size + 1) * sizeof(unsigned char));
    if (str == NULL) {
        printf("Memory allocation error");
	    return NULL;
    }
    return str;
}

unsigned int file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    unsigned int size = ftell(file);
    rewind(file);
    return size;
}

//определяем, является ли файл zipjpegом
void arch_def(unsigned char *data, char *file, int size){
    unsigned char zip_signature[] = {0x50, 0x4B, 0x05, 0x06}; //сигнатура Zip-архива
    int zip_sign_len = sizeof(zip_signature);
    unsigned char *res = find_sign(data, size, zip_signature, zip_sign_len);
	unsigned char jpg_signature[] = {0xFF, 0xD8}; //сигнатура jpg
    int jpg_sign_len = sizeof(jpg_signature);
    unsigned char *res1 = find_sign(data, size, jpg_signature, jpg_sign_len);
    if (res == NULL || res1 == NULL) {
	printf("The file \"%s\" is not zipjpg\n", file);
    }
    else {
        unsigned char lh_signature[] = {0x50, 0x4B, 0x03, 0x04}; //сигнатура локального заголовка
        int ls_sign_len = sizeof(lh_signature);
        res = find_sign(data, size, lh_signature, ls_sign_len); //ищем локальный заголовок
        printf("The file \"%s\" is zipjpg\n", file);

        while (res != NULL) {
            struct local_file_headers header;
            memcpy(&header, res, FIX_PART_STRUC);
            int name_len = ntohs(header.file_name_len);
            header.file_name = (unsigned char*)malloc(name_len * sizeof(unsigned char));
            if (header.file_name == NULL) {
                printf("Memory allocation error");
	            return;
            }
            memcpy(header.file_name, res+FIX_PART_STRUC, name_len);
            printf("%.*s\n", name_len, header.file_name);

            unsigned int len_copy = size - (res - data) - 4;
            memmove(data, res + 4, len_copy); //отрезаем сигнатуру, чтобы искать следующую
            size = len_copy;
            res = find_sign(data, size, lh_signature, ls_sign_len); //ищем следующий локальный заголовок
            free(header.file_name);
        }
    }
}
 
unsigned char *find_sign(unsigned char *data, unsigned int datalen, unsigned char signature[], int signlen) {
    if (data == NULL || signature == NULL || datalen == 0 || signlen == 0) {
        return NULL;
    }
    for (int i = 0; i <= (int)(datalen - signlen); i++) {
        if (memcmp(data + i, signature, signlen) == 0) {
            return (data + i);
        }
    }
    return NULL;
}

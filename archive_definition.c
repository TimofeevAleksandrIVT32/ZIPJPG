#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archive_definition.h"

//считываем данные из файла в строку data[i]
unsigned char **read_data(int amount, char *files[], int size[]) {
    unsigned char **data = arr_memory_allocation(amount);
    if (data == NULL) {
	    return NULL;
    }
    for (int i = 0; i < amount; i++) {
        FILE *file = fopen(files[i + 1], "rb");
        if (!file) {
            printf("Failed to open file\n");
	        return NULL;
        }

        size[i] = file_size(file);
        unsigned char buffer[size[i]];
        int nbytes = fread(buffer, 1, sizeof(buffer), file);
        if (nbytes != size[i]) {
            printf("Failed to read data from file\n");
	        return NULL;
        }
        fclose(file);

        data[i] = str_memory_allocation(size[i]);
	    if (data[i] == NULL) {
		    return NULL;
    	}
        for (int j = 0; j < size[i]; j++) {
            sprintf((char*)data[i] + j, "%c", buffer[j]);
        }
    }
    return data;
}

//выделяем память для двумерного массива
unsigned char **arr_memory_allocation(int amount) {
    unsigned char **data = (unsigned char**)malloc((amount) * sizeof(unsigned char*));
    if (data == NULL) {
        printf("Memory allocation error");
	    return NULL;
    }
    return data;
}

//выделяем память для строки массива
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

//определяем, содержит ли файл архив
void arch_def(int amount, unsigned char **data, char *files[], int size[]){
    for (int i = 0; i < amount; i++) {
        unsigned char zip_signature[] = {0x50, 0x4B, 0x05, 0x06}; //сигнатура Zip-архива
        int zip_sign_len = sizeof(zip_signature);
        unsigned char *res = find_sign(data[i], size[i], zip_signature, zip_sign_len);
	unsigned char jpg_signature[] = {0xFF, 0xD8}; //сигнатура jpg
        int jpg_sign_len = sizeof(jpg_signature);
        unsigned char *res1 = find_sign(data[i], size[i], jpg_signature, jpg_sign_len);
        if (res == NULL || res1 == NULL) {
	    printf("The file \"%s\" is not zipjpg\n", files[i + 1]);
        }
        else {
            unsigned char lh_signature[] = {0x50, 0x4B, 0x03, 0x04}; //сигнатура локального заголовка
            int ls_sign_len = sizeof(lh_signature);
            res = find_sign(data[i], size[i], lh_signature, ls_sign_len); //ищем локальный заголовок
            printf("The file \"%s\" is not zipjpg\n", files[i + 1]);

            while (res != NULL) {
                const int fix_part_struc = 30; //в структуре до названия файла всегда 30 байт
                struct local_file_headers header;
                memcpy(&header, res, fix_part_struc);
                int name_len = (int)header.file_name_len[0]+(int)header.file_name_len[1]*256;
                header.file_name = (unsigned char*)malloc(name_len * sizeof(unsigned char));
                if (header.file_name == NULL) {
                    printf("Memory allocation error");
	                return;
                }
                memcpy(header.file_name, res+fix_part_struc, name_len);
                ascii_decoding(header.file_name,name_len);

                unsigned int len_copy = size[i] - (res - data[i]) - 4;
                memmove(data[i], res + 4, len_copy); //отрезаем сигнатуру, чтобы искать следующую
                size[i] = len_copy;
                res = find_sign(data[i], size[i], lh_signature, ls_sign_len); //ищем следующий локальный заголовок
                free(header.file_name);
            }
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

//декодируем название файла и выводим на экран
void ascii_decoding(unsigned char encoded[], int length) {
    unsigned char byte[1];
    for (int i = 0; i < length; i++) {
        memcpy(byte, &encoded[i], 1);
        char ascii_code;
        sscanf((const char *)byte, "%c", &ascii_code);
        printf("%c", ascii_code);
    }
    printf("\n");
}

//освобождаем память
void free_mem(int amount, unsigned char **hex) {
    for (int i = 0; i < amount; i++) {
        free(hex[i]);
    }
    free(hex);
}

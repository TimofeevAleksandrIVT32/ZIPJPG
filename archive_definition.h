#ifndef ARCHIVE_DEFINITION_H
#define ARCHIVE_DEFINITION_H

struct local_file_headers {
    unsigned char signature[4];
    unsigned char version[2];
    unsigned char flags[2];
    unsigned char compression[2];
    unsigned char mod_time[2];
    unsigned char mod_date[2];
    unsigned char crc_32[4];
    unsigned char compressed_size[4];
    unsigned char uncompressed_size[4];
    unsigned char file_name_len[2];
    unsigned char extra_field_len[2];
    unsigned char *file_name;
};

unsigned char **read_data(int amount, char *files[], int size[]);
unsigned char **arr_memory_allocation(int amount);
unsigned char *str_memory_allocation(int size);
unsigned int file_size(FILE *file);
unsigned char *find_sign(unsigned char *data, unsigned int datalen, unsigned char signature[], int signlen);
void arch_def(int amount, unsigned char **data, char *files[], int size[]);
void ascii_decoding(unsigned char encoded[], int length);
void free_mem(int amount, unsigned char **hex);

#endif /* ARCHIVE_DEFINITION_H */

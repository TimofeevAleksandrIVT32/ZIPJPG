#ifndef ARCHIVE_DEFINITION_H
#define ARCHIVE_DEFINITION_H

#pragma pack(push,1)
struct local_file_headers {
    uint32_t signature;
    uint16_t version;
    uint16_t flags;
    uint16_t compression;
    uint16_t mod_time;
    uint16_t mod_date;
    uint32_t crc_32;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t file_name_len;
    uint16_t extra_field_len;
    uint8_t *file_name;
};
#pragma pack(pop)

unsigned int read_data(int amount, char *files[]);
unsigned char *str_memory_allocation(int size);
unsigned int file_size(FILE *file);
void arch_def(unsigned char *data, char *file, int size);
unsigned char *find_sign(unsigned char *data, unsigned int datalen, unsigned char signature[], int signlen);

#endif /* ARCHIVE_DEFINITION_H */

#ifndef HUFF_FORMAT_H
#define HUFF_FORMAT_H

#define HUFF_MAGIC "HUFF"
#define BYTE_VALUES 256
#include<stdint.h>



int write_header(int fileDescriptor , uint64_t filesize , uint64_t freq[BYTE_VALUES]);
int read_header(int fileDescriptor, uint64_t *filesize, uint64_t freq[BYTE_VALUES]);
#endif

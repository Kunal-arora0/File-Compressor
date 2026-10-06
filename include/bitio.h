#ifndef BITIO_H
#define BITIO_H



typedef struct BitWriter{
    int fd;
    unsigned char buffer;
    int bits_used;
}BitWriter;

void bitwriter_init(BitWriter *writer, int fd);
void bitwriter_write_bit(BitWriter *writer, unsigned char bit);
void bitwriter_flush(BitWriter *writer);
#endif

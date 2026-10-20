#ifndef BITIO_H
#define BITIO_H



typedef struct BitWriter{
    int fd;
    unsigned char buffer;
    int bits_used;
}BitWriter;

typedef struct BitReader {
    int fd;
    unsigned char buffer;
    int bits_remaining;
} BitReader;


void bitwriter_init(BitWriter *writer, int fd);
void bitwriter_write_bit(BitWriter *writer, unsigned char bit);
void bitwriter_flush(BitWriter *writer);
void bitreader_init(BitReader *reader, int fd);
int bitreader_read_bit(BitReader *reader);
#endif

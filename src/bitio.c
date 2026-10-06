#include<stdio.h>
#include<stdlib.h>
#include "bitio.h"
#include<unistd.h>



void bitwriter_init(BitWriter *writer , int fd){
    writer->fd = fd;
    writer->buffer = 0;
    writer->bits_used = 0;
}

void bitwriter_write_bit(BitWriter *writer, unsigned char bit){

    writer->buffer = writer->buffer << 1 | bit;
    writer->bits_used++;


    if(writer->bits_used == 8){
        write(writer->fd,&writer->buffer, 1);
        writer->buffer = 0;
        writer->bits_used = 0;
    }
}

void bitwriter_flush(BitWriter *writer){
    if(writer->bits_used > 0){
        writer->buffer = writer->buffer << (8 - writer->bits_used);
        write(writer->fd, &writer->buffer, 1); // we only do this so that if the last remaining bits are not completed i.e !=8 then we shift them and make a byte and
                                               // push to the fd,cz write command expects the no of bytes not bit
    }

    writer->buffer = 0;
    writer->bits_used = 0;          
}

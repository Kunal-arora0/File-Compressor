#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include "huff_format.h"

int write_header(int fileDescriptor, uint64_t fileSize, uint64_t freq[BYTE_VALUES]){
    int output = write(fileDescriptor , HUFF_MAGIC , 4);

    if(output != 4){
        return -1;
    }

    unsigned char bytes[8];

    uint64_t originalFileSize = fileSize;

    for(int i = 0; i<8 ;i ++){
        bytes[i] = (unsigned char)(originalFileSize & 0XFF);
        originalFileSize = originalFileSize>>8;
    }

    ssize_t writeFileSize = write(fileDescriptor, bytes, sizeof(bytes));

    if(writeFileSize !=8){
        return -1;
    }

    for (int i = 0; i < BYTE_VALUES; i++) {
        uint64_t temp = freq[i];

        for (int j = 0; j < 8; j++) {
            bytes[j] = (unsigned char)(temp & 0xFF);
            temp >>= 8;
        }

        ssize_t written = write(fileDescriptor, bytes, sizeof(bytes));

        if (written != sizeof(bytes)) {
            return -1;
        }
    }

    return 0;
}

int read_header(int fileDescriptor, uint64_t *fileSize, uint64_t freq[BYTE_VALUES]){

    unsigned char bytes[8];

    int input = read(fileDescriptor, bytes ,4);

    if(input!=4){
        return -1;
    }

    if(memcmp(bytes, HUFF_MAGIC ,4) !=0){
        return -1;
    }

    ssize_t readFileSize = read(fileDescriptor, bytes, 8);
    if(readFileSize != 8){
        return -1;
    }
    *fileSize = 0;

    for(int i=0;i<8;i++){
        *fileSize |=  (uint64_t) bytes[i] << (8*i);
    }



    
        /*
         * Read and reconstruct the frequency table.
         *
         * Each of the 256 possible byte values has a frequency
         * stored as a 64-bit unsigned integer (8 bytes).
         * The frequencies are stored in little-endian format.
         *
         * The outer loop reads 8 bytes for each frequency.
         * The inner loop reconstructs the original integer by
         * shifting each byte to its correct position and combining
         * the results using bitwise OR.
         *
         * Example:
         * If the stored bytes are:
         *     01 02 00 00 00 00 00 00
         *
         * Then:
         *     bytes[0] = 1  -> 1 << 0 = 1
         *     bytes[1] = 2  -> 2 << 8 = 512
         *
         * Combining them:
         *     1 | 512 = 513
         *
         * Therefore, the reconstructed frequency is 513.
         */
        for (int i = 0; i < BYTE_VALUES; i++) {
            ssize_t bytesRead = read(fileDescriptor, bytes, 8);

            if (bytesRead != 8) {
                return -1;
            }

            freq[i] = 0;

            for (int j = 0; j < 8; j++) {
                freq[i] |= (uint64_t)bytes[j] << (8 * j);
            }
        }

    return 0;

}

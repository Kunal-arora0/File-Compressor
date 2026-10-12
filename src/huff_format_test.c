#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "huff_format.h"

int main(void) {
    /* Create a test file and write the header */
    int fd = open("test.huff", O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    uint64_t originalSize = 513;
    uint64_t originalFreq[BYTE_VALUES] = {0};

    originalFreq['A'] = 2;
    originalFreq['B'] = 1;
    originalFreq['C'] = 1;

    if (write_header(fd, originalSize, originalFreq) == -1) {
        fprintf(stderr, "Header writing failed\n");
        close(fd);
        return 1;
    }

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    printf("Header written successfully.\n");

    /* Reopen the file to read the header */
    fd = open("test.huff", O_RDONLY);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    uint64_t recoveredSize = 0;
    uint64_t recoveredFreq[BYTE_VALUES] = {0};

    if (read_header(fd, &recoveredSize, recoveredFreq) == -1) {
        fprintf(stderr, "Header reading failed\n");
        close(fd);
        return 1;
    }

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    /* Verify the recovered file size */
    if (recoveredSize != originalSize) {
        printf("FAIL: File size mismatch.\n");
        printf("Expected: %llu, Got: %llu\n",
               (unsigned long long)originalSize,
               (unsigned long long)recoveredSize);
        return 1;
    }

    /* Verify all 256 frequency values */
    for (int i = 0; i < BYTE_VALUES; i++) {
        if (recoveredFreq[i] != originalFreq[i]) {
            printf("FAIL: Frequency mismatch at index %d.\n", i);
            return 1;
        }
    }

    printf("Recovered file size: %llu bytes\n",
           (unsigned long long)recoveredSize);
    printf("Recovered frequencies: A= %llu, B= %llu, C= %llu\n",
           (unsigned long long)recoveredFreq['A'],
           (unsigned long long)recoveredFreq['B'],
           (unsigned long long)recoveredFreq['C']);

    printf("PASS: File size and all frequencies match!\n");

    return 0;
}

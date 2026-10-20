#include <limits.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "huffman.h"
#include "bitio.h"
#include "huff_format.h"

#define BUF_SIZE 4096
#define BYTE_VALUES 256

void free_huffman_tree(HuffmanNode *node) {
    if (node == NULL) {
        return;
    }

    free_huffman_tree(node->left);
    free_huffman_tree(node->right);
    free(node);
}

void free_codes(char *codes[BYTE_VALUES]) {
    for (int i = 0; i < BYTE_VALUES; i++) {
        free(codes[i]);
    }
}


int decompress_file(const char *inputPath, const char *outputPath) {
    int inputFd = open(inputPath, O_RDONLY);

    if (inputFd == -1) {
        perror("Opening compressed file");
        return EXIT_FAILURE;
    }

    uint64_t originalSize = 0;
    uint64_t freq64[BYTE_VALUES];

    if (read_header(inputFd, &originalSize, freq64) == -1) {
        fprintf(stderr, "Invalid or incomplete Huffman header\n");
        close(inputFd);
        return EXIT_FAILURE;
    }

    unsigned long freq[BYTE_VALUES];

    for (int i = 0; i < BYTE_VALUES; i++) {
        if (freq64[i] > ULONG_MAX) {
            fprintf(stderr, "Frequency exceeds supported range\n");
            close(inputFd);
            return EXIT_FAILURE;
        }

        freq[i] = (unsigned long)freq64[i];
    }

    HuffmanNode *root = build_huffman_tree(freq);

    if (originalSize > 0 && root == NULL) {
        fprintf(stderr, "Invalid Huffman frequency table\n");
        close(inputFd);
        return EXIT_FAILURE;
    }

    int outputFd = open(outputPath,
            O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (outputFd == -1) {
        perror("Opening decompressed output file");
        free_huffman_tree(root);
        close(inputFd);
        return EXIT_FAILURE;
    }

    uint64_t bytesWritten = 0;
    int failed = 0;

    /*
     * Special case: an empty tree represents an empty file.
     * A single leaf represents a file containing one repeated byte.
     */
    if (root != NULL &&
            root->left == NULL &&
            root->right == NULL) {

        unsigned char byte = root->character;

        while (bytesWritten < originalSize) {
            ssize_t n = write(outputFd, &byte, 1);

            if (n != 1) {
                perror("Writing decompressed file");
                failed = 1;
                break;
            }

            bytesWritten++;
        }
    } else if (root != NULL) {
        BitReader reader;
        bitreader_init(&reader, inputFd);

        HuffmanNode *current = root;

        while (bytesWritten < originalSize) {
            int bit = bitreader_read_bit(&reader);

            if (bit == -1) {
                fprintf(stderr, "Unexpected end of compressed data\n");
                failed = 1;
                break;
            }

            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }

            if (current == NULL) {
                fprintf(stderr, "Invalid compressed bitstream\n");
                failed = 1;
                break;
            }

            if (current->left == NULL && current->right == NULL) {
                unsigned char byte = current->character;

                if (write(outputFd, &byte, 1) != 1) {
                    perror("Writing decompressed file");
                    failed = 1;
                    break;
                }

                bytesWritten++;
                current = root;
            }
        }
    }

    if (close(inputFd) == -1) {
        perror("Closing compressed file");
        failed = 1;
    }

    if (close(outputFd) == -1) {
        perror("Closing decompressed file");
        failed = 1;
    }

    free_huffman_tree(root);

    if (failed) {
        return EXIT_FAILURE;
    }

    printf("Decompression completed: %s -> %s\n",
            inputPath, outputPath);
    printf("Restored size: %llu bytes\n",
            (unsigned long long)bytesWritten);

    return EXIT_SUCCESS;
}


int main(int argc, char *argv[]) {
    if (argc == 4 && strcmp(argv[1], "-d") == 0) {
        return decompress_file(argv[2], argv[3]);
    }

    if (argc != 3) {
        fprintf(stderr, "Usage:\n"
                "  %s <input_file> <output.huff>\n"
                "  %s -d <input.huff> <output_file>\n",
                argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    int inputFd = open(argv[1], O_RDONLY);

    if (inputFd == -1) {
        perror("Opening input file");
        return EXIT_FAILURE;
    }


    unsigned char buf[BUF_SIZE];
    unsigned long freq[BYTE_VALUES] = {0};
    uint64_t filesize = 0;
    ssize_t numRead;

    /* Pass 1: Count byte frequencies and original file size. */
    while ((numRead = read(inputFd, buf, sizeof(buf))) > 0) {
        for (ssize_t i = 0; i < numRead; i++) {
            freq[buf[i]]++;
        }
        filesize += (uint64_t)numRead;
    }

    if (numRead == -1) {
        perror("Reading input file");
        close(inputFd);
        return EXIT_FAILURE;
    }

    /* Convert frequencies to the fixed-width header format. */
    uint64_t freq64[BYTE_VALUES];
    for (int i = 0; i < BYTE_VALUES; i++) {
        freq64[i] = (uint64_t)freq[i];
    }

    /* Build the Huffman tree and generate codes. */
    HuffmanNode *root = build_huffman_tree(freq);

    char *codes[BYTE_VALUES] = {0};
    char code[BYTE_VALUES];

    if (root != NULL) {
        generate_codes(root, code, 0, codes);

        for (int i = 0; i < BYTE_VALUES; i++) {
            if (freq[i] > 0 && codes[i] == NULL) {
                fprintf(stderr, "Failed to generate Huffman codes\n");
                free_codes(codes);
                free_huffman_tree(root);
                close(inputFd);
                return EXIT_FAILURE;
            }
        }
    }

    int outputFd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outputFd == -1) {
        perror("Opening output file");
        free_codes(codes);
        free_huffman_tree(root);
        close(inputFd);
        return EXIT_FAILURE;
    }

    /* Write magic bytes, original size, and frequency table. */
    if (write_header(outputFd, filesize, freq64) == -1) {
        fprintf(stderr, "Failed to write compressed file header\n");
        close(inputFd);
        close(outputFd);
        free_codes(codes);
        free_huffman_tree(root);
        return EXIT_FAILURE;
    }

    /* Pass 2: Rewind input and encode its bytes. */
    if (lseek(inputFd, 0, SEEK_SET) == -1) {
        perror("Rewinding input file");
        close(inputFd);
        close(outputFd);
        free_codes(codes);
        free_huffman_tree(root);
        return EXIT_FAILURE;
    }

    BitWriter writer;
    bitwriter_init(&writer, outputFd);

    while ((numRead = read(inputFd, buf, sizeof(buf))) > 0) {
        for (ssize_t i = 0; i < numRead; i++) {
            unsigned char byte = buf[i];

            for (int j = 0; codes[byte][j] != '\0'; j++) {
                bitwriter_write_bit(&writer,
                        (unsigned char)(codes[byte][j] - '0'));
            }
        }
    }

    if (numRead == -1) {
        perror("Reading input during compression");
        close(inputFd);
        close(outputFd);
        free_codes(codes);
        free_huffman_tree(root);
        return EXIT_FAILURE;
    }


    bitwriter_flush(&writer);

    /* Get compressed size while outputFd is still open. */
    struct stat outputStat;

    if (fstat(outputFd, &outputStat) == -1) {
        perror("Getting compressed file size");
        close(inputFd);
        close(outputFd);
        free_codes(codes);
        free_huffman_tree(root);
        return EXIT_FAILURE;
    }

    uint64_t compressedSize = (uint64_t)outputStat.st_size;

    /* Close files. */
    close(inputFd);

    if (close(outputFd) == -1) {
        perror("Closing output file");
        free_codes(codes);
        free_huffman_tree(root);
        return EXIT_FAILURE;
    }

    /* Print statistics. */
    printf("Compression completed: %s -> %s\n", argv[1], argv[2]);
    printf("Original size:      %llu bytes\n",
            (unsigned long long)filesize);
    printf("Compressed size:    %llu bytes\n",
            (unsigned long long)compressedSize);

    if (filesize > 0) {
        double savedPercent =
            (1.0 - (double)compressedSize / (double)filesize) * 100.0;

        double ratio = (double)filesize / (double)compressedSize;

        printf("Space saved:        %.2f%%\n", savedPercent);
        printf("Compression ratio:  %.2f:1\n", ratio);
    } else {
        printf("Space saved:        N/A (empty file)\n");
        printf("Compression ratio:  N/A (empty file)\n");
    }

    free_codes(codes);
    free_huffman_tree(root);

    return EXIT_SUCCESS;
} 

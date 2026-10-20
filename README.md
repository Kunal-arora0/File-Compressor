# File Compressor and Decompressor

A lossless file compression and decompression utility written in C using **Huffman coding** and Linux/POSIX system calls. The project supports text and binary files through a custom compressed file format.

## Features

- Lossless compression and decompression using Huffman coding
- Byte-frequency analysis for all 256 possible byte values
- Huffman tree construction using a min-heap
- Bit-level encoding and decoding
- Custom file format with a frequency table
- POSIX file operations using `open()`, `read()`, `write()`, and `close()`
- Build automation using GNU Make

## Project Structure

```text
File-Compressor/
├── include/
│   ├── bitio.h
│   ├── heap.h
│   ├── huff_format.h
│   └── huffman.h
├── src/
│   ├── bitio.c
│   ├── heap.c
│   ├── huff_format.c
│   ├── huff_format_test.c
│   ├── huffman.c
│   └── main.c
├── .gitignore
├── LICENSE
├── Makefile
└── README.md
```

## Prerequisites

- Linux or a compatible POSIX environment
- GCC compiler
- GNU Make

On Ubuntu, install the required tools with:

```bash
sudo apt update
sudo apt install build-essential
```

## Build

Clone the repository:

```bash
git clone https://github.com/Kunal-arora0/File-Compressor.git
cd File-Compressor
```

Compile the project:

```bash
make
```

To remove the compiled executable:

```bash
make clean
```

## Usage

### Compress a file

```bash
./compressor input.txt output.huff
```

### Decompress a file

```bash
./compressor -d output.huff restored.txt
```

Replace the example filenames with your own input and output paths.

## Verify Compression and Decompression

You can verify that decompression restores the original file exactly:

```bash
./compressor input.txt output.huff
./compressor -d output.huff restored.txt
cmp input.txt restored.txt
```

If `cmp` produces no output and exits with status `0`, the files are identical.

## How It Works

1. Read the input file and count the frequency of each byte.
2. Build a Huffman tree using a min-heap.
3. Generate binary Huffman codes for the bytes present in the file.
4. Write the custom file header and encode the input as bits.
5. During decompression, read the header and reconstruct the Huffman tree.
6. Decode the bitstream to recover the original data.

## Notes

- Small files may become larger because of the header and frequency-table overhead.
- Compression effectiveness depends on the input data.
- The compressed format is specific to this implementation.

## License

See [LICENSE](LICENSE) for the license terms.


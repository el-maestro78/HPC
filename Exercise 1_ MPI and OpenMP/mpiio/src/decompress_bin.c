#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define INITIAL_BUFFER_SIZE 1024 * 1024

// Function to decompress data using zlib
void decompress_data(const unsigned char *compressed_data, size_t compressed_size, double **decompressed_data, size_t original_size) {
    size_t buffer_size = original_size * original_size;
    *decompressed_data = NULL;

    // Allocate initial buffer
    unsigned char *buffer = (unsigned char *)malloc(buffer_size);
    if (buffer == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    uLongf dest_len = buffer_size;

    // Try to decompress
    int result = uncompress(buffer, &dest_len, compressed_data, compressed_size);

    // Resize buffer if needed
    while (result == Z_BUF_ERROR) {
        buffer_size *= 2; // Double the size
        buffer = (unsigned char *)realloc(buffer, buffer_size);
        if (buffer == NULL) {
            perror("Error reallocating memory");
            exit(EXIT_FAILURE);
        }
        dest_len = buffer_size; // Reset size for decompression

        result = uncompress(buffer, &dest_len, compressed_data, compressed_size);
    }

    if (result != Z_OK) {
        perror("Decompression failed");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    *decompressed_data = (double *)buffer;
}

// Function to read compressed data from file
void read_compressed_data(const char *filename, unsigned char **compressed_data, size_t *compressed_size) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    *compressed_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *compressed_data = (unsigned char *)malloc(*compressed_size);
    if (*compressed_data == NULL) {
        perror("Error allocating memory");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(*compressed_data, 1, *compressed_size, file);
    fclose(file);
}

// Function to write decompressed data to file
void write_decompressed_data(const char *filename, const double *data, size_t data_size) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fwrite(data, sizeof(double), data_size / sizeof(double), file);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <compressed_file> <original_size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *compressed_file = argv[1];
    size_t original_size = strtoul(argv[2], NULL, 10);
    const char *output_file = "decompressed.bin";

    // Read compressed data
    unsigned char *compressed_data;
    size_t compressed_size;
    read_compressed_data(compressed_file, &compressed_data, &compressed_size);

    // Decompress data
    double *decompressed_data;
    decompress_data(compressed_data, compressed_size, &decompressed_data, original_size);

    // Write decompressed data to file
    write_decompressed_data(output_file, decompressed_data, original_size);

    // Clean up
    free(compressed_data);
    free(decompressed_data);

    printf("Decompression and writing to file completed successfully.\n");

    return 0;
}

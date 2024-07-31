#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <string.h>

// Compress data
void compress_data(const double *data, size_t data_size, unsigned char **compressed_data, size_t *compressed_size) {
    uLongf dest_len = compressBound(data_size);
    *compressed_data = (unsigned char *)malloc(dest_len);

    if (*compressed_data == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    if (compress(*compressed_data, &dest_len, (const Bytef *)data, data_size) != Z_OK) {
        perror("Compression failed");
        free(*compressed_data);
        exit(EXIT_FAILURE);
    }

    *compressed_size = dest_len;
}

// Decompress data
void decompress_data(const unsigned char *compressed_data, size_t compressed_size, double **decompressed_data, size_t original_size) {
    *decompressed_data = (double *)malloc(original_size);
    if (*decompressed_data == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    uLongf dest_len = original_size;
    if (uncompress((Bytef *)*decompressed_data, &dest_len, compressed_data, compressed_size) != Z_OK) {
        perror("Decompression failed");
        free(*decompressed_data);
        exit(EXIT_FAILURE);
    }
}

int main() {
    const char *data_file = "density_mpi.bin"; // Replace with your file names
    const char *compressed_file = "density_comp.bin";
    const char *decompressed_file = "decompressed.bin";

    // Read original data
    FILE *file = fopen(data_file, "rb");
    if (!file) {
        perror("Error opening data file");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    size_t data_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    double *original_data = (double *)malloc(data_size);
    if (!original_data) {
        perror("Memory allocation failed");
        fclose(file);
        return EXIT_FAILURE;
    }

    fread(original_data, 1, data_size, file);
    fclose(file);

    // Compress data
    unsigned char *compressed_data;
    size_t compressed_size;
    compress_data(original_data, data_size, &compressed_data, &compressed_size);

    // Write compressed data
    file = fopen(compressed_file, "wb");
    if (!file) {
        perror("Error opening compressed file");
        free(original_data);
        free(compressed_data);
        return EXIT_FAILURE;
    }
    fwrite(compressed_data, 1, compressed_size, file);
    fclose(file);
    free(compressed_data);

    // Read compressed data
    file = fopen(compressed_file, "rb");
    if (!file) {
        perror("Error opening compressed file");
        free(original_data);
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    compressed_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    compressed_data = (unsigned char *)malloc(compressed_size);
    if (!compressed_data) {
        perror("Memory allocation failed");
        fclose(file);
        free(original_data);
        return EXIT_FAILURE;
    }

    fread(compressed_data, 1, compressed_size, file);
    fclose(file);

    // Decompress data
    double *decompressed_data;
    decompress_data(compressed_data, compressed_size, &decompressed_data, data_size);
    free(compressed_data);

    // Write decompressed data
    file = fopen(decompressed_file, "wb");
    if (!file) {
        perror("Error opening decompressed file");
        free(original_data);
        free(decompressed_data);
        return EXIT_FAILURE;
    }
    fwrite(decompressed_data, 1, data_size, file);
    fclose(file);

    // Compare data
    if (memcmp(original_data, decompressed_data, data_size) == 0) {
        printf("Data verification successful\n");
    } else {
        printf("Data verification failed\n");
    }

    // Clean up
    free(original_data);
    free(decompressed_data);

    return EXIT_SUCCESS;
}

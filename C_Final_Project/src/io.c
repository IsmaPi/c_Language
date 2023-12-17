#include "include/io.h"
#include <stdlib.h>
#include <stdio.h>

char* get_file_contents(const char* filepath) {
    char* buffer = NULL;
    long length;
    FILE* f = fopen(filepath, "rb");

    if (f) {
        // Get the file length
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        // Allocate memory for the buffer plus null terminator
        buffer = calloc(length + 1, 1); // Using 1 as the size to ensure the buffer is initialized to zeros

        if (buffer) {
            // Read the file into the buffer
            if (fread(buffer, 1, length, f) != length) {
                // Handle read error
                free(buffer);
                fclose(f);
                fprintf(stderr, "Error reading file %s\n", filepath);
                exit(EXIT_FAILURE);
            }

            buffer[length] = '\0'; // Ensure null-terminated string
        } else {
            // Handle memory allocation error
            fclose(f);
            fprintf(stderr, "Failed to allocate memory for reading file %s\n", filepath);
            exit(EXIT_FAILURE);
        }

        fclose(f);
        return buffer;
    } else {
        // Handle file opening error
        fprintf(stderr, "Unable to open file %s\n", filepath);
        exit(EXIT_FAILURE);
    }
}
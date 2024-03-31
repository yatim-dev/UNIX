#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>
#include <getopt.h>


int main(int argc, char **argv) {
    int exit_code = 0;
    int written_bytes = 0;
    int read_bytes = 0;
    int buffer_size = 4096;

    int fd_to_read;
    int fd_to_write;
    bool is_zero_block;
    char * restrict buffer;

    static struct option options[] = {
        {"block-size", required_argument, NULL, 'b'},
        {0, 0, 0, 0},
    };
    char *optstring = "b:";

    while (true) {
        if (getopt(argc, argv, optstring) == -1)
            break;
        
        buffer_size = atoi(optarg);
        if (buffer_size <= 0) {
            fprintf(stderr, "ERROR: buffer's size must be a positive integer.\n");
            return exit_code;
        }
    }

    if (optind + 1 == argc) {
        fd_to_write = open(argv[optind], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd_to_write == -1) {
            fprintf(stderr, "ERROR: failed to open file for writting.\n");
            return exit_code;
        }
    }
    else if (optind + 2 == argc) {
        if (strcmp(argv[optind + 1], argv[optind]) == 0) {
            fprintf(stderr, "ERROR: file for writing and file for reading cannot be the same.\n");
            return exit_code;            
        }

        fd_to_write = open(argv[optind + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd_to_write == -1) {
            fprintf(stderr, "ERROR: failed to open file for writting.\n");
            return exit_code;
        }
        fd_to_read = open(argv[optind], O_RDONLY);
        if (fd_to_read == -1) {
            close(fd_to_write);
        }
    }
    else {
        fprintf(stderr, "ERROR: wrong arguments count.\n");
        return exit_code;
    }

    buffer = calloc(buffer_size, sizeof(char));
    if (buffer == NULL) {
        close(fd_to_read);
    }

    while (true) {
        is_zero_block = true;
        read_bytes = read(fd_to_read, buffer, buffer_size);
        if (read_bytes == -1) {
            free(buffer);
        }
        if (read_bytes == 0)
            break;

        for (int i = 0; i < read_bytes; i++) {
            if (buffer[i] != 0) {
                is_zero_block = false;
                break;
            }
        }

        if (is_zero_block) {
            if (lseek(fd_to_write, read_bytes, SEEK_CUR) == -1) {
                free(buffer);
            }
        }
        else {
            written_bytes = write(fd_to_write, buffer, read_bytes);
            if (written_bytes != read_bytes) {
                free(buffer);
            }
        }
    }
}
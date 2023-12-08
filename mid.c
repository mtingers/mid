#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void usage(const char *name) {
    printf("%s [-o] [-n number-of-lines] [-] [file1] [file2...]\n", name);
    printf("    -o      print offset (e.g. '@@ line,offset @@')\n");
    printf("    -n N    print N lines surrounding midpoint\n");
    printf("    -       read from stdin\n");
    printf("\nexamples:\n");
    printf("    %s file1.txt\n", name);
    printf("    %s -n 20 file1.txt file2.txt\n", name);
}

FILE *file_open(const char *path, const char *mode) {
    FILE *f = fopen(path, mode);
    if (f == NULL) {
        perror(path);
        return NULL;
    }
    return f;
}

FILE *open_tmp_file(char *path) {
    mkstemp(path);
    FILE *fd = file_open(path, "w+");
    return fd;
}
void mid_line_mode(int print_seperator, size_t number_of_lines,
                   const char *file_path, int include_offset) {
    FILE *fd = file_path != NULL ? file_open(file_path, "r") : stdin;
    char tmp_name_buf[] = "/tmp/midXXXXX";
    // int tmp_status =
    FILE *fd_tmp = file_path == NULL ? open_tmp_file(tmp_name_buf) : NULL;
    size_t pos = 0;
    char c = 0;
    size_t line_count = 0;
    size_t line_start = 0;
    size_t cur_line = 0;

    if (!fd) {
        goto error;
    }

    while ((c = fgetc(fd)) != EOF) {
        if (fd_tmp) {
            fputc(c, fd_tmp);
        }
        if (c == '\n') {
            line_count++;
        }
    }
    if (fd_tmp) {
        fd = fd_tmp;
    }
    rewind(fd);
    line_start = (line_count / 2) - (number_of_lines / 2);
    while ((c = fgetc(fd)) != EOF) {
        if (c == '\n') {
            cur_line++;
            if (cur_line == line_start) {
                break;
            }
        }
    }
#ifdef DEBUG
    fprintf(stderr, "DEBUG: file_path=%s line_count=%lu line_start=%lu\n",
            file_path, line_count, line_start);
#endif
    if (print_seperator > 0) {
        printf("==> %s <==\n", file_path != NULL ? file_path : "stdin");
    }
    pos = ftell(fd);
    if (include_offset > 0) {
        printf("@@ %lu,%lu @@\n", cur_line, pos);
    }
    line_count = 0;
    while ((c = fgetc(fd)) != EOF) {
        if (c == '\n') {
            line_count++;
            if (line_count >= number_of_lines) {
                printf("\n");
                break;
            }
        }
        printf("%c", c);
    }
    goto cleanup;

error:
    perror("error");

cleanup:
    fflush(stderr);
    fflush(stdout);
    if (fd) {
        fclose(fd);
    }
    if (fd_tmp) {
        fclose(fd_tmp);
        unlink(tmp_name_buf);
    }
}

int main(int argc, char **argv) {
    int number_of_lines = 10;
    int file_count = 0;
    int include_offset = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && strncmp("-n", argv[i], 2) == 0) {
            i++;
            if (i > argc) {
                break;
            }
            number_of_lines = atoi(argv[i]);
        } else if (argv[i][0] == '-' && strncmp("-o", argv[i], 2) == 0) {
            include_offset = 1;
        } else if (argv[i][0] == '-' && (strncmp("-h", argv[i], 2) == 0 ||
                                         strncmp("-H", argv[i], 2) == 0 ||
                                         strncmp("--help", argv[1], 6) == 0)) {

            usage(argv[0]);
            return 1;
        } else {
            file_count++;
        }
    }
    if (file_count < 1) {
        mid_line_mode(file_count - 1, number_of_lines, NULL, include_offset);
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && strncmp("-n", argv[i], 2) == 0) {
            i++;
        } else if (argv[i][0] == '-' && strncmp("-o", argv[i], 2) == 0) {
            // pass, no optarg
        } else {
            if (argv[i][0] == '-' && argv[i][1] == '\0') {
                mid_line_mode(file_count - 1, number_of_lines, NULL,
                              include_offset);
            } else {
                mid_line_mode(file_count - 1, number_of_lines, argv[i],
                              include_offset);
            }
        }
    }
    return 0;
}

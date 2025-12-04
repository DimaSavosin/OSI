#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    FILE *src, *dst;
    int ch;
    struct stat st;


    if (argc < 3) {
    printf("Error: not enough arguments.\n");
    return 1;

   }
    src = fopen(argv[1], "r");
    if (src == NULL) {
        perror("Error opening source file");
        return 1;
    }

    dst = fopen(argv[2], "w");
    if (dst == NULL) {
        perror("Error opening destination file");
        fclose(src);
        return 1;
    }

    if (stat(argv[1], &st) == 0) {
        chmod(argv[2], st.st_mode);
    }

    while ((ch = fgetc(src)) != EOF) {
        if (fputc(ch, dst) == EOF) {
            perror("Error writing to destination file");
            fclose(src);
            fclose(dst);
            return 1;
        }
    }

    if (ferror(src)) {
        perror("Error reading source file");
        fclose(src);
        fclose(dst);
        return 1;
    }

    if (fclose(src) == EOF) {
        perror("Error closing source file");
        return 1;
    }

    if (fclose(dst) == EOF) {
        perror("Error closing destination file");
        return 1;
    }

    return 0;
}


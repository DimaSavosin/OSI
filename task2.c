#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int ch;

    if (argc < 2) {
        fprintf(stderr, "Error: please specify a file name.\n");
        return 1;
    }

    fp = fopen(argv[1], "w");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    printf("Enter text. Press Ctrl-F to exit.\n");

    while ((ch = fgetc(stdin)) != EOF) {
        if (ch == 6) {
            printf("\nCtrl-F detected. Exiting input.\n");
            break;
        }

        if (fputc(ch, fp) == EOF) {
            if (fclose(fp) == EOF) {
                perror("Error closing file");
            }
            return 1;
        }
    }

    if (ferror(stdin)) {
        perror("Error reading from stdin");
        if (fclose(fp) == EOF) {
            perror("Error closing file");
        }
        return 1;
    }

    if (fclose(fp) == EOF) {
        perror("Error closing file");
        return 1;
    }

    printf("Data successfully written to file '%s'.\n", argv[1]);
    return 0;
}

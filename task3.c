#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int ch;
    int current_lines = 0;
    
    if (argc < 3) { 
        printf("Error: not enough arguments.\n");
        return 1;
    }   
    int lines_per_page = atoi(argv[2]);

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    while ((ch = fgetc(fp)) != EOF) {

        if (fputc(ch, stdout) == EOF) {
            perror("Error writing to stdout");
            fclose(fp);
            return 1;
        }

        if (lines_per_page > 0 && ch == '\n') {
            current_lines++;

            if (current_lines >= lines_per_page) {
                getc(stdin);
                current_lines = 0;
            }
        }
    }


    if (ferror(fp)) {
        perror("Error reading file");
        fclose(fp);
        return 1;
    }


    if (fclose(fp) == EOF) {
        perror("Error closing file");
        return 1;
    }

    return 0;
}

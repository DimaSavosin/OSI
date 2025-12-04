#include <stdio.h>
#include <dirent.h>
#include <errno.h>

void print_directory(const char *path)
{
    DIR *dp;
    struct dirent *entry;

    printf("Directory: %s\n", path);

    dp = opendir(path);
    if (dp == NULL) {
        perror("Error opening directory");
        return;
    }

    errno = 0;
    while ((entry = readdir(dp)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    if (errno != 0) {
        perror("Error reading directory");

    }

    if (closedir(dp) == -1) {
        perror("Error closing directory");
    }

    printf("\n"); 
}

int main(int argc, char *argv[])
{
   
    print_directory(".");

    if (argc >= 2) {
        print_directory(argv[1]);
    }

    return 0;
}

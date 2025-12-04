#include <stdio.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    const char *path;
    DIR *dp;
    struct dirent *entry;


    if (argc < 2) {
        path = ".";
    } else {
        path = argv[1];
    }

    printf("Directory: %s\n", path);

    dp = opendir(path);
    if (dp == NULL) {
        perror("Error opening directory");
        return 1;
    }

    errno = 0;
    while ((entry = readdir(dp)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    if (errno != 0) {
        perror("Error reading directory");
        closedir(dp);
        return 1;
    }

    if (closedir(dp) == -1) {
        perror("Error closing directory");
        return 1;
    }

    return 0;
}

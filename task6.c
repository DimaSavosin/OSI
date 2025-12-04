#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <time.h>

void process_directory(const char *dirpath,
                       FILE *out,
                       long N1, long N2,
                       time_t M1, time_t M2)
{
    DIR *dp;
    struct dirent *entry;

    dp = opendir(dirpath);
    if (dp == NULL) {
        perror("Error opening directory");
        return;
    }

    errno = 0;
    while ((entry = readdir(dp)) != NULL) {
        char fullpath[PATH_MAX];
        struct stat st;

        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }


        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, entry->d_name);

        if (stat(fullpath, &st) == -1) {
            perror("Error getting file info (stat)");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {

            process_directory(fullpath, out, N1, N2, M1, M2);
        } else if (S_ISREG(st.st_mode)) {
            if (st.st_size >= N1 && st.st_size <= N2 &&
                st.st_mtime >= M1 && st.st_mtime <= M2) {

                char timebuf[64];
                struct tm *tm_info = localtime(&st.st_mtime);
                if (tm_info != NULL) {
                    strftime(timebuf, sizeof(timebuf),
                             "%Y-%m-%d %H:%M:%S", tm_info);
                } else {
                    snprintf(timebuf, sizeof(timebuf), "N/A");
                }


                if (printf("File: %s | Size: %ld bytes | Time: %s\n",
                           fullpath, (long)st.st_size, timebuf) < 0) {
                    perror("Error writing to stdout");
                }


                if (fprintf(out, "File: %s | Size: %ld bytes | Time: %s\n",
                            fullpath, (long)st.st_size, timebuf) < 0) {
                    perror("Error writing to output file");
                }
            }
        }
    }

    if (errno != 0) {
        perror("Error reading directory");
    }

    if (closedir(dp) == -1) {
        perror("Error closing directory");
    }
}

int main(int argc, char *argv[])
{
    const char *start_dir;
    const char *out_filename;
    long N1, N2;
    time_t M1, M2;
    FILE *out;

    if (argc != 7) {
        fprintf(stderr,
                "Usage: %s <start_dir> <output_file> <N1> <N2> <M1> <M2>\n"
                "  N1, N2 - file size bounds in bytes.\n"
                "  M1, M2 - time bounds (seconds since 1970-01-01).\n",
                argv[0]);
        return 1;
    }

    start_dir = argv[1];
    out_filename = argv[2];
    N1 = atol(argv[3]);
    N2 = atol(argv[4]);
    M1 = (time_t)atol(argv[5]);
    M2 = (time_t)atol(argv[6]);


    out = fopen(out_filename, "w");
    if (out == NULL) {
        perror("Error opening output file");
        return 1;
    }

    process_directory(start_dir, out, N1, N2, M1, M2);


    if (fclose(out) == EOF) {
        perror("Error closing output file");
        return 1;
    }

    return 0;
}


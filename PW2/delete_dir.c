#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void delete_contents(const char *dir_path) {
    struct dirent *entry;
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    char path[1024];
    struct stat statbuf;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (stat(path, &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {
                delete_contents(path);
                rmdir(path);
            } else {
                unlink(path);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    delete_contents(argv[1]);
    printf("Contents of %s deleted successfully.\n", argv[1]);

    return 0;
}

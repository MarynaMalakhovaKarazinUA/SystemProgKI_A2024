#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }

    const char *dir_path = argv[1];
    struct stat statbuf;

    // Перевіряємо, чи існує каталог
    if (stat(dir_path, &statbuf) == 0) {
        if (S_ISDIR(statbuf.st_mode)) {
            printf("(1) - Directory '%s' exists. (SUCCESS)\n", dir_path);
        } else {
            printf("(2) - Path '%s' exists, but it is not a directory.\n", dir_path);
        }
    } else {
        printf("(3) - Directory '%s' does not exist. Error: %s\n", dir_path, strerror(errno));
    }

    return 0;
}

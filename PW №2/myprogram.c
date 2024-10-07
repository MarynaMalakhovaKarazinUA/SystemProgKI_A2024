#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

int main() {
    char template[] = "/tmp/tempfileXXXXXX";
    int fd = mkstemp(template);

    if (fd == -1) {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }

    // Заповнюємо файл даними
    const char *data = "Це тимчасовий файл.\n";
    if (write(fd, data, strlen(data)) == -1) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Робимо файл доступним для читання для всіх
    if (chmod(template, S_IRUSR | S_IRGRP | S_IROTH) == -1) {
        perror("chmod");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Закриваємо файл
    close(fd);

    // Робимо файл постійним (перейменовуємо його)
    const char *new_filename = "/tmp/permanentfile.txt";
    if (rename(template, new_filename) == -1) {
        perror("rename");
        exit(EXIT_FAILURE);
    }

    printf("Файл створено та збережено як %s\n", new_filename);
    return 0;
}


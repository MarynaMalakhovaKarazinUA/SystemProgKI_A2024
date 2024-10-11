#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    const char *path = "/app/new_directory";  // Назва каталогу
    int status = mkdir(path, 0755);  // Створення каталогу з правами доступу

    if (status == 0) {
        printf("Каталог успішно створено.\n");
    } else {
        perror("Помилка при створенні каталогу");
    }

    return 0;
}

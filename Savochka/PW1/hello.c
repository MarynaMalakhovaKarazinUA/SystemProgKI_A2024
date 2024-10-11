#include <stdio.h>
#include <unistd.h> // Для системного вызова rename()
#include <errno.h>  // Для обработки ошибок

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Використання: %s <каталог_джерело> <каталог_призначення>\n", argv[0]);
        return 1;
    }

    // Переміщення каталогу за допомогою системного виклику rename()
    if (rename(argv[1], argv[2]) != 0) {
        perror("Помилка переміщення каталогу");
        return 1;
    }

    printf("Каталог успішно переміщено з %s до %s\n", argv[1], argv[2]);
    return 0;
}
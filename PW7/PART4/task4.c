#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>
#include <sys/inotify.h>

#define WATCHED_DIR "./documents" // Папка для моніторингу
#define BACKUP_DIR "./backup"  // Папка для зберігання резервних копій
#define LOG_FILE "./backup_log.txt"      // Лог-файл

// Функція для запису в журнал
void log_event(const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Не вдалося відкрити лог-файл");
        return;
    }
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';  // Видалити символ нового рядка в кінці
    fprintf(log_file, "[%s] %s\n", time_str, message);
    fclose(log_file);
}

// Функція для створення резервної копії файлу
void backup_file(const char *filename) {
    char backup_filename[1024];
    snprintf(backup_filename, sizeof(backup_filename), "%s/%s.bak", BACKUP_DIR, filename);

    if (access(filename, F_OK) == -1) {
        log_event("Файл не знайдений, не можна зробити резервну копію.");
        return;
    }

    if (rename(filename, backup_filename) == -1) {
        perror("Не вдалося зробити резервну копію");
        return;
    }

    char log_msg[1024];
    snprintf(log_msg, sizeof(log_msg), "Створено резервну копію для %s", filename);
    log_event(log_msg);
}

// Функція для демонізації
void daemonize() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Не вдалося виконати fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);  // Батьківський процес завершується
    }

    if (setsid() < 0) {
        perror("Не вдалося створити сесію");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("Не вдалося виконати fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);  // Батьківський процес завершується
    }

    chdir("/");  // Зміна робочої директорії
    umask(0);    // Зміна маски файлів
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    open("/dev/null", O_RDWR);  // Стандартний вхід
    dup(0);                      // Стандартний вивід
    dup(0);                      // Стандартна помилка
}

// Головна функція
int main() {
    daemonize();  // Перетворення в демон

    // Ініціалізація inotify для моніторингу змін у папці
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("Не вдалося ініціалізувати inotify");
        exit(EXIT_FAILURE);
    }

    // Додаємо спостереження за папкою
    int wd = inotify_add_watch(inotify_fd, WATCHED_DIR, IN_MODIFY | IN_CREATE | IN_DELETE);
    if (wd == -1) {
        perror("Не вдалося додати спостереження за папкою");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    while (1) {
        int length = read(inotify_fd, buffer, sizeof(buffer));
        if (length == -1) {
            perror("Помилка читання з inotify");
            exit(EXIT_FAILURE);
        }

        // Обробка подій
        for (int i = 0; i < length; i += sizeof(struct inotify_event)) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE || event->mask & IN_MODIFY) {
                    // Якщо файл створено або змінено, робимо резервну копію
                    backup_file(event->name);
                }
            }
        }
    }

    close(inotify_fd);
    return 0;
}
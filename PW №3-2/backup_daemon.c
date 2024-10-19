#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <syslog.h>  // Добавлено для работы с syslog

#define SOURCE_DIR "/path/to/source_folder"
#define BACKUP_DIR "/path/to/backup_folder"
#define MAX_BACKUPS 5
#define LOG_FILE "/var/log/backup_daemon.log"

// Функция для демонстрации демонизации процесса
void daemonize() {
    pid_t pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    chdir("/");

    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }

    openlog("backup_daemon", LOG_PID, LOG_DAEMON);  // Работа с системным журналом
}

// Функция для записи событий в журнал
void log_event(const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        return;
    }

    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0'; // Удаляем символ новой строки

    fprintf(log_file, "[%s] %s\n", time_str, message);
    fclose(log_file);
}

// Функция для создания резервной копии файла
void create_backup(const char *file_path) {
    char backup_path[1024];
    snprintf(backup_path, sizeof(backup_path), "%s/%s", BACKUP_DIR, file_path + strlen(SOURCE_DIR) + 1);

    char backup_dir[1024];
    strncpy(backup_dir, backup_path, strrchr(backup_path, '/') - backup_path);
    backup_dir[strrchr(backup_path, '/') - backup_path] = '\0';

    mkdir(backup_dir, 0755);

    int src_fd = open(file_path, O_RDONLY);
    int dest_fd = open(backup_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    char buffer[4096];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        write(dest_fd, buffer, bytes_read);
    }

    close(src_fd);
    close(dest_fd);

    log_event("Backup created");
}

// Функция для управления резервными копиями
void manage_backups() {
    DIR *dir = opendir(BACKUP_DIR);
    if (dir == NULL) {
        return;
    }

    struct dirent *entry;
    int backup_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            backup_count++;
        }
    }
    closedir(dir);

    if (backup_count > MAX_BACKUPS) {
        // Удаление старейших резервных копий
        // (Этот код можно расширить для удаления старейших файлов)
    }
}

// Функция для мониторинга папки
void monitor_folder() {
    while (1) {
        DIR *dir = opendir(SOURCE_DIR);
        if (dir == NULL) {
            sleep(10);
            continue;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                char file_path[1024];
                snprintf(file_path, sizeof(file_path), "%s/%s", SOURCE_DIR, entry->d_name);
                create_backup(file_path);
            }
        }
        closedir(dir);

        manage_backups();
        sleep(10);
    }
}

int main() {
    daemonize();
    log_event("Daemon started");
    monitor_folder();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))
#define MAX_BACKUPS 5 // Максимальна кількість резервних копій

const char *backup_folder = "C:\\Test\\part1\\path\\to\\backup"; // Резервна папка
const char *watch_folder = "C:\\Test\\part1\\path\\to\\watch";   // Папка для моніторингу
const char *log_file = "C:\\Test\\part1\\path\\to\\log_file.log"; // Журнал подій

void log_message(const char *message) {
    FILE *log_fp = fopen(log_file, "a");
    if (log_fp) {
        time_t now = time(NULL);
        fprintf(log_fp, "[%s] %s\n", ctime(&now), message);
        fclose(log_fp);
    }
}

void create_backup(const char *file) {
    char src_path[1024], dest_path[1024];
    snprintf(src_path, sizeof(src_path), "%s/%s", watch_folder, file);
    snprintf(dest_path, sizeof(dest_path), "%s/%s", backup_folder, file);

    FILE *src_fp = fopen(src_path, "rb");
    if (!src_fp) {
        log_message("Failed to open source file for backup.");
        return;
    }

    FILE *dest_fp = fopen(dest_path, "wb");
    if (!dest_fp) {
        fclose(src_fp);
        log_message("Failed to create backup file.");
        return;
    }

    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src_fp)) > 0) {
        fwrite(buffer, 1, bytes, dest_fp);
    }

    fclose(src_fp);
    fclose(dest_fp);

    char log_buf[1024];
    snprintf(log_buf, sizeof(log_buf), "Backup created for file: %s", file);
    log_message(log_buf);
}

void manage_backups() {
    DIR *dir = opendir(backup_folder);
    if (!dir) {
        log_message("Failed to open backup folder for cleanup.");
        return;
    }

    struct dirent *entry;
    char *files[MAX_BACKUPS + 10]; // Тимчасове сховище для файлів
    int file_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            files[file_count++] = strdup(entry->d_name);
        }
    }
    closedir(dir);

    if (file_count > MAX_BACKUPS) {
        for (int i = 0; i < file_count - MAX_BACKUPS; ++i) {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", backup_folder, files[i]);
            remove(path);

            char log_buf[1024];
            snprintf(log_buf, sizeof(log_buf), "Old backup removed: %s", files[i]);
            log_message(log_buf);

            free(files[i]);
        }
    }
    for (int i = file_count - MAX_BACKUPS; i < file_count; ++i) {
        free(files[i]);
    }
}

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    if (chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main() {
    daemonize();
    log_message("Backup daemon started.");

    int fd = inotify_init();
    if (fd < 0) {
        log_message("Failed to initialize inotify.");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(fd, watch_folder, IN_CREATE | IN_MODIFY);
    if (wd < 0) {
        log_message("Failed to add watch.");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char buffer[EVENT_BUF_LEN];
    while (1) {
        int length = read(fd, buffer, EVENT_BUF_LEN);
        if (length < 0) {
            log_message("Failed to read inotify events.");
            break;
        }

        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            if (event->len > 0) {
                if (event->mask & (IN_CREATE | IN_MODIFY)) {
                    create_backup(event->name);
                    manage_backups();
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
    log_message("Backup daemon stopped.");
    return 0;
}

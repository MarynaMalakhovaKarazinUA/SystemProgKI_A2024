#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>

#define WATCH_DIR "/app/backup_test/watch"
#define BACKUP_DIR "/app/backup_test/backup"

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    setsid();

    // Змінити робочий каталог
    chdir("/");

    // Закрити стандартні дескриптори
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    // Відкрити журнал
    openlog("BackupDaemon", LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

void log_event(const char *event) {
    syslog(LOG_NOTICE, "%s", event);
}

void backup_file(const char *file_name) {
    char source[512], destination[512];
    snprintf(source, sizeof(source), "%s/%s", WATCH_DIR, file_name);
    snprintf(destination, sizeof(destination), "%s/%s", BACKUP_DIR, file_name);
    rename(source, destination);
    log_event("Backup created for file.");
}

void monitor_directory() {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(WATCH_DIR);
    if (!dir) {
        perror("Failed to open watch directory");
        return;
    }

    while (1) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) { // Тільки файли
                backup_file(entry->d_name);
            }
        }
        rewinddir(dir);
        sleep(10); // Затримка між перевірками
    }

    closedir(dir);
}

int main() {
    daemonize();
    monitor_directory();
    return 0;
}

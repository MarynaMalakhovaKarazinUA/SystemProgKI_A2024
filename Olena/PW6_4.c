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
#include <errno.h>

#define WATCH_DIR "/app/backup_test/watch"
#define BACKUP_DIR "/app/backup_test/backup"
#define BACKUP_LIMIT 5

void write_log(const char *event) {
    syslog(LOG_NOTICE, "%s", event);
}

void copy_file(const char *src, const char *dest) {
    FILE *source = fopen(src, "rb");
    if (!source) {
        syslog(LOG_ERR, "Failed to open source file: %s", src);
        return;
    }

    FILE *destination = fopen(dest, "wb");
    if (!destination) {
        syslog(LOG_ERR, "Failed to open destination file: %s", dest);
        fclose(source);
        return;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytes, destination);
    }

    fclose(source);
    fclose(destination);
    write_log("Backup created.");
}

void manage_backups() {
    struct dirent **namelist;
    int n = scandir(BACKUP_DIR, &namelist, NULL, alphasort);

    if (n < 0) {
        syslog(LOG_ERR, "Failed to scan backup folder.");
        return;
    }

    int file_count = 0;
    for (int i = 0; i < n; i++) {
        if (namelist[i]->d_name[0] != '.') file_count++;
    }

    if (file_count > BACKUP_LIMIT) {
        for (int i = 0; i < file_count - BACKUP_LIMIT; i++) {
            if (namelist[i]->d_name[0] != '.') {
                char path[1024];
                snprintf(path, sizeof(path), "%s/%s", BACKUP_DIR, namelist[i]->d_name);
                if (unlink(path) == 0) {
                    syslog(LOG_NOTICE, "Old backup deleted: %s", path);
                } else {
                    syslog(LOG_ERR, "Failed to delete old backup: %s", path);
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        free(namelist[i]);
    }
    free(namelist);
}

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    if (setsid() < 0) exit(EXIT_FAILURE);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }
}

int main() {
    daemonize(); // Перевести процес у фоновий режим

    write_log("Backup daemon started.");

    while (1) {
        DIR *dir = opendir(WATCH_DIR);
        if (!dir) {
            syslog(LOG_ERR, "Failed to open watch directory: %s", WATCH_DIR);
            return 1;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                char src[512], dest[512];
                snprintf(src, sizeof(src), "%s/%s", WATCH_DIR, entry->d_name);
                snprintf(dest, sizeof(dest), "%s/%s", BACKUP_DIR, entry->d_name);
                copy_file(src, dest);
            }
        }
        closedir(dir);

        manage_backups(); // Керувати кількістю резервних копій

        sleep(10); // Затримка між перевірками
    }

    return 0;
}
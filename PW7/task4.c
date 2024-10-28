#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

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
    umask(0);
    chdir("/");

    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }

    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
}

void backup_file(const char *src, const char *dest) {
    char command[256];
    snprintf(command, sizeof(command), "cp -u %s %s", src, dest);
    system(command);
}

int main() {
    daemonize();
    
    const char *watch_dir = "/watch";  
    const char *backup_dir = "/backup"; 

    // Створюємо директорії, якщо вони не існують
    mkdir(watch_dir, 0777);
    mkdir(backup_dir, 0777);

    int fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(fd, watch_dir, IN_MODIFY | IN_CREATE);
    if (wd == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_LEN];
    printf("Monitoring directory: %s\n", watch_dir);
    
    while (1) {
        int length = read(fd, buffer, BUF_LEN);
        if (length < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    printf("File created: %s\n", event->name);
                    char src[256], dest[256];
                    snprintf(src, sizeof(src), "%s/%s", watch_dir, event->name);
                    snprintf(dest, sizeof(dest), "%s/%s", backup_dir, event->name);
                    backup_file(src, dest);
                    printf("Backup created for: %s\n", event->name);
                } else if (event->mask & IN_MODIFY) {
                    printf("File modified: %s\n", event->name);
                    char src[256], dest[256];
                    snprintf(src, sizeof(src), "%s/%s", watch_dir, event->name);
                    snprintf(dest, sizeof(dest), "%s/%s", backup_dir, event->name);
                    backup_file(src, dest);
                    printf("Backup updated for: %s\n", event->name);
                }
            }
            i += EVENT_SIZE + event->len;
        }
        sleep(1); // Затримка між перевірками
    }

    inotify_rm_watch(fd, wd);
    close(fd);
    return 0;
}

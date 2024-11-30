#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void set_env(const char *var, const char *value) {
    if (setenv(var, value, 1) != 0) {
        perror("setenv");
    }
}

void get_env(const char *var) {
    const char *value = getenv(var);
    if (value) {
        printf("%s=%s\n", var, value);
    } else {
        printf("%s is not set\n", var);
    }
}

void print_pid() {
    printf("PID: %d\n", getpid());
}

void print_ppid() {
    printf("PPID: %d\n", getppid());
}

void print_addresses() {
    printf("TEXT: %p\n", (void *)print_addresses);
    printf("DATA: %p\n", (void *)&set_env);
    printf("BSS: %p\n", (void *)&get_env);
}

void print_uid() {
    printf("UID: %d\n", getuid());
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [options]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--set") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s -s VAR=VALUE\n", argv[0]);
            return 1;
        }
        char *var = strtok(argv[2], "=");
        char *value = strtok(NULL, "=");
        if (var && value) {
            set_env(var, value);
        } else {
            fprintf(stderr, "Invalid format. Use VAR=VALUE\n");
        }
    } else if (strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "--get") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s -g VAR\n", argv[0]);
            return 1;
        }
        get_env(argv[2]);
    } else if (strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "--pid") == 0) {
        print_pid();
    } else if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--ppid") == 0) {
        print_ppid();
    } else if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--addresses") == 0) {
        print_addresses();
    } else if (strcmp(argv[1], "-u") == 0 || strcmp(argv[1], "--uid") == 0) {
        print_uid();
    } else {
        fprintf(stderr, "Unknown option: %s\n", argv[1]);
        return 1;
    }

    return 0;
}

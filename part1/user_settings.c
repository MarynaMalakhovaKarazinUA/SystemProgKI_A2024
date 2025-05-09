#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

// Функція для встановлення змінної оточення
void set_env_variable(const char *variable) {
    char *equal_sign = strchr(variable, '=');
    if (equal_sign == NULL) {
        fprintf(stderr, "Invalid format. Use NAME=VALUE.\n");
        exit(EXIT_FAILURE);
    }

    char name[256];
    strncpy(name, variable, equal_sign - variable);
    name[equal_sign - variable] = '\0';

    const char *value = equal_sign + 1;
    if (setenv(name, value, 1) == 0) {
        printf("Environment variable set: %s=%s\n", name, value);
    } else {
        perror("Failed to set environment variable");
    }
}

// Функція для отримання змінної оточення
void get_env_variable(const char *name) {
    const char *value = getenv(name);
    if (value) {
        printf("%s=%s\n", name, value);
    } else {
        printf("Environment variable '%s' is not set.\n", name);
    }
}

void print_pid() {
    printf("PID: %d\n", getpid());
}

void print_ppid() {
    printf("PPID: %d\n", getppid());
}

void print_memory_addresses() {
    printf("Memory segment addresses are platform-dependent and not implemented.\n");
}

void print_uid() {
    printf("Real User ID: %d\n", getuid());
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [OPTIONS]\n", argv[0]);
        printf("Options:\n");
        printf("  -s NAME=VALUE    Set an environment variable\n");
        printf("  -g NAME          Get the value of an environment variable\n");
        printf("  -p               Print the PID of the current process\n");
        printf("  -r               Print the PPID of the current process\n");
        printf("  -a               Print memory segment addresses\n");
        printf("  -u               Print the real user ID\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            set_env_variable(argv[++i]);
        } else if (strcmp(argv[i], "-g") == 0 && i + 1 < argc) {
            get_env_variable(argv[++i]);
        } else if (strcmp(argv[i], "-p") == 0) {
            print_pid();
        } else if (strcmp(argv[i], "-r") == 0) {
            print_ppid();
        } else if (strcmp(argv[i], "-a") == 0) {
            print_memory_addresses();
        } else if (strcmp(argv[i], "-u") == 0) {
            print_uid();
        } else {
            fprintf(stderr, "Unknown option or missing argument: %s\n", argv[i]);
            return 1;
        }
    }

    return 0;
}

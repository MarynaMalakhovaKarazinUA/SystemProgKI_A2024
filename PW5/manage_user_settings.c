#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void set_env_variable(char *arg) {
    char *key = strtok(arg, "=");
    char *value = strtok(NULL, "=");
    if (key && value) {
        if (setenv(key, value, 1) == 0) {
            printf("Environment variable %s set to %s\n", key, value);
        } else {
            perror("Error setting environment variable");
        }
    } else {
        fprintf(stderr, "Invalid format. Use KEY=VALUE.\n");
    }
}

void get_env_variable(char *arg) {
    char *value = getenv(arg);
    if (value) {
        printf("%s=%s\n", arg, value);
    } else {
        printf("Environment variable %s is not set\n", arg);
    }
}

void print_pid() {
    printf("Current PID: %d\n", getpid());
}

void print_ppid() {
    printf("Parent PID: %d\n", getppid());
}

void print_segment_addresses() {
    extern char etext, edata, end;
    printf("TEXT segment ends at: %p\n", &etext);
    printf("DATA segment ends at: %p\n", &edata);
    printf("BSS segment ends at: %p\n", &end);
}

void print_uid() {
    printf("Real User ID (UID): %d\n", getuid());
}

void print_usage(const char *prog_name) {
    printf("Usage: %s [options]\n", prog_name);
    printf("Options:\n");
    printf("  -s VAR=VALUE     Set an environment variable\n");
    printf("  -g VAR           Get the value of an environment variable\n");
    printf("  -p               Print the current process ID (PID)\n");
    printf("  -r               Print the parent process ID (PPID)\n");
    printf("  -a               Print segment addresses (TEXT, DATA, BSS)\n");
    printf("  -u               Print the real user ID\n");
}

// Структура для зберігання команди та відповідної функції
typedef struct {
    const char *command;
    void (*handler)(char *arg);
    int requires_arg; // Чи потребує команда аргумент
} Command;

// Масив команд
Command commands[] = {
    {"-s", set_env_variable, 1},
    {"-g", get_env_variable, 1},
    {"-p", (void (*)(char *))print_pid, 0},
    {"-r", (void (*)(char *))print_ppid, 0},
    {"-a", (void (*)(char *))print_segment_addresses, 0},
    {"-u", (void (*)(char *))print_uid, 0},
    {NULL, NULL, 0} // Кінець масиву
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        int handled = 0;

        for (int j = 0; commands[j].command != NULL; j++) {
            if (strcmp(argv[i], commands[j].command) == 0) {
                handled = 1;
                if (commands[j].requires_arg) {
                    if (i + 1 < argc) {
                        commands[j].handler(argv[++i]);
                    } else {
                        fprintf(stderr, "Option %s requires an argument\n", argv[i]);
                        return 1;
                    }
                } else {
                    commands[j].handler(NULL);
                }
                break;
            }
        }

        if (!handled) {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    return 0;
}

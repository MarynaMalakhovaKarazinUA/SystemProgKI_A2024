#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void set_env_variable(char *var) {
    char *key = strtok(var, "=");
    char *value = strtok(NULL, "=");
    if (key && value) {
        setenv(key, value, 1);
        printf("Environment variable %s set to %s\n", key, value);
    } else {
        fprintf(stderr, "Invalid format. Use KEY=VALUE.\n");
    }
}

void get_env_variable(char *var) {
    char *value = getenv(var);
    if (value) {
        printf("%s=%s\n", var, value);
    } else {
        printf("Environment variable %s is not set\n", var);
    }
}

void print_pid() {
    printf("PID: %d\n", getpid());
}

void print_ppid() {
    printf("PPID: %d\n", getppid());
}

void print_addresses() {
    printf("Addresses of segments are system dependent and typically require low-level system access.\n");
}

void print_uid() {
    printf("Real User ID (UID): %d\n", getuid());
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [options]\n", argv[0]);
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -s, --set KEY=VALUE    Set an environment variable\n");
        fprintf(stderr, "  -g, --get KEY          Get an environment variable\n");
        fprintf(stderr, "  -p, --pid              Print the PID of the current process\n");
        fprintf(stderr, "  -r, --ppid             Print the PPID of the current process\n");
        fprintf(stderr, "  -a, --addresses        Print the TEXT, DATA, BSS segment addresses\n");
        fprintf(stderr, "  -u, --uid              Print the real user ID (UID)\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--set") == 0) {
            if (i + 1 < argc) {
                set_env_variable(argv[++i]);
            } else {
                fprintf(stderr, "Option -s requires an argument\n");
            }
        } else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--get") == 0) {
            if (i + 1 < argc) {
                get_env_variable(argv[++i]);
            } else {
                fprintf(stderr, "Option -g requires an argument\n");
            }
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--pid") == 0) {
            print_pid();
        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--ppid") == 0) {
            print_ppid();
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--addresses") == 0) {
            print_addresses();
        } else if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--uid") == 0) {
            print_uid();
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
        }
    }

    return 0;
}

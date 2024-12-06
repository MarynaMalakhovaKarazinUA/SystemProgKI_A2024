#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_usage(const char *prog_name) {
    printf("Usage: %s [options]\n", prog_name);
    printf("Options:\n");
    printf("  -s, --set VAR=VALUE     Set an environment variable\n");
    printf("  -g, --get VAR           Get the value of an environment variable\n");
    printf("  -p, --pid               Print the current process ID (PID)\n");
    printf("  -r, --ppid              Print the parent process ID (PPID)\n");
    printf("  -a, --addresses         Print segment addresses (TEXT, DATA, BSS)\n");
    printf("  -u, --uid               Print the real user ID\n");
}

void print_segment_addresses() {
    extern char etext, edata, end; // These symbols are defined by the linker
    printf("TEXT segment ends at: %p\n", &etext);
    printf("DATA segment ends at: %p\n", &edata);
    printf("BSS segment ends at: %p\n", &end);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--set") == 0) {
            if (i + 1 < argc) {
                char *env_var = argv[++i];
                char *delimiter = strchr(env_var, '=');
                if (delimiter != NULL) {
                    *delimiter = '\0'; // Split into key and value
                    const char *key = env_var;
                    const char *value = delimiter + 1;
                    if (setenv(key, value, 1) == 0) {
                        printf("Set environment variable: %s=%s\n", key, value);
                    } else {
                        perror("Failed to set environment variable");
                    }
                } else {
                    fprintf(stderr, "Invalid format for -s/--set. Use VAR=VALUE.\n");
                }
            } else {
                fprintf(stderr, "Missing argument for -s/--set.\n");
            }
        } else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--get") == 0) {
            if (i + 1 < argc) {
                const char *key = argv[++i];
                const char *value = getenv(key);
                if (value) {
                    printf("%s=%s\n", key, value);
                } else {
                    printf("Environment variable '%s' not found.\n", key);
                }
            } else {
                fprintf(stderr, "Missing argument for -g/--get.\n");
            }
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--pid") == 0) {
            printf("Current PID: %d\n", getpid());
        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--ppid") == 0) {
            printf("Parent PID: %d\n", getppid());
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--addresses") == 0) {
            print_segment_addresses();
        } else if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--uid") == 0) {
            printf("Real User ID: %d\n", getuid());
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    return 0;
}

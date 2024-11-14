#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include <termios.h>
#include <errno.h>

#define MAX_INPUT 128

// Function to securely get password input without echoing
void get_password(char *password, size_t max_len) {
    struct termios oldt, newt;

    // Get current terminal settings
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
    newt = oldt;

    // Disable ECHO flag to hide input
    newt.c_lflag &= ~ECHO;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    // Prompt for password
    if (fgets(password, max_len, stdin) == NULL) {
        perror("fgets");
        // Restore terminal settings before exiting
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        exit(EXIT_FAILURE);
    }

    // Restore terminal settings
    if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) != 0) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    // Remove newline character if present
    size_t len = strlen(password);
    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    }
}

int main() {
    char username[MAX_INPUT];
    char password[MAX_INPUT];
    struct passwd *pwd;
    struct spwd *spwd_entry;
    char *encrypted;

    // Prompt for username
    printf("Enter username: ");
    if (fgets(username, sizeof(username), stdin) == NULL) {
        perror("fgets");
        exit(EXIT_FAILURE);
    }
    // Remove newline character
    size_t len = strlen(username);
    if (len > 0 && username[len - 1] == '\n') {
        username[len - 1] = '\0';
    }

    // Prompt for password
    printf("Enter password: ");
    get_password(password, sizeof(password));
    printf("\n");

    // Retrieve user information from /etc/passwd
    pwd = getpwnam(username);
    if (pwd == NULL) {
        fprintf(stderr, "User '%s' not found.\n", username);
        exit(EXIT_FAILURE);
    }

    // Retrieve shadow password entry
    errno = 0;
    spwd_entry = getspnam(username);
    if (spwd_entry == NULL) {
        if (errno != 0) {
            perror("getspnam");
        } else {
            fprintf(stderr, "No shadow entry found for user '%s'.\n", username);
        }
        exit(EXIT_FAILURE);
    }

    // Check if password is set and authentication is allowed
    if (spwd_entry->sp_pwdp == NULL || strlen(spwd_entry->sp_pwdp) == 0) {
        fprintf(stderr, "No password set for user '%s' or authentication is disabled.\n", username);
        exit(EXIT_FAILURE);
    }

    // Encrypt the entered password using the salt from /etc/shadow
    encrypted = crypt(password, spwd_entry->sp_pwdp);
    if (encrypted == NULL) {
        perror("crypt");
        exit(EXIT_FAILURE);
    }

    // Compare the encrypted password with the stored password
    if (strcmp(encrypted, spwd_entry->sp_pwdp) == 0) {
        printf("Welcome, %s!\n", username);
    } else {
        printf("Incorrect password for user '%s'.\n", username);
    }

    // Clear the password from memory
    memset(password, 0, sizeof(password));

    return 0;
}

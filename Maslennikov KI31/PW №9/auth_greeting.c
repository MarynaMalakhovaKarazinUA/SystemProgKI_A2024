#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>

void greet_user(const char *username) {
    printf("Hello, %s! Welcome back!\n", username);
}

int authenticate_user(const char *username, const char *password) {
    struct passwd *pwd = getpwnam(username);
    if (!pwd) {
        fprintf(stderr, "User not found\n");
        return 0;
    }

    struct spwd *sp = getspnam(username);
    if (!sp) {
        fprintf(stderr, "Failed to get shadow entry\n");
        return 0;
    }

    char *encrypted = crypt(password, sp->sp_pwdp);
    if (!encrypted) {
        perror("crypt");
        return 0;
    }

    return strcmp(encrypted, sp->sp_pwdp) == 0;
}

int main() {
    char username[256];
    char *password;

    printf("Username: ");
    if (scanf("%255s", username) != 1) {
        fprintf(stderr, "Failed to read username\n");
        return 1;
    }

    password = getpass("Password: ");
    if (!password) {
        fprintf(stderr, "Failed to read password\n");
        return 1;
    }

    if (authenticate_user(username, password)) {
        greet_user(username);
    } else {
        fprintf(stderr, "Authentication failed\n");
    }

    return 0;
}

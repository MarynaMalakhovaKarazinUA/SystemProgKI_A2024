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
#include <limits.h>

#define MAX_INPUT 128

// Функція для безпечного отримання пароля без відображення введених символів
void get_password(char *password, size_t max_len) {
    struct termios oldt, newt;

    // Отримуємо поточні налаштування терміналу
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
    newt = oldt;

    // Вимикаємо ECHO для приховування введення
    newt.c_lflag &= ~ECHO;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    // Запитуємо пароль
    if (fgets(password, max_len, stdin) == NULL) {
        perror("fgets");
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Відновлюємо налаштування терміналу
        exit(EXIT_FAILURE);
    }

    // Відновлюємо налаштування терміналу
    if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) != 0) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    // Видаляємо символ нового рядка (якщо він є)
    size_t len = strlen(password);
    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    }
}

// Функція для аутентифікації користувача
int authenticate_user(const char *username, const char *password) {
    struct passwd *pwd = getpwnam(username);
    if (pwd == NULL) {
        fprintf(stderr, "User '%s' not found.\n", username);
        return 0;
    }

    struct spwd *spwd_entry = getspnam(username);
    if (spwd_entry == NULL) {
        if (errno != 0) {
            perror("getspnam");
        } else {
            fprintf(stderr, "No shadow entry found for user '%s'.\n", username);
        }
        return 0;
    }

    // Перевірка пароля
    char *encrypted = crypt(password, spwd_entry->sp_pwdp);
    if (encrypted == NULL) {
        perror("crypt");
        return 0;
    }

    return strcmp(encrypted, spwd_entry->sp_pwdp) == 0;
}

// Функція для привітання користувача
void greet_user(const char *username) {
    printf("Welcome, %s!\n", username);
}

// Основна функція
int main() {
    char username[MAX_INPUT];
    char password[MAX_INPUT];

    // Запитуємо ім'я користувача
    printf("Enter username: ");
    if (fgets(username, sizeof(username), stdin) == NULL) {
        perror("fgets");
        exit(EXIT_FAILURE);
    }
    // Видаляємо символ нового рядка
    size_t len = strlen(username);
    if (len > 0 && username[len - 1] == '\n') {
        username[len - 1] = '\0';
    }

    // Запитуємо пароль
    printf("Enter password: ");
    get_password(password, sizeof(password));
    printf("\n");

    // Аутентифікація користувача
    if (authenticate_user(username, password)) {
        greet_user(username);
    } else {
        fprintf(stderr, "Authentication failed for user '%s'.\n", username);
    }

    // Очищаємо пам'ять пароля
    memset(password, 0, sizeof(password));

    return 0;
}

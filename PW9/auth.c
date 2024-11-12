#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
#include <shadow.h>
#include <pwd.h>
#include <termios.h>

// Функція для зчитування пароля без відображення введених символів
void read_password(char *password, size_t size) {
    struct termios oldt, newt;
    printf("Password: ");
    fflush(stdout);
    
    // Отримуємо поточні налаштування терміналу
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO; // Вимикаємо ECHO для приховування введення
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // Читаємо пароль
    if (fgets(password, size, stdin) == NULL) {
        fprintf(stderr, "Error reading password.\n");
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Відновлюємо налаштування терміналу
    printf("\n");
    
    // Видаляємо символ нового рядка
    size_t len = strlen(password);
    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    }
}

int main() {
    char username[256];
    char password[256];
    
    // Запитуємо ім'я користувача
    printf("Username: ");
    if (scanf("%255s", username) != 1) {
        fprintf(stderr, "Error reading username.\n");
        return 1;
    }

    // Очищаємо залишки в буфері, щоб наступне зчитування працювало правильно
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    // Зчитуємо пароль
    read_password(password, sizeof(password));
    
    // Отримуємо інформацію про користувача
    struct spwd *shadow_entry = getspnam(username);
    if (!shadow_entry) {
        fprintf(stderr, "Authentication failed: user not found.\n");
        return 1;
    }
    
    // Шифруємо введений пароль
    char *encrypted_password = crypt(password, shadow_entry->sp_pwdp);
    
    // Порівнюємо зашифрований пароль
    if (strcmp(encrypted_password, shadow_entry->sp_pwdp) == 0) {
        printf("Welcome, %s!\n", username);
    } else {
        fprintf(stderr, "Authentication failed: incorrect password.\n");
        return 1;
    }
    
    return 0;
}

#include <stdio.h>
#include <string.h>

// Структура для збереження даних користувача
typedef struct {
    char username[50];
    char password[50];
} User;

// Перевірка авторизації
int authenticate(User users[], int userCount, char *inputUsername, char *inputPassword) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, inputUsername) == 0 &&
            strcmp(users[i].password, inputPassword) == 0) {
            return 1; // Авторизація успішна
        }
    }
    return 0; // Авторизація неуспішна
}

int main() {
    // Ініціалізація списку користувачів
    User users[] = {
        {"admin", "1234"},
        {"katya", "securepass"},
        {"user", "password"}
    };
    int userCount = sizeof(users) / sizeof(users[0]);

    char inputUsername[50];
    char inputPassword[50];

    // Запит даних у користувача
    printf("Enter username: ");
    scanf("%49s", inputUsername);

    printf("Enter password: ");
    scanf("%49s", inputPassword);

    // Перевірка авторизації
    if (authenticate(users, userCount, inputUsername, inputPassword)) {
        printf("Welcome, %s!\n", inputUsername);
    } else {
        printf("Authorization failed. Access denied.\n");
    }

    return 0;
}

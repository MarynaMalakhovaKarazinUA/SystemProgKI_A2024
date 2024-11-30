#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

#define YEARS 5

int main() {
    // Встановлення локалі (можна змінити на потрібну, наприклад, "uk_UA.UTF-8")
    setlocale(LC_TIME, "");

    char locale[100];
    printf("Введіть локаль (наприклад, uk_UA.UTF-8): ");
    scanf("%99s", locale);
    setlocale(LC_TIME, locale);

    char datetime_str[100];
    printf("Введіть дату та час народження (формат YYYY-MM-DD HH:MM:SS): ");
    scanf(" %[^\n]", datetime_str);

    // Парсинг введеної дати та часу
    struct tm birth_tm;
    memset(&birth_tm, 0, sizeof(struct tm));
    if (strptime(datetime_str, "%Y-%m-%d %H:%M:%S", &birth_tm) == NULL) {
        fprintf(stderr, "Неправильний формат дати та часу.\n");
        return 1;
    }

    time_t birth_time = mktime(&birth_tm);
    if (birth_time == -1) {
        fprintf(stderr, "Помилка перетворення часу.\n");
        return 1;
    }

    // Отримання поточної локалі та часового поясу
    char timezone[100];
    printf("Введіть часовий пояс (наприклад, Europe/Prague): ");
    scanf("%99s", timezone);
    setenv("TZ", timezone, 1);
    tzset();

    printf("\nДата, час та день тижня на найближчі %d років:\n", YEARS);
    printf("--------------------------------------------------\n");

    struct tm *current_tm = localtime(&birth_time);
    for (int i = 0; i < YEARS; i++) {
        // Збільшення року
        current_tm->tm_year += 1;
        // Перетворення назад у time_t
        time_t new_time = mktime(current_tm);
        if (new_time == -1) {
            fprintf(stderr, "Помилка перетворення часу.\n");
            return 1;
        }
        // Отримання локального часу
        struct tm *new_tm = localtime(&new_time);
        // Форматування та вивід
        char buffer[100];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S %A", new_tm);
        printf("%s\n", buffer);
    }

    return 0;
}

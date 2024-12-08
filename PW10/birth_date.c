#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

#define YEARS 5

void calculate_next_five_years(time_t birth_time) {
    struct tm *time_info;
    char buffer[100];

    for (int i = 1; i <= YEARS; i++) {
        // Копіюємо дату народження, щоб змінити рік
        struct tm current_tm = *localtime(&birth_time);
        current_tm.tm_year += i; // Збільшуємо рік

        // Перетворюємо назад у time_t для коректної обробки
        time_t future_time = mktime(&current_tm);
        if (future_time == -1) {
            fprintf(stderr, "Помилка перетворення часу.\n");
            return;
        }

        // Отримуємо локальний час
        time_info = localtime(&future_time);

        // Форматування та виведення дати і часу з урахуванням локалі
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S %A", time_info);
        printf("Year %d: %s\n", i, buffer);
    }
}

int main() {
    // Встановлюємо локаль (можна змінити на потрібну, наприклад, "uk_UA.UTF-8")
    setlocale(LC_TIME, "");

    char locale[100];
    printf("Введіть локаль (наприклад, uk_UA.UTF-8): ");
    scanf("%99s", locale);
    setlocale(LC_TIME, locale);

    // Введення дати та часу народження
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

    // Введення часового поясу
    char timezone[100];
    printf("Введіть часовий пояс (наприклад, Europe/Kiev): ");
    scanf("%99s", timezone);
    setenv("TZ", timezone, 1);
    tzset();

    printf("\nДата, час та день тижня на найближчі %d років:\n", YEARS);
    printf("--------------------------------------------------\n");

    // Обчислюємо та виводимо наступні 5 років
    calculate_next_five_years(birth_time);

    return 0;
}

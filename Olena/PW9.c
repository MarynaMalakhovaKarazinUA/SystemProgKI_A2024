#include <stdio.h>
#include <time.h>
#include <locale.h>

void print_date_and_day(const struct tm *time_info) {
    char buffer[80];
    
    // Форматуємо дату в рядок
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);
    printf("Date and time: %s\n", buffer);

    // Форматуємо день тижня
    strftime(buffer, sizeof(buffer), "%A", time_info);
    printf("Day of the week: %s\n", buffer);
}

int main() {
    struct tm birth_date = {0};
    int year, month, day, hour, minute, second;

    // Запитуємо у користувача дату та час народження
    printf("Enter your birth date (YYYY MM DD HH MM SS): ");
    scanf("%d %d %d %d %d %d", &year, &month, &day, &hour, &minute, &second);

    // Заповнюємо структуру time_t
    birth_date.tm_year = year - 1900;  // tm_year - це рік з 1900
    birth_date.tm_mon = month - 1;     // tm_mon - місяць від 0 до 11
    birth_date.tm_mday = day;
    birth_date.tm_hour = hour;
    birth_date.tm_min = minute;
    birth_date.tm_sec = second;

    // Налаштовуємо локаль для відображення дат і часу
    setlocale(LC_TIME, "en_US.utf8"); // Для прикладу використовуємо англійську локаль

    // Виводимо дату, час і день тижня для наступних 5 років
    for (int i = 0; i < 5; i++) {
        struct tm future_date = birth_date;

        // Збільшуємо рік
        future_date.tm_year += i;

        // Перевіряємо, чи дата не вийшла за межі, і коригуємо
        mktime(&future_date);

        printf("\nYear: %d\n", future_date.tm_year + 1900);  // Виводимо рік (знову додаємо 1900)
        print_date_and_day(&future_date);
    }

    return 0;
}
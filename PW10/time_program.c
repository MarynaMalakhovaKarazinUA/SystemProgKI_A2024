#include <stdio.h>
#include <time.h>
#include <locale.h>

void calculate_next_five_years(time_t birth_time) {
    struct tm *time_info;
    char buffer[80];
    
    for (int i = 1; i <= 5; i++) {
        time_t future_time = birth_time + (i * 365 * 24 * 60 * 60); // додавання року в секундах
        time_info = localtime(&future_time);
        
        // Формат виводу дати і часу з урахуванням локалі
        strftime(buffer, 80, "%c", time_info);
        printf("Year %d: %s\n", i, buffer);
    }
}

int main() {
    setlocale(LC_TIME, "uk_UA.utf8"); // Установлення локалі для України
    
    int year, month, day, hour, minute;
    printf("Enter your birth date (YYYY MM DD HH MM): ");
    scanf("%d %d %d %d %d", &year, &month, &day, &hour, &minute);
    
    struct tm birth_tm = {0};
    birth_tm.tm_year = year - 1900;
    birth_tm.tm_mon = month - 1;
    birth_tm.tm_mday = day;
    birth_tm.tm_hour = hour;
    birth_tm.tm_min = minute;
    
    time_t birth_time = mktime(&birth_tm);
    if (birth_time == -1) {
        printf("Error in calculating time.\n");
        return 1;
    }
    
    calculate_next_five_years(birth_time);
    return 0;
}

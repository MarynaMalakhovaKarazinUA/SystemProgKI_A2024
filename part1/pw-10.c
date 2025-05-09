#include <stdio.h>
#include <time.h>

// Function to print date, time, and day of the week
void print_date_time_day(const struct tm *date) {
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M (%A)", date);
    printf("%s\n", buffer);
}

int main() {
    // Input: date and time of birth
    int year, month, day, hour, minute;
    char timezone[10];

    printf("Enter your date of birth (year month day hour minute, e.g., 2000 1 1 12 0): ");
    scanf("%d %d %d %d %d", &year, &month, &day, &hour, &minute);

    printf("Enter the time zone (e.g., UTC, EET, PST): ");
    scanf("%s", timezone);

    // Set the local time zone
    setenv("TZ", timezone, 1);
    tzset();

    // Create a tm structure for the entered date
    struct tm date = {0};
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;
    date.tm_hour = hour;
    date.tm_min = minute;

    // Convert to time in seconds
    time_t base_time = mktime(&date);

    if (base_time == -1) {
        printf("Error converting the date.\n");
        return 1;
    }

    printf("Dates for the next 5 years:\n");

    // Output the date for each year
    for (int i = 0; i < 5; i++) {
        struct tm *next_date = localtime(&base_time);
        print_date_time_day(next_date);

        // Add one year
        date.tm_year++;
        base_time = mktime(&date);
    }

    return 0;
}

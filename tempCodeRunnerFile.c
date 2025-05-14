void display_calendar_grid(const char *house_code, int year, int month) {
    const char *days[] = { "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };

    printf(BLUE_COLOR "\n%s %d - Availability for House %s\n" RESET_COLOR, 
        month_name(month), year, house_code);
 

        
    for (int i = 0; i < 7; i++) {
        printf("%-4s", days[i]);
    }
    printf("\n");

    
    struct tm first = {0};
    first.tm_year = year - 1900;
    first.tm_mon = month - 1;
    first.tm_mday = 1;
    mktime(&first);  

    int first_wday = first.tm_wday;

    int days_in_month[] = { 31, is_leap_year(year) ? 29 : 28, 31, 30, 31, 30,
                            31, 31, 30, 31, 30, 31 };
    int total_days = days_in_month[month - 1];


    for (int i = 0; i < first_wday; i++) {
        printf("    "); 
    }


    time_t now = time(NULL);
struct tm today = *localtime(&now);
struct tm end_date_tm = today;
end_date_tm.tm_mday += 30;
mktime(&end_date_tm);  // normalize

time_t start_time = mktime(&today);
time_t end_time = mktime(&end_date_tm);
    for (int day = 1; day <= total_days; day++) {
        char date_str[20];
        snprintf(date_str, sizeof(date_str), "%04d-%02d-%02d", year, month, day);

        char status_char = ' ';
        for (int i = 0; i < calendar_count; i++) {
            char status_upper[20];
            strcpy(status_upper, calendar[i].status);
            to_uppercase(status_upper); 

            if (strcmp(status_upper, "AVAILABLE") == 0)
            status_char = 'A';
            else if (strcmp(status_upper, "BOOKED") == 0)
                status_char = 'B';
            else
                status_char = 'X';
            
        }

        char cell[5];
        snprintf(cell, sizeof(cell), "%02d%c", day, status_char);
        printf("%-4s", cell);
        if ((first_wday + day) % 7 == 0) {
            printf("\n");
        }
    }

    printf("\n\nLegend:\n");
    printf(GREEN_COLOR "A = Available\n" RESET_COLOR);
    printf(RED_COLOR "B = Booked\n" RESET_COLOR);
    printf("X = Blocked by manager\n");

    printf(YELLOW_COLOR "Press Enter to continue..." RESET_COLOR);
    getchar(); getchar();
}

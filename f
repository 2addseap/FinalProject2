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
mktime(&end_date_tm); 

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



void manager_set_availability() {
    clear_screen();

    char house_code[10];
    printf(GREEN_COLOR "Enter House Code to manage availability (e.g. HH1): " RESET_COLOR);
    scanf("%s", house_code);
    to_uppercase(house_code);

    int found = 0;
    for (int i = 0; i < house_count; i++) {
        if (strcmp(houses[i].code, house_code) == 0) {
            found = 1;
            break;
        }
    }
    if (!found) {
        printf(RED_COLOR "House code not found.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    int year, month;
    printf(GREEN_COLOR "Enter month and year to view (MM YYYY): " RESET_COLOR);
    scanf("%d %d", &month, &year);

    display_calendar_grid(house_code, year, month); 

    printf(YELLOW_COLOR "\n1. Block a date\n2. Unblock a date\n0. Return\nChoose: " RESET_COLOR);
    int option;
    scanf("%d", &option);

    if (option == 0) return;

    int day;
    printf(GREEN_COLOR "Enter the day to modify (1-31): " RESET_COLOR);
    scanf("%d", &day);

    if (day < 1 || day > 31) {
        printf(RED_COLOR "Invalid day!\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    char date_str[20];
    snprintf(date_str, sizeof(date_str), "%04d-%02d-%02d", year, month, day);

    for (int i = 0; i < calendar_count; i++) {
        if (strcmp(calendar[i].code, house_code) == 0 &&
            strcmp(calendar[i].date, date_str) == 0) {

            if (option == 1) {
                printf(GREEN_COLOR "Enter reason to block (e.g., MAINTENANCE): " RESET_COLOR);
                char reason[30];
                getchar(); 
                fgets(reason, sizeof(reason), stdin);
                reason[strcspn(reason, "\n")] = 0;
                to_uppercase(reason);
                strcpy(calendar[i].status, reason);
            } else if (option == 2) {
                if (strcmp(calendar[i].status, "Booked") == 0) {
                    printf(RED_COLOR "Cannot unblock a booked date.\n" RESET_COLOR);
                } else {
                    strcpy(calendar[i].status, "Available");
                }
            }

            break;
        }
    }

    FILE *file = fopen("Calendar.csv", "w");
    if (file) {
        fprintf(file, "Code,Date,Status\n");
        for (int i = 0; i < calendar_count; i++) {
            fprintf(file, "%s,%s,%s\n", calendar[i].code, calendar[i].date, calendar[i].status);
        }
        fclose(file);
        printf(GREEN_COLOR "Calendar updated successfully.\n" RESET_COLOR);
    }

    printf(YELLOW_COLOR "Press Enter to return..." RESET_COLOR);
    getchar(); getchar();
}


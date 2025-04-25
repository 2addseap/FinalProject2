#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define ANSI escape codes for colors
#define RESET_COLOR "\033[0m"
#define BLUE_COLOR "\033[1;34m"
#define GREEN_COLOR "\033[1;32m"
#define RED_COLOR "\033[1;31m"
#define YELLOW_COLOR "\033[1;33m"
#define WHITE_COLOR "\033[1;37m"

void clear_screen() {
    // For Windows
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[H\033[J");  // Clear the screen using ANSI escape codes
    #endif
    fflush(stdout);  // Ensure everything is flushed and displayed properly
}


// Define structures
typedef struct {
    int day, month, year;
} Date;

typedef struct {
    int id;
    char name[50];
    char location[50];
    float price;
    int is_available;
} House;

typedef struct {
    int id;
    int house_id;
    int customer_id;
    Date date;
    char status[10];
} Booking;

House houses[10];
Booking bookings[10];
int house_count = 0;
int booking_count = 0;

void show_title() {
    printf("\n%s--------------------------------------------------\n", BLUE_COLOR);
    printf("              HOUSE BOOKING SYSTEM                \n");
    printf("--------------------------------------------------\n" RESET_COLOR);
}

void manager_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n%s--------------------------------------------------\n", YELLOW_COLOR);
        printf("                   MANAGER MENU                   \n");
        printf("--------------------------------------------------\n" RESET_COLOR);
        printf("1. Add House\n");
        printf("2. Edit House\n");
        printf("3. Delete House\n");
        printf("4. Set Availability\n");
        printf("5. View All Houses\n");
        printf("6. View Bookings For a House\n");
        printf("7. Accept/Reject Booking\n");
        printf("0. Back\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: /* Add House */ break;
            case 2: /* Edit House */ break;
            case 3: /* Delete House */ break;
            case 4: /* Set Availability */ break;
            case 5: /* View All Houses */ break;
            case 6: /* View Bookings For a House */ break;
            case 7: /* Accept/Reject Booking */ break;
            case 0: return;
            default: printf(RED_COLOR "Invalid choice!\n" RESET_COLOR);
        }
    }
}

void customer_view_all_houses() {
    printf("%sFunction: View All Houses\n" RESET_COLOR);
}

void customer_search_by_location() {
    printf("%sFunction: Search\n" RESET_COLOR);
}

void customer_filter_by_price() {
    printf("%sFunction: Filter\n" RESET_COLOR);
}

void customer_make_booking() {
    printf("%sFunction: Booking\n" RESET_COLOR);
}

void customer_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n%s--------------------------------------------------\n", YELLOW_COLOR);
        printf("                   CUSTOMER MENU                  \n");
        printf("--------------------------------------------------\n" RESET_COLOR);
        printf("1. View All Houses\n");
        printf("2. Search House\n");
        printf("3. Filter Houses by Price\n");
        printf("4. Make Booking\n");
        printf("0. Back\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: customer_view_all_houses(); break;
            case 2: customer_search_by_location(); break;
            case 3: customer_filter_by_price(); break;
            case 4: customer_make_booking(); break;
            case 0: return;
            default: printf(RED_COLOR "Invalid choice!\n" RESET_COLOR);
        }
    }
}

void run_system() {
    int choice;
    while (1) {
        clear_screen();
        show_title();
        printf("%sChoose your role:\n", YELLOW_COLOR);
        printf("1. Customer\n");
        printf("2. Manager\n");
        printf("0. Exit\n");
        printf("Your choice: ");

        // Input validation for scanf
        int result = scanf("%d", &choice);
        if (result != 1) {
            while (getchar() != '\n');  // Clear input buffer
            printf(RED_COLOR "Invalid input. Please enter a valid number.\n" RESET_COLOR);
            continue;  // Skip this loop iteration and prompt again
        }

        // Debugging output (optional)
        printf("You selected choice %d\n", choice); // Debug print

        switch (choice) {
            case 1: customer_menu(); break;
            case 2: manager_menu(); break;
            case 0:
                printf(RED_COLOR "Goodbye!\n" RESET_COLOR);
                return;
            default:
                printf(RED_COLOR "Invalid input. Please try again.\n" RESET_COLOR);
        }
    }
}



int main() {
    run_system();
    return 0;
}

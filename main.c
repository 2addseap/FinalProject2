#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

// Define ANSI escape codes for colors
#define RESET_COLOR "\033[0m"
#define BLUE_COLOR "\033[1;34m"
#define GREEN_COLOR "\033[1;32m"
#define RED_COLOR "\033[1;31m"
#define YELLOW_COLOR "\033[1;33m"
#define WHITE_COLOR "\033[1;37m"

// Clear screen function
void clear_screen() {
    printf("\033[2J\033[H");  // Clears terminal and resets cursor
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

void manager_add_house() {
    if (house_count >= 10) {
        printf(RED_COLOR "House limit reached!\n" RESET_COLOR);
        return;
    }

    House new_house;
    printf(GREEN_COLOR "Enter House ID: " RESET_COLOR);
    scanf("%d", &new_house.id);
    getchar();
    printf(GREEN_COLOR "Enter House Name: " RESET_COLOR);
    fgets(new_house.name, sizeof(new_house.name), stdin);
    new_house.name[strcspn(new_house.name, "\n")] = 0;
    printf(GREEN_COLOR "Enter Location: " RESET_COLOR);
    fgets(new_house.location, sizeof(new_house.location), stdin);
    new_house.location[strcspn(new_house.location, "\n")] = 0;
    printf(GREEN_COLOR "Enter Price: " RESET_COLOR);
    scanf("%f", &new_house.price);
    new_house.is_available = 1;

    houses[house_count++] = new_house;
    printf(GREEN_COLOR "House added successfully!\n" RESET_COLOR);
}

void manager_view_all_houses() {
    if (house_count == 0) {
        printf(RED_COLOR "No houses available!\n" RESET_COLOR);
        return;
    }

    printf(YELLOW_COLOR "List of all houses:\n" RESET_COLOR);
    for (int i = 0; i < house_count; i++) {
        printf("ID: %d, Name: %s, Location: %s, Price: %.2f, Available: %s\n",
               houses[i].id, houses[i].name, houses[i].location, houses[i].price,
               houses[i].is_available ? "Yes" : "No");
    }
}

void manager_accept_booking() {
    if (booking_count == 0) {
        printf(RED_COLOR "No pending bookings!\n" RESET_COLOR);
        return;
    }

    printf(YELLOW_COLOR "Pending bookings:\n" RESET_COLOR);
    for (int i = 0; i < booking_count; i++) {
        if (strcmp(bookings[i].status, "Pending") == 0) {
            printf("Booking ID: %d, House ID: %d, Customer ID: %d, Date: %d/%d/%d\n",
                   bookings[i].id, bookings[i].house_id, bookings[i].customer_id,
                   bookings[i].date.day, bookings[i].date.month, bookings[i].date.year);
        }
    }

    int booking_id;
    printf(GREEN_COLOR "Enter the Booking ID to accept: " RESET_COLOR);
    scanf("%d", &booking_id);

    for (int i = 0; i < booking_count; i++) {
        if (bookings[i].id == booking_id && strcmp(bookings[i].status, "Pending") == 0) {
            strcpy(bookings[i].status, "Accepted");
            printf(GREEN_COLOR "Booking Accepted!\n" RESET_COLOR);
            for (int j = 0; j < house_count; j++) {
                if (houses[j].id == bookings[i].house_id) {
                    houses[j].is_available = 0;
                    break;
                }
            }
            return;
        }
    }
    printf(RED_COLOR "Booking not found or already processed.\n" RESET_COLOR);
}

void customer_search_by_location() {
    char location[50];
    getchar();
    printf(GREEN_COLOR "Enter location to search for houses: " RESET_COLOR);
    fgets(location, sizeof(location), stdin);
    location[strcspn(location, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < house_count; i++) {
        if (strcmp(houses[i].location, location) == 0 && houses[i].is_available == 1) {
            printf(GREEN_COLOR "Found House - ID: %d, Name: %s, Price: %.2f\n" RESET_COLOR,
                   houses[i].id, houses[i].name, houses[i].price);
            found = 1;
        }
    }
    if (!found) {
        printf(RED_COLOR "No available houses found in this location.\n" RESET_COLOR);
    }
}

void customer_make_booking() {
    if (house_count == 0) {
        printf(RED_COLOR "No houses available to book.\n" RESET_COLOR);
        return;
    }

    int house_id;
    printf(GREEN_COLOR "Enter House ID to book: " RESET_COLOR);
    scanf("%d", &house_id);

    int found = 0;
    for (int i = 0; i < house_count; i++) {
        if (houses[i].id == house_id && houses[i].is_available == 1) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED_COLOR "House not available for booking.\n" RESET_COLOR);
        return;
    }

    Booking new_booking;
    new_booking.id = booking_count + 1;
    new_booking.house_id = house_id;
    printf(GREEN_COLOR "Enter your Customer ID: " RESET_COLOR);
    scanf("%d", &new_booking.customer_id);
    printf(GREEN_COLOR "Enter booking date (day month year): " RESET_COLOR);
    scanf("%d %d %d", &new_booking.date.day, &new_booking.date.month, &new_booking.date.year);
    strcpy(new_booking.status, "Pending");

    bookings[booking_count++] = new_booking;
    printf(GREEN_COLOR "Booking request sent! Waiting for manager approval.\n" RESET_COLOR);
}

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
            case 1: manager_add_house(); break;
            case 5: manager_view_all_houses(); break;
            case 7: manager_accept_booking(); break;
            case 0: return;
            default: printf(RED_COLOR "Invalid choice!\n" RESET_COLOR);
        }
    }
}

void customer_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n%s--------------------------------------------------\n", YELLOW_COLOR);
        printf("                   CUSTOMER MENU                  \n");
        printf("--------------------------------------------------\n" RESET_COLOR);
        printf("1. Search House\n");
        printf("2. View House Detail\n");
        printf("3. Make Booking\n");
        printf("4. Cancel Booking\n");
        printf("5. Rate House\n");
        printf("6. Save Favorite\n");
        printf("7. View Booking History\n");
        printf("0. Back\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: customer_search_by_location(); break;
            case 3: customer_make_booking(); break;
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
        scanf("%d", &choice);

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

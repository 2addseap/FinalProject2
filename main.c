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
    char code[10];            // e.g., HH0, HH1, P2
    char name[50];
    char province[50];
    float price;
    float rating;
    int bedrooms;
    int beds;
    int bathrooms;
    int kitchens;
    int is_available;
} House;

typedef struct {
    int id;
    char house_code[10];
    int customer_id;
    Date date;
    char status[20];
} Booking;


House houses[10];
Booking bookings[10];
int house_count = 0;
int booking_count = 0;

//save the information to csv
void save_houses_to_csv(const char *Briefly_Info) {
    FILE *file = fopen(Briefly_Info, "w");
    if (!file) {
        printf(RED_COLOR "Failed to save house data.\n" RESET_COLOR);
        return;
    }

    fprintf(file, "code,name,province,price,rating,bedrooms,beds,bathrooms,kitchens\n");
    for (int i = 0; i < house_count; i++) {
        fprintf(file, "%s,%s,%s,%.2f,%.1f,%d,%d,%d,%d\n",
                houses[i].code, houses[i].name, houses[i].province,
                houses[i].price, houses[i].rating,
                houses[i].bedrooms, houses[i].beds,
                houses[i].bathrooms, houses[i].kitchens);
    }

    fclose(file);
    printf(GREEN_COLOR "Saved houses to CSV.\n" RESET_COLOR);
}

//load house information from csv file
void load_houses_from_csv(const char *Briefly_Info) {
    FILE *file = fopen(Briefly_Info, "r");
    if (!file) {
        printf(RED_COLOR "Failed to open house CSV file.\n" RESET_COLOR);
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip header

    house_count = 0;
    while (fgets(line, sizeof(line), file)) {
        House h;
        sscanf(line, "%9[^,],%49[^,],%49[^,],%f,%f,%d,%d,%d,%d",
               h.code, h.name, h.province, &h.price, &h.rating,
               &h.bedrooms, &h.beds, &h.bathrooms, &h.kitchens);
        h.is_available = 1; // Default to available
        houses[house_count++] = h;
    }

    fclose(file);
    //printf(GREEN_COLOR "Loaded %d houses from CSV.\n" RESET_COLOR, house_count);
}

void manager_add_house() {
    if (house_count >= 10) {
        printf(RED_COLOR "House limit reached!\n" RESET_COLOR);
        return;
    }

    House new_house;
    getchar();
    printf(GREEN_COLOR "Enter House Code (e.g., HH0): " RESET_COLOR);
    fgets(new_house.code, sizeof(new_house.code), stdin);
    new_house.code[strcspn(new_house.code, "\n")] = 0;
    printf(GREEN_COLOR "Enter House Name: " RESET_COLOR);
    fgets(new_house.name, sizeof(new_house.name), stdin);
    new_house.name[strcspn(new_house.name, "\n")] = 0;
    printf(GREEN_COLOR "Enter Province: " RESET_COLOR);
    fgets(new_house.province, sizeof(new_house.province), stdin);
    new_house.province[strcspn(new_house.province, "\n")] = 0;
    printf(GREEN_COLOR "Enter Price: " RESET_COLOR);
    scanf("%f", &new_house.price);
    printf(GREEN_COLOR "Enter Rating: " RESET_COLOR);
    scanf("%f", &new_house.rating);
    printf(GREEN_COLOR "Enter Number of Bedrooms: " RESET_COLOR);
    scanf("%d", &new_house.bedrooms);
    printf(GREEN_COLOR "Enter Number of Beds: " RESET_COLOR);
    scanf("%d", &new_house.beds);
    printf(GREEN_COLOR "Enter Number of Bathrooms: " RESET_COLOR);
    scanf("%d", &new_house.bathrooms);
    printf(GREEN_COLOR "Enter Number of Kitchens: " RESET_COLOR);
    scanf("%d", &new_house.kitchens);
    new_house.is_available = 1;

    houses[house_count++] = new_house;
    printf(GREEN_COLOR "House added successfully!\n" RESET_COLOR);
}

void manager_view_all_houses() {
    clear_screen();
    if (house_count == 0) {
        printf(RED_COLOR "No houses available!\n" RESET_COLOR);
    } else {
        printf(BLUE_COLOR "========================\n");
        printf("   LIST OF ALL HOUSES   \n");
        printf("========================\n" RESET_COLOR);

        for (int i = 0; i < house_count; i++) {
            printf(YELLOW_COLOR "\nHouse #%d\n" RESET_COLOR, i + 1);
            printf(WHITE_COLOR "Code: " RESET_COLOR "%s\n", houses[i].code);
            printf(WHITE_COLOR "Name: " RESET_COLOR "%s\n", houses[i].name);
            printf(WHITE_COLOR "Province: " RESET_COLOR "%s\n", houses[i].province);
            printf(WHITE_COLOR "Price: " RESET_COLOR "%.2f\n", houses[i].price);
            printf(WHITE_COLOR "Rating: " RESET_COLOR "%.1f\n", houses[i].rating);
            printf(WHITE_COLOR "Bedrooms: " RESET_COLOR "%d\n", houses[i].bedrooms);
            printf(WHITE_COLOR "Beds: " RESET_COLOR "%d\n", houses[i].beds);
            printf(WHITE_COLOR "Bathrooms: " RESET_COLOR "%d\n", houses[i].bathrooms);
            printf(WHITE_COLOR "Kitchens: " RESET_COLOR "%d\n", houses[i].kitchens);
            printf(WHITE_COLOR "Available: " RESET_COLOR "%s\n",
                   houses[i].is_available ? GREEN_COLOR "Yes" RESET_COLOR : RED_COLOR "No" RESET_COLOR);
        }
    }

    printf(YELLOW_COLOR "\nPress Enter to return to menu..." RESET_COLOR);
    getchar();
    getchar();  // to pause and wait for user input
}

void manager_accept_booking() {
    if (booking_count == 0) {
        printf(RED_COLOR "No pending bookings!\n" RESET_COLOR);
        return;
    }

    printf(YELLOW_COLOR "Pending bookings:\n" RESET_COLOR);
    for (int i = 0; i < booking_count; i++) {
        if (strcmp(bookings[i].status, "Pending") == 0) {
            printf("Booking ID: %d, House Code: %s, Customer ID: %d, Date: %d/%d/%d\n",
                   bookings[i].id, bookings[i].house_code, bookings[i].customer_id,
                   bookings[i].date.day, bookings[i].date.month, bookings[i].date.year);
        }
    }

    int booking_id;
    printf(GREEN_COLOR "Enter the Booking ID to accept: " RESET_COLOR);
    scanf("%d", &booking_id);

    for (int i = 0; i < booking_count; i++) {
        if (bookings[i].id == booking_id && strcmp(bookings[i].status, "Pending") == 0) {
            strcpy(bookings[i].status, "Accepted");

            // Mark house as unavailable
            for (int j = 0; j < house_count; j++) {
                if (strcmp(houses[j].code, bookings[i].house_code) == 0) {
                    houses[j].is_available = 0;
                    break;
                }
            }
            printf(GREEN_COLOR "Booking Accepted!\n" RESET_COLOR);
            return;
        }
    }

    printf(RED_COLOR "Booking not found or already processed.\n" RESET_COLOR);
}

void customer_search_by_location() {
    char province[50];
    getchar();
    printf(GREEN_COLOR "Enter province to search for houses: " RESET_COLOR);
    fgets(province, sizeof(province), stdin);
    province[strcspn(province, "\n")] = 0;
    int found = 0;
    for (int i = 0; i < house_count; i++) {
        if (strcmp(houses[i].province, province) == 0 && houses[i].is_available == 1) {
            printf(GREEN_COLOR "Found House - Code: %s, Name: %s, Price: %.2f, Rating: %.1f\n" RESET_COLOR,
                   houses[i].code, houses[i].name, houses[i].price, houses[i].rating);
            found = 1;
        }
    }
    if (!found) {
        printf(RED_COLOR "No available houses found in this province.\n" RESET_COLOR);
    }
}

void customer_make_booking() {
    if (house_count == 0) {
        printf(RED_COLOR "No houses available to book.\n" RESET_COLOR);
        return;
    }

    char house_code[10];
    getchar();
    printf(GREEN_COLOR "Enter House Code to book: " RESET_COLOR);
    fgets(house_code, sizeof(house_code), stdin);
    house_code[strcspn(house_code, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < house_count; i++) {
        if (strcmp(houses[i].code, house_code) == 0 && houses[i].is_available == 1) {
            found = 1;

            Booking new_booking;
            new_booking.id = booking_count + 1;
            strcpy(new_booking.house_code, house_code);
            printf(GREEN_COLOR "Enter your Customer ID: " RESET_COLOR);
            scanf("%d", &new_booking.customer_id);
            printf(GREEN_COLOR "Enter booking date (day month year): " RESET_COLOR);
            scanf("%d %d %d", &new_booking.date.day, &new_booking.date.month, &new_booking.date.year);
            strcpy(new_booking.status, "Pending");
            bookings[booking_count++] = new_booking;
            printf(GREEN_COLOR "Booking request sent! Waiting for manager approval.\n" RESET_COLOR);
            return;
        }
    }

    if (!found) {
        printf(RED_COLOR "House not available for booking.\n" RESET_COLOR);
    }
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
    load_houses_from_csv("Briefly_Info.csv");
    run_system();
    save_houses_to_csv("Briefly_Info.csv");
    return 0;
}

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
    char house_code[10];
    int customer_id;
    Date date;
    char status[20];
} Booking;

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
    char name[50];
    char address[100];
    char province[30];
    float price_per_night;
    float area;
    int beds;
    int bedrooms;
    int bathrooms;
    int max_guests;
    char facilities[150];
    char landmark[100];
    char transportation[100];
    char essential[100];
    float rating;
    int is_available;
} Property;


Property properties[10];
House houses[10];
Booking bookings[10];

int property_count = 0;
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
    if (property_count >= 10) {
        printf(RED_COLOR "Cannot add more properties. Maximum limit reached.\n" RESET_COLOR);
        return;
    }

    Property new_property;
    new_property.id = property_count + 1;  // Auto-generate ID

    printf(GREEN_COLOR "\nEnter house name: " RESET_COLOR);
    getchar(); // clear newline
    fgets(new_property.name, sizeof(new_property.name), stdin);
    new_property.name[strcspn(new_property.name, "\n")] = 0;

    printf(GREEN_COLOR "Enter house address (street, number, etc.): " RESET_COLOR);
    fgets(new_property.address, sizeof(new_property.address), stdin);
    new_property.address[strcspn(new_property.address, "\n")] = 0;

    printf(GREEN_COLOR "Enter house province: " RESET_COLOR);
    fgets(new_property.province, sizeof(new_property.province), stdin);
    new_property.province[strcspn(new_property.province, "\n")] = 0;

    printf(GREEN_COLOR "Enter price per night: " RESET_COLOR);
    scanf("%f", &new_property.price_per_night);

    printf(GREEN_COLOR "Enter area (in square meters): " RESET_COLOR);
    scanf("%f", &new_property.area);

    printf(GREEN_COLOR "Enter number of beds: " RESET_COLOR);
    scanf("%d", &new_property.beds);

    printf(GREEN_COLOR "Enter number of bedrooms: " RESET_COLOR);
    scanf("%d", &new_property.bedrooms);

    printf(GREEN_COLOR "Enter number of bathrooms: " RESET_COLOR);
    scanf("%d", &new_property.bathrooms);

    printf(GREEN_COLOR "Enter maximum number of guests: " RESET_COLOR);
    scanf("%d", &new_property.max_guests);

    getchar(); // clear newline again

    printf(GREEN_COLOR "Enter facilities (comma-separated, e.g., WiFi,Pool,Parking): " RESET_COLOR);
    fgets(new_property.facilities, sizeof(new_property.facilities), stdin);
    new_property.facilities[strcspn(new_property.facilities, "\n")] = 0;

    printf(GREEN_COLOR "Enter nearby landmarks (comma-separated): " RESET_COLOR);
    fgets(new_property.landmark, sizeof(new_property.landmark), stdin);
    new_property.landmark[strcspn(new_property.landmark, "\n")] = 0;

    printf(GREEN_COLOR "Enter transportation options (comma-separated): " RESET_COLOR);
    fgets(new_property.transportation, sizeof(new_property.transportation), stdin);
    new_property.transportation[strcspn(new_property.transportation, "\n")] = 0;

    printf(GREEN_COLOR "Enter essentials (comma-separated): " RESET_COLOR);
    fgets(new_property.essential, sizeof(new_property.essential), stdin);
    new_property.essential[strcspn(new_property.essential, "\n")] = 0;

    printf(GREEN_COLOR "Enter rating (1 to 10): " RESET_COLOR);
    scanf("%f", &new_property.rating);

    // Validation: Make sure rating is between 1 and 10
    while (new_property.rating < 1 || new_property.rating > 10) {
        printf(RED_COLOR "Invalid rating! Please enter a value between 1 and 10: " RESET_COLOR);
        scanf("%f", &new_property.rating);
    } 


    new_property.is_available = 1; // Default available

    properties[property_count] = new_property;
    property_count++;

    printf(BLUE_COLOR "\nHouse added successfully!\n" RESET_COLOR);
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
            case 2: /* Edit House */ break;
            case 3: /* Delete House */ break;
            case 4: /* Set Availability */ break;
            case 5: manager_view_all_houses(); break;
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

void customer_filter_advanced() {
    clear_screen();
    if (house_count == 0) {
        printf(RED_COLOR "No houses available!\n" RESET_COLOR);
        printf(YELLOW_COLOR "\nPress Enter to return to menu..." RESET_COLOR);
        getchar();
        getchar();
        return;
    }

    char province[50];
    float min_price, max_price;
    float min_rating, max_rating;
    int min_bedrooms;

    getchar(); // flush
    printf(GREEN_COLOR "Enter province to search (leave blank to skip): " RESET_COLOR);
    fgets(province, sizeof(province), stdin);
    province[strcspn(province, "\n")] = 0; // Remove newline

    printf(GREEN_COLOR "Enter minimum price (or 0 to skip): " RESET_COLOR);
    scanf("%f", &min_price);
    printf(GREEN_COLOR "Enter maximum price (or 0 to skip): " RESET_COLOR);
    scanf("%f", &max_price);

    printf(GREEN_COLOR "Enter minimum rating (or 0 to skip): " RESET_COLOR);
    scanf("%f", &min_rating);
    printf(GREEN_COLOR "Enter maximum rating (or 0 to skip): " RESET_COLOR);
    scanf("%f", &max_rating);

    printf(GREEN_COLOR "Enter minimum bedrooms (or 0 to skip): " RESET_COLOR);
    scanf("%d", &min_bedrooms);

    int found = 0;
    printf(BLUE_COLOR "\n========================\n");
    printf("     FILTERED RESULTS    \n");
    printf("========================\n" RESET_COLOR);

    for (int i = 0; i < house_count; i++) {
        if (!houses[i].is_available) continue; // Only available houses

        // Province check
        if (strlen(province) > 0 && strstr(houses[i].province, province) == NULL)
            continue;

        // Price check
        if ((min_price > 0 && houses[i].price < min_price) ||
            (max_price > 0 && houses[i].price > max_price))
            continue;

        // Rating check
        if ((min_rating > 0 && houses[i].rating < min_rating) ||
            (max_rating > 0 && houses[i].rating > max_rating))
            continue;

        // Bedrooms check
        if (min_bedrooms > 0 && houses[i].bedrooms < min_bedrooms)
            continue;

        // If passed all checks, print
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

        found = 1;
    }

    if (!found) {
        printf(RED_COLOR "\nNo houses match your criteria.\n" RESET_COLOR);
    }

    printf(YELLOW_COLOR "\nPress Enter to return to menu..." RESET_COLOR);
    getchar();
    getchar(); // Pause before return
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
        printf("3. Make Booking\n");
        printf("0. Back\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: customer_view_all_houses(); break;
            case 2: customer_filter_advanced(); break;
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
    load_houses_from_csv("Briefly_Info.csv");
    run_system();
    save_houses_to_csv("Briefly_Info.csv");
    return 0;
}

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
        sscanf(line, "%9[^,],%49[^,],%49[^,],%f,%f,%d,%d,%d,%d,%d",
               h.code, h.name, h.province, &h.price, &h.rating,
               &h.bedrooms, &h.beds, &h.bathrooms, &h.kitchens, &h.is_available);
        houses[house_count++] = h;
    }

    fclose(file);
    //printf(GREEN_COLOR "Loaded %d houses from CSV.\n" RESET_COLOR, house_count);
}

void favorite_house(int house_index) {
    FILE *file = fopen("favorites.csv", "a"); // Append mode
    if (!file) {
        printf(RED_COLOR "Failed to save favorite.\n" RESET_COLOR);
        return;
    }

    House h = houses[house_index];

    fprintf(file, "%s,%s,%s,%.2f,%.1f\n",
            h.code, h.name, h.province, h.price, h.rating);

    fclose(file);

    printf(GREEN_COLOR "House added to favorites!\n" RESET_COLOR);
    getchar(); getchar();  // Pause
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
            case 5: manager_view_all_houses(); break;
            case 6: /* View Bookings For a House */ break;
            case 7: /* Accept/Reject Booking */ break;
            case 0: return;
            default: printf(RED_COLOR "Invalid choice!\n" RESET_COLOR);
        }
    }
}

void customer_make_booking(const char *house_code) {
    Booking new_booking;
    new_booking.id = booking_count + 1;
    strcpy(new_booking.house_code, house_code);

    printf(GREEN_COLOR "Enter your Customer ID: " RESET_COLOR);
    scanf("%d", &new_booking.customer_id);

    printf(GREEN_COLOR "Enter booking date (DD MM YYYY): " RESET_COLOR);
    scanf("%d %d %d", &new_booking.date.day, &new_booking.date.month, &new_booking.date.year);

    strcpy(new_booking.status, "Pending");

    bookings[booking_count++] = new_booking;

    printf(GREEN_COLOR "Booking request submitted!\n" RESET_COLOR);
    getchar(); getchar();  // Pause
}

void customer_view_favorite_houses() {
    clear_screen();

    FILE *file = fopen("favorites.csv", "r");
    if (!file) {
        printf(RED_COLOR "No favorite houses found.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    char line[256];
    int fav_count = 0;
    House fav_houses[20]; // assuming max 20 favorites

    // Optional: Skip header if you add one later
    // fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        House h;
        sscanf(line, "%9[^,],%49[^,],%49[^,],%f,%f",
               h.code, h.name, h.province, &h.price, &h.rating);
        h.is_available = 1; // assume available by default
        fav_houses[fav_count++] = h;
    }
    fclose(file);

    if (fav_count == 0) {
        printf(RED_COLOR "No favorite houses found!\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    printf(BLUE_COLOR "========================\n");
    printf("     FAVORITE HOUSES     \n");
    printf("========================\n" RESET_COLOR);

    for (int i = 0; i < fav_count; i++) {
        printf(YELLOW_COLOR "\nFavorite #%d\n" RESET_COLOR, i + 1);
        printf(WHITE_COLOR "Code: " RESET_COLOR "%s\n", fav_houses[i].code);
        printf(WHITE_COLOR "Name: " RESET_COLOR "%s\n", fav_houses[i].name);
        printf(WHITE_COLOR "Province: " RESET_COLOR "%s\n", fav_houses[i].province);
        printf(WHITE_COLOR "Price: " RESET_COLOR "%.2f\n", fav_houses[i].price);
        printf(WHITE_COLOR "Rating: " RESET_COLOR "%.1f\n", fav_houses[i].rating);
    }

    printf(YELLOW_COLOR "\nEnter the number of the favorite house to book (0 to cancel): " RESET_COLOR);
    int selection;
    scanf("%d", &selection);

    if (selection == 0) {
        printf(GREEN_COLOR "Returning to menu.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    if (selection < 1 || selection > fav_count) {
        printf(RED_COLOR "Invalid selection!\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    // Proceed to booking this favorite house
    //customer_make_booking_from_fav(fav_houses[selection - 1]);
}

void customer_view_house_details(int house_index) {
    clear_screen();

    House h = houses[house_index];

    printf(BLUE_COLOR "========================\n");
    printf("       HOUSE DETAILS     \n");
    printf("========================\n" RESET_COLOR);

    printf(WHITE_COLOR "Code: " RESET_COLOR "%s\n", h.code);
    printf(WHITE_COLOR "Name: " RESET_COLOR "%s\n", h.name);
    printf(WHITE_COLOR "Province: " RESET_COLOR "%s\n", h.province);
    printf(WHITE_COLOR "Price: " RESET_COLOR "%.2f\n", h.price);
    printf(WHITE_COLOR "Rating: " RESET_COLOR "%.1f\n", h.rating);
    printf(WHITE_COLOR "Bedrooms: " RESET_COLOR "%d\n", h.bedrooms);
    printf(WHITE_COLOR "Beds: " RESET_COLOR "%d\n", h.beds);
    printf(WHITE_COLOR "Bathrooms: " RESET_COLOR "%d\n", h.bathrooms);
    printf(WHITE_COLOR "Kitchens: " RESET_COLOR "%d\n", h.kitchens);
    printf(WHITE_COLOR "Available: " RESET_COLOR "%s\n",
           h.is_available ? GREEN_COLOR "Yes" RESET_COLOR : RED_COLOR "No" RESET_COLOR);

    printf("\n%sWhat would you like to do?\n", YELLOW_COLOR);
    printf("1. Book this House\n");
    printf("2. Favorite this House\n");
    printf("0. Go back to list\n" RESET_COLOR);

    int action;
    printf("Enter your choice: ");
    scanf("%d", &action);

    switch (action) {
        case 1:
            if (!h.is_available) {
                printf(RED_COLOR "Sorry, this house is not available for booking.\n" RESET_COLOR);
            } else {
                //customer_make_hbooking(house_index);
            }
            break;
        case 2:
            favorite_house(house_index);
            break;
        case 0:
            return;
        default:
            printf(RED_COLOR "Invalid choice!\n" RESET_COLOR);
            getchar(); getchar();  // Pause
    }
}

void customer_view_all_houses() {
    clear_screen();
    if (house_count == 0) {
        printf(RED_COLOR "No houses available!\n" RESET_COLOR);
    } else {
        printf(BLUE_COLOR "========================\n");
        printf("     AVAILABLE HOUSES    \n");
        printf("========================\n" RESET_COLOR);

        for (int i = 0; i < house_count; i++) {
            printf(YELLOW_COLOR "\nHouse #%d\n" RESET_COLOR, i + 1);
            printf(WHITE_COLOR "Code: " RESET_COLOR "%s\n", houses[i].code);
            printf(WHITE_COLOR "Name: " RESET_COLOR "%s\n", houses[i].name);
            printf(WHITE_COLOR "Province: " RESET_COLOR "%s\n", houses[i].province);
            printf(WHITE_COLOR "Price: " RESET_COLOR "%.2f\n", houses[i].price);
            printf(WHITE_COLOR "Available: " RESET_COLOR "%s\n",
                   houses[i].is_available ? GREEN_COLOR "Yes" RESET_COLOR : RED_COLOR "No" RESET_COLOR);
        }
    }

    printf(YELLOW_COLOR "\nEnter the number of the house you want to view (0 to cancel): " RESET_COLOR);
    int selection;
    scanf("%d", &selection);

    if (selection == 0) {
        printf(GREEN_COLOR "Returning to menu.\n" RESET_COLOR);
        getchar(); getchar();  // Pause
        return;
    }

    if (selection < 1 || selection > house_count) {
        printf(RED_COLOR "Invalid selection!\n" RESET_COLOR);
        getchar(); getchar();  // Pause
        return;
    }

    customer_view_house_details(selection - 1);  // <-- New function here!
}

void customer_search_by_location() {
    printf("%sFunction: Search\n" RESET_COLOR);
}

void customer_filter_by_price() {
    printf("%sFunction: Filter\n" RESET_COLOR);
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
            case 4: /*customer_make_booking()*/; break;
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

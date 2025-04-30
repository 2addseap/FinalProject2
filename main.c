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

#define MAX_HOUSES 100

void clear_screen() {
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
    char code[10];  
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

typedef struct {
    char id[10];
    char name[100];
    char address[100];
    char province[50];
    float price;
    float area;
    int beds;
    int bedrooms;
    int bathrooms;
    int maxGuests;
    char facilities[100];
    char landmark[100];
    char transport[100];
    char essential[100];
    float rating;
} DetailedHouse;

Property properties[10];
House houses[MAX_HOUSES];
Booking bookings[100];
DetailedHouse details[100];

int detail_count = 0;
int property_count = 0;
int house_count = 0;
int booking_count = 0;

void save_houses_to_csv(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf(RED_COLOR "Failed to save house data.\n" RESET_COLOR);
        return;
    }

    fprintf(file, "code,name,province,price,rating,bedrooms,beds,bathrooms,kitchens,available\n");
    for (int i = 0; i < house_count; i++) {
        fprintf(file, "%s,%s,%s,%.2f,%.1f,%d,%d,%d,%d,%d\n",
                houses[i].code, houses[i].name, houses[i].province,
                houses[i].price, houses[i].rating,
                houses[i].bedrooms, houses[i].beds,
                houses[i].bathrooms, houses[i].kitchens,
                houses[i].is_available);
    }

    fclose(file);
    printf(GREEN_COLOR "Saved houses to CSV.\n" RESET_COLOR);
}

void load_houses_from_csv(const char *Briefly_Info) {
    //printf("Loading from CSV file: %s\n", Briefly_Info);
    FILE *file = fopen(Briefly_Info, "r");
    if (!file) {
        printf(RED_COLOR "Failed to open house CSV file.\n" RESET_COLOR);
        return;
    }

    char line[512];
    fgets(line, sizeof(line), file); // skip header

    house_count = 0;
    while (fgets(line, sizeof(line), file) && house_count < MAX_HOUSES) {
        House h;
        char *token = strtok(line, ",");
        if (!token) continue;
        strcpy(h.code, token);
        token = strtok(NULL, ",");
        strcpy(h.name, token ? token : "");
        token = strtok(NULL, ",");
        strcpy(h.province, token ? token : "");
        token = strtok(NULL, ",");
        h.price = token ? atof(token) : 0;
        token = strtok(NULL, ",");
        h.rating = token ? atof(token) : 0;
        token = strtok(NULL, ",");
        h.bedrooms = token ? atoi(token) : 0;
        token = strtok(NULL, ",");
        h.beds = token ? atoi(token) : 0;
        token = strtok(NULL, ",");
        h.bathrooms = token ? atoi(token) : 0;
        token = strtok(NULL, ",");
        h.kitchens = token ? atoi(token) : 0;
        token = strtok(NULL, ",");
        h.is_available = token ? atoi(token) : 1;  // default to 1 if missing

        // printf("DEBUG LOAD: [%s] [%s] [%s] %.2f %.1f %d %d %d %d %d\n",
        //     h.code, h.name, h.province, h.price, h.rating,
        //     h.bedrooms, h.beds, h.bathrooms, h.kitchens, h.is_available);
     
        houses[house_count++] = h;
    }

    fclose(file);
}

// Reads a field from CSV, respecting quoted commas
char *parse_csv_field(char **line_ptr) {
    char *line = *line_ptr;
    static char buffer[256];
    int i = 0;
    int in_quotes = 0;

    while (*line) {
        if (*line == '"' && !in_quotes) {
            in_quotes = 1;
            line++;
            continue;
        }
        if (*line == '"' && in_quotes && line[1] == ',') {
            line += 2;  // Skip quote and comma
            break;
        }
        if (*line == '"' && in_quotes && line[1] == '\0') {
            line++;
            break;
        }
        if (*line == ',' && !in_quotes) {
            line++;  // Skip comma
            break;
        }
        buffer[i++] = *line++;
    }
    buffer[i] = '\0';
    *line_ptr = line;
    return buffer;
}

void load_detailed_houses_from_csv(const char *Detail) {
    FILE *file = fopen(Detail, "r");
    if (!file) {
        printf(RED_COLOR "Failed to open detailed house file.\n" RESET_COLOR);
        return;
    }

    char line[1024];
    fgets(line, sizeof(line), file); // skip header

    detail_count = 0;
    while (fgets(line, sizeof(line), file) && detail_count < 100) {
        char *ptr = line;
        DetailedHouse d;

        strcpy(d.id, parse_csv_field(&ptr));
        strcpy(d.name, parse_csv_field(&ptr));
        strcpy(d.address, parse_csv_field(&ptr));
        strcpy(d.province, parse_csv_field(&ptr));
        d.price = atof(parse_csv_field(&ptr));
        d.area = atof(parse_csv_field(&ptr));
        d.beds = atoi(parse_csv_field(&ptr));
        d.bedrooms = atoi(parse_csv_field(&ptr));
        d.bathrooms = atoi(parse_csv_field(&ptr));
        d.maxGuests = atoi(parse_csv_field(&ptr));
        strcpy(d.facilities, parse_csv_field(&ptr));
        strcpy(d.landmark, parse_csv_field(&ptr));
        strcpy(d.transport, parse_csv_field(&ptr));
        strcpy(d.essential, parse_csv_field(&ptr));
        d.rating = atof(parse_csv_field(&ptr));

        details[detail_count++] = d;
    }

    fclose(file);
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

void customer_booking_page(const char *house_code) {
    clear_screen();

    // Find the house from code
    House *selected_house = NULL;
    for (int i = 0; i < house_count; i++) {
        if (strcmp(houses[i].code, house_code) == 0) {
            selected_house = &houses[i];
            break;
        }
    }

    if (selected_house == NULL) {
        printf(RED_COLOR "House not found!\n" RESET_COLOR);
        printf(YELLOW_COLOR "\nPress Enter to return..." RESET_COLOR);
        while (getchar() != '\n');
        getchar();
        return;
    }

    printf(BLUE_COLOR "========================\n");
    printf("         BOOKING PAGE     \n");
    printf("========================\n" RESET_COLOR);

    printf(WHITE_COLOR "Name: " RESET_COLOR "%s\n", selected_house->name);
    printf(WHITE_COLOR "Province: " RESET_COLOR "%s\n", selected_house->province);
    printf(WHITE_COLOR "Price: " RESET_COLOR "%.2f\n", selected_house->price);
    printf(WHITE_COLOR "Rating: " RESET_COLOR "%.1f\n", selected_house->rating);
    printf(WHITE_COLOR "Bedrooms: " RESET_COLOR "%d\n", selected_house->bedrooms);
    printf(WHITE_COLOR "Beds: " RESET_COLOR "%d\n", selected_house->beds);
    printf(WHITE_COLOR "Bathrooms: " RESET_COLOR "%d\n", selected_house->bathrooms);
    printf(WHITE_COLOR "Kitchens: " RESET_COLOR "%d\n", selected_house->kitchens);
    printf(WHITE_COLOR "Available: " RESET_COLOR "%s\n",
           selected_house->is_available ? GREEN_COLOR "Yes" RESET_COLOR : RED_COLOR "No" RESET_COLOR);

    printf("\nPlease enter your booking details:\n" YELLOW_COLOR);
    printf("----------------------------------\n" RESET_COLOR);

    Booking new_booking;
    new_booking.id = booking_count + 1;
    strcpy(new_booking.house_code, house_code);

    printf(GREEN_COLOR "Enter your Customer ID: " RESET_COLOR);
    scanf("%d", &new_booking.customer_id);

    printf(GREEN_COLOR "Enter booking date (DD MM YYYY): " RESET_COLOR);
    scanf("%d %d %d", &new_booking.date.day, &new_booking.date.month, &new_booking.date.year);

    strcpy(new_booking.status, "Pending");

    bookings[booking_count++] = new_booking;

    printf(GREEN_COLOR "\nBooking request submitted successfully!\n" RESET_COLOR);

    // Flush and pause
    while (getchar() != '\n');
    printf(YELLOW_COLOR "Press Enter to return to menu..." RESET_COLOR);
    getchar();
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
    customer_booking_page(fav_houses[selection - 1].code);
}

void customer_view_house_details(int house_index) {
    clear_screen();

    House h = houses[house_index];

    printf(BLUE_COLOR "========================\n");
    printf("       HOUSE DETAILS     \n");
    printf("========================\n" RESET_COLOR);

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

    // Embed detail info here:
    for (int i = 0; i < detail_count; i++) {
        if (strcmp(details[i].id, h.code) == 0) {
            printf(WHITE_COLOR "Address: " RESET_COLOR "%s\n", details[i].address);
            printf(WHITE_COLOR "Area: " RESET_COLOR "%.2f sqm\n", details[i].area);
            printf(WHITE_COLOR "Max Guests: " RESET_COLOR "%d\n", details[i].maxGuests);
            printf(WHITE_COLOR "Facilities: " RESET_COLOR "%s\n", details[i].facilities);
            printf(WHITE_COLOR "Landmark: " RESET_COLOR "%s\n", details[i].landmark);
            printf(WHITE_COLOR "Transport: " RESET_COLOR "%s\n", details[i].transport);
            printf(WHITE_COLOR "Essential Info: " RESET_COLOR "%s\n", details[i].essential);
            break;
        }
    }

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
                customer_booking_page(h.code);
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

void customer_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n%s--------------------------------------------------\n", YELLOW_COLOR);
        printf("                   CUSTOMER MENU                  \n");
        printf("--------------------------------------------------\n" RESET_COLOR);
        printf("1. View All Houses\n");
        printf("2. Search House\n");
        printf("3. View Favorite Houses\n");
        printf("4. Make Booking\n");
        printf("0. Back\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: customer_view_all_houses(); break;
            case 2: customer_filter_advanced(); break;
            case 3: customer_view_favorite_houses(); break;
            case 4: //customer_make_booking(); break;
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
    load_detailed_houses_from_csv("Detail.csv");
    run_system();
    save_houses_to_csv("Briefly_Info.csv");
    return 0;
}

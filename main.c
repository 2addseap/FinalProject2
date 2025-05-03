
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

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
    char code[10];
    char date[20];     // "YYYY-MM-DD"
    char status[20];   // "Available", "Booked", "Unavailable"
} CalendarEntry;

CalendarEntry calendar[3000];  // enough for 100 houses × 30 days
int calendar_count = 0;


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
    char code[10];
    char name[50];
    char address[100];
    char province[30];
    float price;
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
    char code[10];
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

void load_properties_from_csv(const char *filename);
void load_houses_for_customer(const char *filename);
void load_calendar(const char *filename);
void manager_set_availability();
void generate_calendar_for_new_house(const char *house_code, int days_ahead);
int parse_date(const char *date_str, struct tm *tm_out);



//load data from calendar.csv
void load_calendar(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf(RED_COLOR "Calendar.csv not found.\n" RESET_COLOR);
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip header

    calendar_count = 0;
    while (fgets(line, sizeof(line), file) && calendar_count < 1000) {
        CalendarEntry c;
        sscanf(line, "%9[^,],%19[^,],%14s", c.code, c.date, c.status);
        to_uppercase(c.code);  // ADD THIS LINE
        calendar[calendar_count++] = c;

    }

    fclose(file);
}


void save_houses_to_csv(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf(RED_COLOR "Failed to save house data.\n" RESET_COLOR);
        return;
    }

    fprintf(file, "Code,Name,Province,Price,Rating,Bedrooms,Beds,Bathrooms,Kitchens,Availability\n");
    
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

void save_properties_to_csv(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf(RED_COLOR "Failed to save property data.\n" RESET_COLOR);
        return;
    }

    fprintf(file, "Code,ID,Name,Address,Province,Price,Area,Beds,Bedrooms,Bathrooms,MaxGuests,Facilities,Landmark,Transport,Essential,Rating\n");

    for (int i = 0; i < property_count; i++) {
        Property p = properties[i];
        fprintf(file, "%s,%d,%s,%s,%s,%.2f,%.2f,%d,%d,%d,%d,\"%s\",\"%s\",\"%s\",\"%s\",%.1f\n",
            properties[i].code, properties[i].id, properties[i].name, properties[i].address, properties[i].province,
            properties[i].price, properties[i].area, properties[i].beds, properties[i].bedrooms,
            properties[i].bathrooms, properties[i].max_guests,
            properties[i].facilities, properties[i].landmark,
            properties[i].transportation, properties[i].essential,
            properties[i].rating);
        
    }

    fclose(file);
}

void load_houses_for_customer(const char *Briefly_Info)
 {
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

void load_properties_from_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf(RED_COLOR "Failed to open %s\n" RESET_COLOR, filename);
        return;
    }

    char line[1024];
    fgets(line, sizeof(line), file);  // Skip header line

    property_count = 0;
    while (fgets(line, sizeof(line), file)) {
        Property p;

        // Corrected: Now Code is the first field!
        int fields_read = sscanf(line, "%9[^,],%d,%49[^,],%99[^,],%29[^,],%f,%f,%d,%d,%d,%d,\"%149[^\"]\",\"%99[^\"]\",\"%99[^\"]\",\"%99[^\"]\",%f",
            p.code, &p.id, p.name, p.address, p.province,
            &p.price, &p.area,
            &p.beds, &p.bedrooms, &p.bathrooms, &p.max_guests,
            p.facilities, p.landmark, p.transportation, p.essential,
            &p.rating);

        if (fields_read == 16) {  // Now expect 16 fields
            p.is_available = 1;  // Default available
            properties[property_count++] = p;
        } else {
            //printf(RED_COLOR "Warning: Skipped malformed line in Detail.csv\n" RESET_COLOR);
        }
    }

    fclose(file);
}

void load_houses_for_manager() {
    // Load houses from Briefly_Info.csv
    FILE *file = fopen("Briefly_Info.csv", "r");
    if (!file) {
        printf(RED_COLOR "Failed to open Briefly_Info.csv\n" RESET_COLOR);
        return;
    }

    char line[512];
    fgets(line, sizeof(line), file); // skip header
    house_count = 0;

    while (fgets(line, sizeof(line), file) && house_count < MAX_HOUSES) {
        House h;
        int fields = sscanf(line, "%9[^,],%49[^,],%49[^,],%f,%f,%d,%d,%d,%d,%d",
            h.code, h.name, h.province,
            &h.price, &h.rating,
            &h.bedrooms, &h.beds, &h.bathrooms, &h.kitchens, &h.is_available);

        if (fields == 10) {
            houses[house_count++] = h;
        } else {
            printf(RED_COLOR "Warning: Skipped malformed line in Briefly_Info.csv\n" RESET_COLOR);
        }
    }

    fclose(file);

    // Also load property details
    load_properties_from_csv("Detail.csv");
}

void sync_house_availability_from_calendar() {
    for (int i = 0; i < house_count; i++) {
        int has_available = 0;
        for (int j = 0; j < calendar_count; j++) {
            if (strcmp(calendar[j].code, houses[i].code) == 0 &&
                strcmp(calendar[j].status, "Available") == 0) {
                has_available = 1;
                break;
            }
        }
        houses[i].is_available = has_available;
    }
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

int parse_date(const char *date_str, struct tm *tm_out) {
    memset(tm_out, 0, sizeof(struct tm));
    int year, month, day;
    if (sscanf(date_str, "%d-%d-%d", &year, &month, &day) == 3) {
        tm_out->tm_year = year - 1900;
        tm_out->tm_mon = month - 1;
        tm_out->tm_mday = day;
        return 1;
    }
    return 0;
}


void load_detailed_houses_from_csv(const char *Detail) {
    FILE *file = fopen(Detail, "r");
    if (!file) {
        printf(RED_COLOR "Failed to open detailed house file.\n" RESET_COLOR);
        return;
    }

    //printf(GREEN_COLOR "Loading Detailed Houses...\n" RESET_COLOR); // Debug print

    char line[1024];
    fgets(line, sizeof(line), file); // skip header

    detail_count = 0;
    while (fgets(line, sizeof(line), file) && detail_count < 100) {
        char *ptr = line;
        DetailedHouse d;

        char *fields[16];
        int i = 0;
        while (*ptr && i < 16) {
            fields[i++] = strdup(parse_csv_field(&ptr));
        }

        if (i < 16) {
            //printf(RED_COLOR "Malformed line (only %d fields): %s\n" RESET_COLOR, i, line);
            for (int j = 0; j < i; j++) free(fields[j]);
            continue;
        }
        strcpy(d.code, fields[0]);
        strcpy(d.id, fields[1]);
        strcpy(d.name, fields[2]);
        strcpy(d.address, fields[3]);
        strcpy(d.province, fields[4]);
        d.price = atof(fields[5]);
        d.area = atof(fields[6]);
        d.beds = atoi(fields[7]);
        d.bedrooms = atoi(fields[8]);
        d.bathrooms = atoi(fields[9]);
        d.maxGuests = atoi(fields[10]);
        strcpy(d.facilities, fields[11]);
        strcpy(d.landmark, fields[12]);
        strcpy(d.transport, fields[13]);
        strcpy(d.essential, fields[14]);
        d.rating = atof(fields[15]);

        for (int j = 0; j < 16; j++) free(fields[j]);  // Clean up

        details[detail_count++] = d;
    }

    fclose(file);
    //printf(GREEN_COLOR "Detailed houses loaded successfully: %d items\n" RESET_COLOR, detail_count);  // Debug print
}

void append_booking_to_csv(
    const char *filename,
    const char *full_name,
    const char *phone,
    int guests,
    Date checkin,
    Date checkout,
    int nights,
    House *h
) {
    FILE *file = fopen(filename, "a+");  // Open for append and read
    if (!file) {
        printf(RED_COLOR "Failed to open booking history file.\n" RESET_COLOR);
        return;
    }

    // Check if file is empty to write the header
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size == 0) {
        fprintf(file, "Fullname,Phone,Guests,CheckIn,CheckOut,Nights,Code,Name,Province,Price,Rating\n");
    }

    // Write booking entry
    fprintf(file, "\"%s\",\"%s\",%d,%02d/%02d/%04d,%02d/%02d/%04d,%d,%s,\"%s\",\"%s\",%.2f,%.1f\n",
            full_name, phone, guests,
            checkin.day, checkin.month, checkin.year,
            checkout.day, checkout.month, checkout.year,
            nights,
            h->code, h->name, h->province, h->price, h->rating);

    fclose(file);
}

int favorite_house(int house_index) {
    House h = houses[house_index];

    FILE *check = fopen("favorites.csv", "r");
    if (check) {
        char line[256];
        while (fgets(line, sizeof(line), check)) {
            char code[10];
            sscanf(line, "%9[^,]", code);
            if (strcmp(code, h.code) == 0) {
                fclose(check);
                printf(RED_COLOR "This house is already in your favorites list.\n" RESET_COLOR);
                getchar(); getchar();
                return 0;  // Already in favorites
            }
        }
        fclose(check);
    }

    FILE *file = fopen("favorites.csv", "a");
    if (!file) {
        printf(RED_COLOR "Failed to save favorite.\n" RESET_COLOR);
        return 0;
    }

    fprintf(file, "%s,%s,%s,%.2f,%.1f\n",
            h.code, h.name, h.province, h.price, h.rating);

    fclose(file);

    printf(GREEN_COLOR "House added to favorites!\n" RESET_COLOR);
    getchar(); getchar();
    return 1;  // Successfully added
}

// Add house function

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

void manager_add_house() {
    if (property_count >= 100 || house_count >= 100) {
        printf(RED_COLOR "Cannot add more houses. Maximum limit reached.\n" RESET_COLOR);
        return;
    }

    Property new_property;
    new_property.id = property_count + 1;

    printf(GREEN_COLOR "\nEnter house name: " RESET_COLOR);
    getchar();  // flush newline
    fgets(new_property.name, sizeof(new_property.name), stdin);
    new_property.name[strcspn(new_property.name, "\n")] = 0;

    printf(GREEN_COLOR "Enter house address: " RESET_COLOR);
    fgets(new_property.address, sizeof(new_property.address), stdin);
    new_property.address[strcspn(new_property.address, "\n")] = 0;

    printf(GREEN_COLOR "Enter house province (Huahin/Cha-am, Pattaya, Kanchanaburi): " RESET_COLOR);
    fgets(new_property.province, sizeof(new_property.province), stdin);
    new_property.province[strcspn(new_property.province, "\n")] = 0;

    // Normalize province to lowercase for matching
    char province_lower[50];
    strcpy(province_lower, new_property.province);
    to_lowercase(province_lower);

    // Fix display casing for province
    if (strcmp(province_lower, "huahin/cha-am") == 0)
        strcpy(new_property.province, "Huahin/Cha-am");
    else if (strcmp(province_lower, "pattaya") == 0)
        strcpy(new_property.province, "Pattaya");
    else if (strcmp(province_lower, "kanchanaburi") == 0)
        strcpy(new_property.province, "Kanchanaburi");
    else
        strcpy(new_property.province, "Unknown");

    printf(GREEN_COLOR "Enter price: " RESET_COLOR);
    scanf("%f", &new_property.price);

    printf(GREEN_COLOR "Enter area (sqm): " RESET_COLOR);
    scanf("%f", &new_property.area);

    printf(GREEN_COLOR "Enter number of beds: " RESET_COLOR);
    scanf("%d", &new_property.beds);

    printf(GREEN_COLOR "Enter number of bedrooms: " RESET_COLOR);
    scanf("%d", &new_property.bedrooms);

    printf(GREEN_COLOR "Enter number of bathrooms: " RESET_COLOR);
    scanf("%d", &new_property.bathrooms);

    printf(GREEN_COLOR "Enter max number of guests: " RESET_COLOR);
    scanf("%d", &new_property.max_guests);
    getchar();

    printf(GREEN_COLOR "Enter facilities: " RESET_COLOR);
    fgets(new_property.facilities, sizeof(new_property.facilities), stdin);
    new_property.facilities[strcspn(new_property.facilities, "\n")] = 0;

    printf(GREEN_COLOR "Enter landmark: " RESET_COLOR);
    fgets(new_property.landmark, sizeof(new_property.landmark), stdin);
    new_property.landmark[strcspn(new_property.landmark, "\n")] = 0;

    printf(GREEN_COLOR "Enter transportation: " RESET_COLOR);
    fgets(new_property.transportation, sizeof(new_property.transportation), stdin);
    new_property.transportation[strcspn(new_property.transportation, "\n")] = 0;

    printf(GREEN_COLOR "Enter essentials: " RESET_COLOR);
    fgets(new_property.essential, sizeof(new_property.essential), stdin);
    new_property.essential[strcspn(new_property.essential, "\n")] = 0;

    while (1) {
        printf(GREEN_COLOR "Enter rating (1.0 - 10.0): " RESET_COLOR);
        int result = scanf("%f", &new_property.rating);
        while (getchar() != '\n'); // flush the input buffer
    
        if (result == 1 && new_property.rating >= 1.0 && new_property.rating <= 10.0) {
            break;
        } else {
            printf(RED_COLOR "Invalid. Please enter a number between 1.0 and 10.0.\n" RESET_COLOR);
        }
    }
    

    // Determine prefix for house code
    char prefix[4];
    if (strcmp(province_lower, "huahin/cha-am") == 0)
        strcpy(prefix, "HH");
    else if (strcmp(province_lower, "pattaya") == 0)
        strcpy(prefix, "P");
    else if (strcmp(province_lower, "kanchanaburi") == 0)
        strcpy(prefix, "K");
    else
        strcpy(prefix, "U"); // Unknown fallback

    int max_suffix = -1;
    for (int i = 0; i < house_count; i++) {
        if (strncmp(houses[i].code, prefix, strlen(prefix)) == 0) {
            int suffix = atoi(houses[i].code + strlen(prefix));
            if (suffix > max_suffix) {
                max_suffix = suffix;
            }
        }
    }
    int next_number = max_suffix + 1;

    House new_house;
    snprintf(new_house.code, sizeof(new_house.code), "%s%d", prefix, next_number);
    strcpy(new_house.name, new_property.name);
    strcpy(new_house.province, new_property.province);
    new_house.price = new_property.price;
    new_house.rating = new_property.rating;
    new_house.bedrooms = new_property.bedrooms;
    new_house.beds = new_property.beds;
    new_house.bathrooms = new_property.bathrooms;
    new_house.kitchens = 1;
    new_house.is_available = 1;
    houses[house_count++] = new_house;

    // Save code to property too
    strcpy(new_property.code, new_house.code);
    new_property.is_available = 1;
    properties[property_count++] = new_property;

    // === Append to Detail.csv ===
    FILE *detailFile = fopen("Detail.csv", "a");
    if (detailFile) {
        fprintf(detailFile, "%s,%d,%s,%s,%s,%.0f,%.0f,%d,%d,%d,%d,\"%s\",\"%s\",\"%s\",\"%s\",%.1f\n",
            new_house.code, new_property.id, new_property.name, new_property.address, new_property.province,
            new_property.price, new_property.area, new_property.beds, new_property.bedrooms,
            new_property.bathrooms, new_property.max_guests,
            new_property.facilities, new_property.landmark, new_property.transportation,
            new_property.essential, new_property.rating);
        fclose(detailFile);
    } else {
        printf(RED_COLOR "Failed to open Detail.csv!\n" RESET_COLOR);
    }

    // === Append to Briefly_Info.csv ===
    FILE *briefFile = fopen("Briefly_Info.csv", "a");
    if (briefFile) {
        fprintf(briefFile, "%s,%s,%s,%.0f,%.1f,%d,%d,%d,%d,%d\n",
            new_house.code, new_house.name, new_house.province,
            new_house.price, new_house.rating, new_house.bedrooms,
            new_house.beds, new_house.bathrooms, new_house.kitchens,
            new_house.is_available);
        fclose(briefFile);
    } else {
        printf(RED_COLOR "Failed to open Briefly_Info.csv!\n" RESET_COLOR);
    }

    // ✅ Generate calendar for next 30 days
    generate_calendar_for_new_house(new_house.code, 30);

    printf(BLUE_COLOR "\nHouse successfully added with code: %s\n" RESET_COLOR, new_house.code);
    getchar(); // pause
}

// Edit house function

void manager_edit_house() {
    char target_code[10];
    printf(GREEN_COLOR "Enter the house code to edit (e.g. HH1, P3): " RESET_COLOR);
    getchar(); // flush newline
    fgets(target_code, sizeof(target_code), stdin);
    target_code[strcspn(target_code, "\n")] = 0;

    int house_index = -1;
    for (int i = 0; i < house_count; i++) {
        if (strcmp(houses[i].code, target_code) == 0) {
            house_index = i;
            break;
        }
    }

    if (house_index == -1) {
        printf(RED_COLOR "House code not found in summary (Briefly_Info.csv).\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    int property_index = -1;
    for (int i = 0; i < property_count; i++) {
        if (strcmp(properties[i].code, houses[house_index].code) == 0) {
            property_index = i;
            break;
        }
    }
    

    if (property_index == -1) {
        printf(RED_COLOR "Matching house not found in Detail.csv\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    Property *p = &properties[property_index];
    House *h = &houses[house_index];
    char buffer[256];

    printf(GREEN_COLOR "\nEditing house: %s (%s)\n" RESET_COLOR, h->code, p->name);

    getchar(); // clear newline
    printf("Current name: %s\nNew name: ", p->name);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(p->name, buffer);
        strcpy(h->name, buffer);
    }

    printf("Current address: %s\nNew address: ", p->address);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(p->address, buffer);
    }

    printf("Current province: %s\nNew province: ", p->province);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(p->province, buffer);
        strcpy(h->province, buffer);
    }

    printf("Current price: %.2f\nNew price: ", p->price);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        sscanf(buffer, "%f", &p->price);
        h->price = p->price;
    }

    printf("Current area: %.2f\nNew area: ", p->area);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') sscanf(buffer, "%f", &p->area);

    printf("Current beds: %d\nNew beds: ", p->beds);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        sscanf(buffer, "%d", &p->beds);
        h->beds = p->beds;
    }

    printf("Current bedrooms: %d\nNew bedrooms: ", p->bedrooms);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        sscanf(buffer, "%d", &p->bedrooms);
        h->bedrooms = p->bedrooms;
    }

    printf("Current bathrooms: %d\nNew bathrooms: ", p->bathrooms);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        sscanf(buffer, "%d", &p->bathrooms);
        h->bathrooms = p->bathrooms;
    }

    printf("Current max guests: %d\nNew max guests: ", p->max_guests);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') sscanf(buffer, "%d", &p->max_guests);

    printf("Current facilities: %s\nNew facilities: ", p->facilities);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(p->facilities, buffer);
    }

    printf("Current landmark: %s\nNew landmark: ", p->landmark);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(p->landmark, buffer);
    }

    printf("Current transportation: %s\nNew transportation: ", p->transportation);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(p->transportation, buffer);
    }

    printf("Current essentials: %s\nNew essentials: ", p->essential);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(p->essential, buffer);
    }

    printf("Current rating: %.1f\nNew rating (1 - 10): ", p->rating);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        float r;
        sscanf(buffer, "%f", &r);
        if (r >= 1 && r <= 10) {
            p->rating = r;
            h->rating = r;
        }
    }

    printf(GREEN_COLOR "\nHouse updated successfully.\n" RESET_COLOR);

    // Save Briefly_Info.csv
    FILE *brief = fopen("Briefly_Info.csv", "w");
    if (brief) {
        fprintf(brief, "Code,Name,Province,Price,Rating,Bedroom,Bed,Bathroom,Kitchen,Availability\n");
        for (int i = 0; i < house_count; i++) {
            fprintf(brief, "%s,%s,%s,%.0f,%.1f,%d,%d,%d,%d,%d\n",
                houses[i].code, houses[i].name, houses[i].province,
                houses[i].price, houses[i].rating,
                houses[i].bedrooms, houses[i].beds,
                houses[i].bathrooms, houses[i].kitchens,
                houses[i].is_available);
        }
        fclose(brief);
    }

    // Save Detail.csv
FILE *detail = fopen("Detail.csv", "w");
if (detail) {
    fprintf(detail, "Code,ID,Name,Address,Province,Price,Area,Beds,Bedrooms,Bathrooms,MaxGuests,Facilities,Landmark,Transport,Essential,Rating\n");
    for (int i = 0; i < property_count; i++) {
        fprintf(detail, "\"%s\",%d,\"%s\",\"%s\",\"%s\",%.0f,%.0f,%d,%d,%d,%d,\"%s\",\"%s\",\"%s\",\"%s\",%.1f\n",
            properties[i].code, properties[i].id, properties[i].name, properties[i].address, properties[i].province,
            properties[i].price, properties[i].area, properties[i].beds, properties[i].bedrooms,
            properties[i].bathrooms, properties[i].max_guests,
            properties[i].facilities, properties[i].landmark,
            properties[i].transportation, properties[i].essential,
            properties[i].rating);              
    }
    fclose(detail);
}


    printf(YELLOW_COLOR "\nChanges saved to both CSV files.\n" RESET_COLOR);
    getchar(); // pause
}

// Call this inside manager_add_house() after adding the new house
void generate_calendar_for_new_house(const char *house_code, int days_ahead) {
    FILE *file = fopen("Calendar.csv", "a"); // append mode
    if (!file) {
        printf(RED_COLOR "Failed to open Calendar.csv\n" RESET_COLOR);
        return;
    }

    // Write header if file is new
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fprintf(file, "Code,Date,Status\n");
    }

    time_t now = time(NULL);
    struct tm date = *localtime(&now);
    char date_str[20];

    for (int i = 0; i < days_ahead; i++) {
        date.tm_mday += 1;
        mktime(&date);  // normalize the date (e.g. handle overflow to next month)
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", &date);
        fprintf(file, "%s,%s,Available\n", house_code, date_str);
    }

    fclose(file);
}

void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]);
}

// Step 1: Function to delete house
void manager_delete_house() {
    clear_screen();
    char target_code[10];
    printf(GREEN_COLOR "Enter the house code to delete (e.g. HH1): " RESET_COLOR);
    scanf("%s", target_code);
    to_uppercase(target_code);

    // Find house index
    int house_index = -1;
    for (int i = 0; i < house_count; i++) {
        if (strcmp(houses[i].code, target_code) == 0) {
            house_index = i;
            break;
        }
    }

    if (house_index == -1) {
        printf(RED_COLOR "House not found.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    // Warn if house is booked
    for (int i = 0; i < calendar_count; i++) {
        if (strcmp(calendar[i].code, target_code) == 0 && strcmp(calendar[i].status, "Booked") == 0) {
            printf(YELLOW_COLOR "Warning: This house has booked dates!\n" RESET_COLOR);
            break;
        }
    }

    // Confirm deletion
    printf(YELLOW_COLOR "Are you sure you want to delete house %s - \"%s\"? (Y/N): " RESET_COLOR,
           houses[house_index].code, houses[house_index].name);
    char confirm;
    getchar();
    scanf("%c", &confirm);
    if (toupper(confirm) != 'Y') {
        printf(RED_COLOR "Deletion cancelled.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    // Remove from houses[]
    for (int i = house_index; i < house_count - 1; i++) {
        houses[i] = houses[i + 1];
    }
    house_count--;

    // Remove from properties[]
    int prop_index = -1;
    for (int i = 0; i < property_count; i++) {
        if (strcmp(properties[i].code, target_code) == 0) {
            prop_index = i;
            break;
        }
    }
    if (prop_index != -1) {
        for (int i = prop_index; i < property_count - 1; i++) {
            properties[i] = properties[i + 1];
        }
        property_count--;
    }

    // Remove from calendar[]
    int new_calendar_count = 0;
    for (int i = 0; i < calendar_count; i++) {
        if (strcmp(calendar[i].code, target_code) != 0) {
            calendar[new_calendar_count++] = calendar[i];
        }
    }
    calendar_count = new_calendar_count;

    // Re-save all affected CSV files
    save_houses_to_csv("Briefly_Info.csv");
    save_properties_to_csv("Detail.csv");

    FILE *cal_file = fopen("Calendar.csv", "w");
    if (cal_file) {
        fprintf(cal_file, "Code,Date,Status\n");
        for (int i = 0; i < calendar_count; i++) {
            fprintf(cal_file, "%s,%s,%s\n", calendar[i].code, calendar[i].date, calendar[i].status);
        }
        fclose(cal_file);
    }

    printf(GREEN_COLOR "House successfully deleted.\n" RESET_COLOR);
    getchar(); getchar();
}


void manager_set_availability() {
    clear_screen();
    char house_code[10];
    printf(GREEN_COLOR "Enter House Code to manage availability (e.g. HH1): " RESET_COLOR);
    scanf("%s", house_code);
    to_uppercase(house_code);  // Normalize for consistency

    // Validate house code
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

    int choice;
    do {
        clear_screen();
        printf(BLUE_COLOR "===== SET AVAILABILITY FOR %s =====\n" RESET_COLOR, house_code);
        printf("1. View Calendar\n");
        printf("2. Block Date Range\n");
        printf("3. Unblock Date Range\n");
        printf("0. Return\n");
        printf(YELLOW_COLOR "Select an option: " RESET_COLOR);
        scanf("%d", &choice);

        if (choice == 1) {
            printf(BLUE_COLOR "\nAvailability for %s:\n" RESET_COLOR, house_code);
            for (int i = 0; i < calendar_count; i++) {
                if (strcmp(calendar[i].code, house_code) == 0) {
                    printf("Date: %s - Status: %s\n", calendar[i].date, calendar[i].status);
                }
            }
            printf(YELLOW_COLOR "\nPress Enter to return..." RESET_COLOR);
            getchar(); getchar();

        } else if (choice == 2 || choice == 3) {
            char start_date[20], end_date[20];
            char reason[100] = "BLOCKED";

            getchar(); // flush
            printf(GREEN_COLOR "Enter start date (YYYY-MM-DD): " RESET_COLOR);
            fgets(start_date, sizeof(start_date), stdin);
            start_date[strcspn(start_date, "\n")] = 0;

            printf(GREEN_COLOR "Enter end date (YYYY-MM-DD): " RESET_COLOR);
            fgets(end_date, sizeof(end_date), stdin);
            end_date[strcspn(end_date, "\n")] = 0;

            if (choice == 2) {
                printf(GREEN_COLOR "Enter reason (e.g. MAINTENANCE): " RESET_COLOR);
                fgets(reason, sizeof(reason), stdin);
                reason[strcspn(reason, "\n")] = 0;
                to_uppercase(reason);  // Normalize reason
            }
            else if (choice == 3) {
                printf(YELLOW_COLOR "Are you sure you want to unblock this whole range? (y/n): " RESET_COLOR);
                char confirm;
                scanf(" %c", &confirm);
                if (tolower(confirm) != 'y') {
                    printf(RED_COLOR "Unblock canceled.\n" RESET_COLOR);
                    getchar(); getchar();
                    continue;
                }
            }

            struct tm start_tm = {0}, end_tm = {0};
            if (!parse_date(start_date, &start_tm) || !parse_date(end_date, &end_tm)) {
                printf(RED_COLOR "Invalid date format. Please use YYYY-MM-DD.\n" RESET_COLOR);
                getchar(); getchar();
                continue;
            }

            time_t start_time = mktime(&start_tm);
            time_t end_time = mktime(&end_tm);
            if (difftime(end_time, start_time) < 0) {
                printf(RED_COLOR "End date must be after or equal to start date.\n" RESET_COLOR);
                getchar(); getchar();
                continue;
            }

            int modified = 0;
            for (time_t t = start_time; t <= end_time; t += 86400) {
                struct tm current_tm = *localtime(&t);
                char date_str[20];
                strftime(date_str, sizeof(date_str), "%Y-%m-%d", &current_tm);

                for (int i = 0; i < calendar_count; i++) {
                    if (strcmp(calendar[i].code, house_code) == 0 && strcmp(calendar[i].date, date_str) == 0) {
                        if (choice == 2) {
                            if (strcmp(calendar[i].status, "Booked") == 0) {
                                printf(RED_COLOR "Cannot block date %s - already booked.\n" RESET_COLOR, date_str);
                            } else {
                                strcpy(calendar[i].status, reason);
                                modified = 1;
                            }
                        } else if (choice == 3) {
                            if (strcmp(calendar[i].status, "Available") == 0) {
                                printf(YELLOW_COLOR "Date %s is already available.\n" RESET_COLOR, date_str);
                            } else if (strcmp(calendar[i].status, "Booked") == 0) {
                                printf(RED_COLOR "Cannot unblock %s - already booked.\n" RESET_COLOR, date_str);
                            } else {
                                strcpy(calendar[i].status, "Available");
                                modified = 1;
                            }
                        }
                    }
                }
            }

            if (modified) {
                printf(GREEN_COLOR "Saving changes to calendar...\n" RESET_COLOR);
                FILE *file = fopen("Calendar.csv", "w");
                if (!file) {
                    printf(RED_COLOR "Error saving calendar file!\n" RESET_COLOR);
                } else {
                    fprintf(file, "Code,Date,Status\n");
                    for (int i = 0; i < calendar_count; i++) {
                        fprintf(file, "%s,%s,%s\n", calendar[i].code, calendar[i].date, calendar[i].status);
                    }
                    fclose(file);
                    printf(GREEN_COLOR "Calendar updated successfully.\n" RESET_COLOR);
                }
            }

            printf(YELLOW_COLOR "Press Enter to return..." RESET_COLOR);
            getchar(); getchar();
        }

    } while (choice != 0);
}



// View all House function

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
    //load_properties_from_csv("Detail.csv");

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
            case 2: manager_edit_house(); break;
            case 3: manager_delete_house(); break;
            case 4: manager_set_availability(); break;
            case 5: manager_view_all_houses(); break;
            case 6: /* View Bookings For a House */ break;
            case 7: /* Accept/Reject Booking */ break;
            case 0: return;
            default: printf(RED_COLOR "Invalid choice!\n" RESET_COLOR);
        }
    }
}

int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int is_valid_date(int day, int month, int year) {
    if (year < 2024 || month < 1 || month > 12 || day < 1)
        return 0;

    int days_in_month[] = { 31, (is_leap_year(year) ? 29 : 28), 31, 30, 31, 30,
                            31, 31, 30, 31, 30, 31 };

    return day <= days_in_month[month - 1];
}

void customer_booking_page(const char *house_code) {
    clear_screen();

    // Find the house
    House *selected_house = NULL;
    DetailedHouse *selected_detail = NULL;

    for (int i = 0; i < house_count; i++) {
        if (strcmp(houses[i].code, house_code) == 0) {
            selected_house = &houses[i];
            break;
        }
    }

    for (int i = 0; i < detail_count; i++) {
        if (strcmp(details[i].code, house_code) == 0) {
            selected_detail = &details[i];
            break;
        }
    }

    if (!selected_house || !selected_detail) {
        printf(RED_COLOR "Error: House not found.\n" RESET_COLOR);
        printf(YELLOW_COLOR "Press Enter to return..." RESET_COLOR);
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
    printf(WHITE_COLOR "Max Guests: " RESET_COLOR "%d\n", selected_detail->maxGuests);
    //printf(WHITE_COLOR "Status: " RESET_COLOR "%s\n", selected_house->is_available ? GREEN_COLOR "[Available]" RESET_COLOR : RED_COLOR "[Unavailable]" RESET_COLOR);

    printf("\n%sPlease enter your booking details:\n" RESET_COLOR, YELLOW_COLOR);
    printf("----------------------------------\n");

    char full_name[100], phone[20];
    int guests, nights;
    Date checkin, checkout;

    getchar(); // flush leftover newline

    printf(GREEN_COLOR "Full Name: " RESET_COLOR);
    fgets(full_name, sizeof(full_name), stdin);
    full_name[strcspn(full_name, "\n")] = 0;

    printf(GREEN_COLOR "Phone Number: " RESET_COLOR);
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = 0;

    // Guest validation
    do {
        printf(GREEN_COLOR "Number of Guests: " RESET_COLOR);
        scanf("%d", &guests);

        if (guests <= 0 || guests > selected_detail->maxGuests) {
            printf(RED_COLOR "Invalid number of guests! Max allowed: %d\n" RESET_COLOR, selected_detail->maxGuests);
        }
    } while (guests <= 0 || guests > selected_detail->maxGuests);

    do {
        printf(GREEN_COLOR "Check-in Date (DD MM YYYY): " RESET_COLOR);
        scanf("%d %d %d", &checkin.day, &checkin.month, &checkin.year);
    
        if (!is_valid_date(checkin.day, checkin.month, checkin.year)) {
            printf(RED_COLOR "Invalid Check-in Date. Please enter a real date.\n" RESET_COLOR);
        }
    } while (!is_valid_date(checkin.day, checkin.month, checkin.year));    

    do {
        printf(GREEN_COLOR "Check-out Date (DD MM YYYY): " RESET_COLOR);
        scanf("%d %d %d", &checkout.day, &checkout.month, &checkout.year);
    
        if (!is_valid_date(checkout.day, checkout.month, checkout.year)) {
            printf(RED_COLOR "Invalid Check-out Date. Please enter a real date.\n" RESET_COLOR);
        }
    } while (!is_valid_date(checkout.day, checkout.month, checkout.year));    

    struct tm checkin_tm = {0}, checkout_tm = {0};
    checkin_tm.tm_mday = checkin.day;
    checkin_tm.tm_mon = checkin.month - 1;
    checkin_tm.tm_year = checkin.year - 1900;

    checkout_tm.tm_mday = checkout.day;
    checkout_tm.tm_mon = checkout.month - 1;
    checkout_tm.tm_year = checkout.year - 1900;

    time_t checkin_time = mktime(&checkin_tm);
    time_t checkout_time = mktime(&checkout_tm);

    while (checkin_time == -1 || checkout_time == -1 || difftime(checkout_time, checkin_time) <= 0) {
        printf(RED_COLOR "Error: Check-out must be after Check-in.\n" RESET_COLOR);
        printf(YELLOW_COLOR "Please re-enter your check-in and check-out dates.\n" RESET_COLOR);
    
        do {
            printf(GREEN_COLOR "Check-in Date (DD MM YYYY): " RESET_COLOR);
            scanf("%d %d %d", &checkin.day, &checkin.month, &checkin.year);
            if (!is_valid_date(checkin.day, checkin.month, checkin.year)) {
                printf(RED_COLOR "Invalid Check-in Date. Please enter a real date.\n" RESET_COLOR);
            }
        } while (!is_valid_date(checkin.day, checkin.month, checkin.year));    
    
        do {
            printf(GREEN_COLOR "Check-out Date (DD MM YYYY): " RESET_COLOR);
            scanf("%d %d %d", &checkout.day, &checkout.month, &checkout.year);
            if (!is_valid_date(checkout.day, checkout.month, checkout.year)) {
                printf(RED_COLOR "Invalid Check-out Date. Please enter a real date.\n" RESET_COLOR);
            }
        } while (!is_valid_date(checkout.day, checkout.month, checkout.year));    
    
        checkin_tm.tm_mday = checkin.day;
        checkin_tm.tm_mon = checkin.month - 1;
        checkin_tm.tm_year = checkin.year - 1900;
    
        checkout_tm.tm_mday = checkout.day;
        checkout_tm.tm_mon = checkout.month - 1;
        checkout_tm.tm_year = checkout.year - 1900;
    
        checkin_time = mktime(&checkin_tm);
        checkout_time = mktime(&checkout_tm);
    }
    

    nights = (int)(difftime(checkout_time, checkin_time) / (60 * 60 * 24));
    printf(GREEN_COLOR "Number of Nights: " RESET_COLOR "%d\n", nights);

    // Transaction summary
    float total_price = nights * selected_house->price;
    float refund_amount = total_price * 0.9;

    printf(GREEN_COLOR "\n==== TRANSACTION SUMMARY ====\n" RESET_COLOR);
    printf(WHITE_COLOR "Price per Night: " RESET_COLOR "%.2f\n", selected_house->price);
    printf(WHITE_COLOR "Number of Nights: " RESET_COLOR "%d\n", nights);
    printf(WHITE_COLOR "Total Price: " RESET_COLOR "%.2f\n", total_price);

    float amount_paid = 0.0f;
    while (1) {
        printf(GREEN_COLOR "\nEnter amount to pay: " RESET_COLOR);
        scanf("%f", &amount_paid);

        if (amount_paid < total_price) {
            printf(RED_COLOR "Insufficient payment! You must pay at least %.2f\n" RESET_COLOR, total_price);
        } else {
            float change = amount_paid - total_price;
        if (change > 0.0f) {
            printf(GREEN_COLOR "Payment accepted. Your change is: %.2f\n" RESET_COLOR, change);
        } else {
            printf(GREEN_COLOR "Payment accepted.\n" RESET_COLOR);
        }
            break;
        }
    }

    printf(RED_COLOR "\nCancellation Notice:\n" RESET_COLOR);
    printf(YELLOW_COLOR "If you cancel later, only 90%% (%.2f) will be refunded.\n", refund_amount);
    printf("Do you want to confirm the booking? (Y/N): " RESET_COLOR);

    char confirm;
    getchar();  // flush newline
    scanf("%c", &confirm);
    if (tolower(confirm) != 'y') {
        printf(RED_COLOR "\nBooking cancelled by user.\n" RESET_COLOR);
        printf(YELLOW_COLOR "Press Enter to return to menu..." RESET_COLOR);
        while (getchar() != '\n');
        getchar();
        return;
    }

    Booking new_booking;
    new_booking.id = booking_count + 1;
    strcpy(new_booking.house_code, house_code);
    new_booking.customer_id = guests;
    new_booking.date = checkin;
    strcpy(new_booking.status, "Pending");

    bookings[booking_count++] = new_booking;

    // Save to CSV
    append_booking_to_csv("Booking_history.csv", full_name, phone, guests,
                          checkin, checkout, nights, selected_house);

    printf(GREEN_COLOR "\nBooking request submitted successfully!\n" RESET_COLOR);
    printf(YELLOW_COLOR "Press Enter to return to menu..." RESET_COLOR);
    while (getchar() != '\n');
    getchar();
}

void customer_view_house_details(int house_index) {
    clear_screen();

    House h = houses[house_index];
    DetailedHouse d = details[house_index];

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
    printf(WHITE_COLOR "Area: " RESET_COLOR "%.2f sqm\n", d.area);
    printf(WHITE_COLOR "Max Guests: " RESET_COLOR "%d\n", d.maxGuests);
    printf(WHITE_COLOR "Facilities: " RESET_COLOR "%s\n", d.facilities);
    printf(WHITE_COLOR "Landmark: " RESET_COLOR "%s\n", d.landmark);
    printf(WHITE_COLOR "Transport: " RESET_COLOR "%s\n", d.transport);
    printf(WHITE_COLOR "Essential Info: " RESET_COLOR "%s\n", d.essential);

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
            printf(GREEN_COLOR "Press Enter to return to the lists..." RESET_COLOR);
            while (getchar() != '\n');
            getchar();
            return;
        default:
            printf(RED_COLOR "Invalid choice!\n" RESET_COLOR);
            getchar(); getchar();  // Pause
    }
}

void remove_favorite_by_index(int remove_index) {
    FILE *file = fopen("favorites.csv", "r");
    if (!file) return;

    FILE *temp = fopen("temp_favorites.csv", "w");
    if (!temp) {
        fclose(file);
        return;
    }

    char line[256];
    int current = 0;

    while (fgets(line, sizeof(line), file)) {
        if (current != remove_index) {
            fputs(line, temp);
        }
        current++;
    }

    fclose(file);
    fclose(temp);
    remove("favorites.csv");
    rename("temp_favorites.csv", "favorites.csv");
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
    House fav_houses[20];

    while (fgets(line, sizeof(line), file)) {
        House h;
        sscanf(line, "%9[^,],%49[^,],%49[^,],%f,%f",
               h.code, h.name, h.province, &h.price, &h.rating);
        h.is_available = 1;
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

    int selection;
    printf(YELLOW_COLOR "\nEnter the number of the favorite house (0 to cancel): " RESET_COLOR);
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

    printf(YELLOW_COLOR "\n1. Book this house\n2. Remove from favorites\n0. Cancel\n" RESET_COLOR);
    printf("Your choice: ");
    int action;
    scanf("%d", &action);

    if (action == 0) {
        printf(GREEN_COLOR "Press Enter to continue.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    if (action == 2) {
        remove_favorite_by_index(selection - 1);
        printf(GREEN_COLOR "House removed from favorites.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    if (action == 1) {
        char *selected_code = fav_houses[selection - 1].code;

        int index_in_main = -1;
        for (int i = 0; i < house_count; i++) {
            if (strcmp(houses[i].code, selected_code) == 0) {
                index_in_main = i;
                break;
            }
        }

        if (index_in_main == -1) {
            printf(RED_COLOR "Error: House %s not found in system.\n" RESET_COLOR, selected_code);
        } else {
            customer_view_house_details(index_in_main);
            customer_view_favorite_houses();
        }
        return;
    }

    printf(RED_COLOR "Invalid action.\n" RESET_COLOR);
    getchar(); getchar();
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
            //int i = house_count;
            printf(YELLOW_COLOR "\nResult #%d\n" RESET_COLOR, i + 1);
            printf(WHITE_COLOR "Name: " RESET_COLOR "%s\n", houses[i].name);
            printf(WHITE_COLOR "Province: " RESET_COLOR "%s\n", houses[i].province);
            printf(WHITE_COLOR "Price: " RESET_COLOR "%.2f\n", houses[i].price);
            printf(WHITE_COLOR "Rating: " RESET_COLOR "%.1f\n", houses[i].rating);
            printf(WHITE_COLOR "Bedrooms: " RESET_COLOR "%d\n", houses[i].bedrooms);
            printf(WHITE_COLOR "Beds: " RESET_COLOR "%d\n", houses[i].beds);
            printf(WHITE_COLOR "Bathrooms: " RESET_COLOR "%d\n", houses[i].bathrooms);
            printf(WHITE_COLOR "Kitchens: " RESET_COLOR "%d\n", houses[i].kitchens);
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

    customer_view_house_details(selection - 1);
    customer_view_all_houses();
}

void customer_search_by_location() {
    printf("%sFunction: Search\n" RESET_COLOR);
}

void to_lowercase_str(char *str) {
    for (int i = 0; str[i]; i++) str[i] = tolower(str[i]);
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

    char province[50], province_input[50];
    float min_price, max_price;
    float min_rating, max_rating;
    int min_bedrooms, min_beds, min_bathrooms;

    getchar(); // flush
    printf(GREEN_COLOR "Enter province to search (leave blank to skip): " RESET_COLOR);
    fgets(province_input, sizeof(province_input), stdin);
    province_input[strcspn(province_input, "\n")] = 0;
    strcpy(province, province_input);
    to_lowercase_str(province);

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
    printf(GREEN_COLOR "Enter minimum beds (or 0 to skip): " RESET_COLOR);
    scanf("%d", &min_beds);
    printf(GREEN_COLOR "Enter minimum bathrooms (or 0 to skip): " RESET_COLOR);
    scanf("%d", &min_bathrooms);
    int matched_indexes[MAX_HOUSES];
    int matched_count = 0;

    for (int i = 0; i < house_count; i++) {
        if (!houses[i].is_available) continue;

        if (strlen(province) > 0) {
            char house_province[50];
            strcpy(house_province, houses[i].province);
            to_lowercase_str(house_province);
            if (strstr(house_province, province) == NULL)
                continue;
        }

        if ((min_price > 0 && houses[i].price < min_price) ||
            (max_price > 0 && houses[i].price > max_price))
            continue;

        if ((min_rating > 0 && houses[i].rating < min_rating) ||
            (max_rating > 0 && houses[i].rating > max_rating))
            continue;

        if (min_bedrooms > 0 && houses[i].bedrooms < min_bedrooms)
            continue;

        if (min_beds > 0 && houses[i].beds < min_beds)
            continue;

        if (min_bathrooms > 0 && houses[i].bathrooms < min_bathrooms)
            continue;

        matched_indexes[matched_count++] = i;
    }

    if (matched_count == 0) {
        printf(RED_COLOR "\nNo houses match your criteria.\n" RESET_COLOR);
        printf(YELLOW_COLOR "\nPress Enter to return to menu..." RESET_COLOR);
        getchar(); getchar();
        return;
    }

    // Sort by price ascending
    for (int i = 0; i < matched_count - 1; i++) {
        for (int j = i + 1; j < matched_count; j++) {
            if (houses[matched_indexes[i]].price > houses[matched_indexes[j]].price) {
                int temp = matched_indexes[i];
                matched_indexes[i] = matched_indexes[j];
                matched_indexes[j] = temp;
            }
        }
    }
    
    printf(BLUE_COLOR "\n========================\n");
    printf("     FILTERED RESULTS    \n");
    printf("========================\n" RESET_COLOR);

    for (int k = 0; k < matched_count; k++) {
        int i = matched_indexes[k];
        printf(YELLOW_COLOR "\nResult #%d\n" RESET_COLOR, k + 1);
        printf(WHITE_COLOR "Name: " RESET_COLOR "%s\n", houses[i].name);
        printf(WHITE_COLOR "Province: " RESET_COLOR "%s\n", houses[i].province);
        printf(WHITE_COLOR "Price: " RESET_COLOR "%.2f\n", houses[i].price);
        printf(WHITE_COLOR "Rating: " RESET_COLOR "%.1f\n", houses[i].rating);
        printf(WHITE_COLOR "Bedrooms: " RESET_COLOR "%d\n", houses[i].bedrooms);
        printf(WHITE_COLOR "Beds: " RESET_COLOR "%d\n", houses[i].beds);
        printf(WHITE_COLOR "Bathrooms: " RESET_COLOR "%d\n", houses[i].bathrooms);
        printf(WHITE_COLOR "Kitchens: " RESET_COLOR "%d\n", houses[i].kitchens);
    }

    int selection;
    printf(YELLOW_COLOR "\nEnter the number of the house to view details (0 to cancel): " RESET_COLOR);
    scanf("%d", &selection);

    if (selection == 0) {
        printf(GREEN_COLOR "Returning to menu.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    if (selection < 1 || selection > matched_count) {
        printf(RED_COLOR "Invalid selection!\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    customer_view_house_details(matched_indexes[selection - 1]);
}

void customer_my_booking() {
    clear_screen();

    FILE *file = fopen("Booking_history.csv", "r");
    if (!file) {
        printf(RED_COLOR "Could not open booking history.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    char line[512], name_input[100];
    getchar(); // flush newline
    printf(GREEN_COLOR "Enter your full name to check your booking: " RESET_COLOR);
    fgets(name_input, sizeof(name_input), stdin);
    name_input[strcspn(name_input, "\n")] = 0;

    // Store matched bookings in arrays
    char bookings[50][512];
    int booking_count = 0;

    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) {
        char fullname[100];
        sscanf(line, "\"%[^\"]", fullname);

        if (strcmp(fullname, name_input) == 0) {
            strcpy(bookings[booking_count++], line);
        }
    }

    fclose(file);

    if (booking_count == 0) {
        printf(RED_COLOR "\nNo booking found for \"%s\".\n" RESET_COLOR, name_input);
        printf(YELLOW_COLOR "\nPress Enter to return..." RESET_COLOR);
        getchar();
        return;
    }

    printf(BLUE_COLOR "\n========================\n");
    printf("     YOUR BOOKINGS       \n");
    printf("========================\n" RESET_COLOR);

    for (int i = 0; i < booking_count; i++) {
        char fullname[100], phone[20], checkin[20], checkout[20];
        int guests, nights;
        char code[10], housename[100], province[50];
        float price, rating;

        sscanf(bookings[i], "\"%[^\"]\",\"%[^\"]\",%d,%[^,],%[^,],%d,%[^,],\"%[^\"]\",\"%[^\"]\",%f,%f",
               fullname, phone, &guests,
               checkin, checkout, &nights,
               code, housename, province,
               &price, &rating);

        printf(YELLOW_COLOR "\nBooking #%d\n" RESET_COLOR, i + 1);
        printf(WHITE_COLOR "Phone: " RESET_COLOR "%s\n", phone);
        printf(WHITE_COLOR "Guests: " RESET_COLOR "%d\n", guests);
        printf(WHITE_COLOR "Check-in: " RESET_COLOR "%s\n", checkin);
        printf(WHITE_COLOR "Check-out: " RESET_COLOR "%s\n", checkout);
        printf(WHITE_COLOR "Nights: " RESET_COLOR "%d\n", nights);
        printf(WHITE_COLOR "House: " RESET_COLOR "%s (%s)\n", housename, province);
        printf(WHITE_COLOR "Price/Night: " RESET_COLOR "%.2f\n", price);
        printf(WHITE_COLOR "Rating: " RESET_COLOR "%.1f\n", rating);
    }

    int choice;
    printf(YELLOW_COLOR "\nEnter booking number to cancel (0 to skip): " RESET_COLOR);
    scanf("%d", &choice);

    if (choice == 0) {
        printf(GREEN_COLOR "No cancellation performed.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    if (choice < 1 || choice > booking_count) {
        printf(RED_COLOR "Invalid selection.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    // Parse and refund
    char fullname[100], phone[20], checkin[20], checkout[20];
    int guests, nights;
    char code[10], housename[100], province[50];
    float price, rating;

    sscanf(bookings[choice - 1], "\"%[^\"]\",\"%[^\"]\",%d,%[^,],%[^,],%d,%[^,],\"%[^\"]\",\"%[^\"]\",%f,%f",
           fullname, phone, &guests,
           checkin, checkout, &nights,
           code, housename, province,
           &price, &rating);

    float total = price * nights;
    float refund = total * 0.9f;

    printf(GREEN_COLOR "\nBooking cancelled successfully!\n" RESET_COLOR);
    printf(WHITE_COLOR "Total Paid: " RESET_COLOR "%.2f\n", total);
    printf(WHITE_COLOR "Refund (90%%): " RESET_COLOR "%.2f\n", refund);

    // Save to Cancelled_bookings.csv
    FILE *cancel_file = fopen("Cancelled_bookings.csv", "a");
    if (!cancel_file) {
        printf(RED_COLOR "Failed to record cancellation.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    // Add header if file is empty
    fseek(cancel_file, 0, SEEK_END);
    if (ftell(cancel_file) == 0) {
        fprintf(cancel_file, "Fullname,Phone,Guests,CheckIn,CheckOut,Nights,Code,Name,Province,Price,Rating,Refund\n");
    }

    fprintf(cancel_file, "%s,%.2f\n", bookings[choice - 1], refund);
    fclose(cancel_file);

    printf(YELLOW_COLOR "\nCancellation recorded. Press Enter to return..." RESET_COLOR);
    getchar(); getchar();
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
        printf("4. My Booking\n");
        printf("0. Back\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: customer_view_all_houses(); break;
            case 2: customer_filter_advanced(); break;
            case 3: customer_view_favorite_houses(); break;
            case 4: customer_my_booking(); break;
            case 0: return;
            default: printf(RED_COLOR "Invalid choice!\n" RESET_COLOR);
        }
    }
}

void run_system() {
    //load_houses_from_csv("Briefly_Info.csv");
    load_detailed_houses_from_csv("Detail.csv");
    printf("Now Im going to properties");
    load_properties_from_csv("Detail.csv");
    printf("Loaded all csv file finished");
    int choice;
    while (1) {
        clear_screen();
        show_title();
        printf("%sChoose your role:\n", YELLOW_COLOR);
        printf("1. Customer\n");
        printf("2. Manager\n");
        printf("0. Exit\n");
        printf("Your choice: ");
        
        int result = scanf("%d", &choice);
        if (result != 1) {
            while (getchar() != '\n');
            printf(RED_COLOR "Invalid input. Please enter a valid number.\n" RESET_COLOR);
            continue;
        }

        switch (choice) {
            case 1:
                load_houses_for_customer("Briefly_Info.csv");  // ✅ correct
                load_detailed_houses_from_csv("Detail.csv");
                customer_menu();
                break;

            case 2:
                load_houses_for_manager();
                load_calendar("Calendar.csv");
            
                // 🔧 Auto-generate calendar for any house missing calendar entries
                for (int i = 0; i < house_count; i++) {
                    int found = 0;
                    for (int j = 0; j < calendar_count; j++) {
                        if (strcmp(houses[i].code, calendar[j].code) == 0) {
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        generate_calendar_for_new_house(houses[i].code, 30);
                    }
                }
            
                load_calendar("Calendar.csv");  // 🔁 Reload updated calendar
                sync_house_availability_from_calendar();  // ✅ Update in-memory house availability
                manager_menu();
                break;

            case 0:
                save_houses_to_csv("Briefly_Info.csv");
                save_properties_to_csv("Detail.csv");
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define RESET_COLOR "\033[0m"
#define BLUE_COLOR "\033[1;34m"
#define GREEN_COLOR "\033[1;32m"
#define RED_COLOR "\033[1;31m"
#define YELLOW_COLOR "\033[1;33m"
#define WHITE_COLOR "\033[1;37m"

typedef struct {
    int day, month, year;
} Date;

// ==== STRUCT DEFINITIONS ====

typedef struct CalendarEntry {
    char code[10];
    char date[20];
    char status[20];
    struct CalendarEntry *next;
} CalendarEntry;

typedef struct House {
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
    struct House *next;
} House;

typedef struct Property {
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
    struct Property *next;
} Property;

typedef struct DetailedHouse {
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
    struct DetailedHouse *next;
} DetailedHouse;

typedef struct Booking {
    int id;
    char house_code[10];
    int customer_id;
    Date date;
    char status[20];
    struct Booking *next;
} Booking;

// ==== GLOBAL HEAD POINTERS ====
House *houseHead = NULL;
Property *propertyHead = NULL;
DetailedHouse *detailHead = NULL;
CalendarEntry *calendarHead = NULL;

// ==== UTILITY FUNCTIONS ====

void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]);
}

void trim_whitespace(char *str) {
    char *start = str;
    while (isspace(*start)) start++;
    if (start != str) memmove(str, start, strlen(start) + 1);

    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[H\033[J");
    #endif
    fflush(stdout);
}

// ==== LINKED LIST OPERATIONS ====

void insertHouse(House h) {
    House *newNode = (House *)malloc(sizeof(House));
    *newNode = h;
    newNode->next = houseHead;
    houseHead = newNode;
}

void insertProperty(Property p) {
    Property *newNode = (Property *)malloc(sizeof(Property));
    *newNode = p;
    newNode->next = propertyHead;
    propertyHead = newNode;
}

void insertDetailedHouse(DetailedHouse d) {
    DetailedHouse *newNode = (DetailedHouse *)malloc(sizeof(DetailedHouse));
    *newNode = d;
    newNode->next = detailHead;
    detailHead = newNode;
}

void insertCalendar(CalendarEntry c) {
    CalendarEntry *newNode = (CalendarEntry *)malloc(sizeof(CalendarEntry));
    *newNode = c;
    newNode->next = calendarHead;
    calendarHead = newNode;
}

void insertBooking(Booking b) {
    Booking *newNode = (Booking *)malloc(sizeof(Booking));
    *newNode = b;
    newNode->next = bookingHead;
    bookingHead = newNode;
}

// ==== EXAMPLE DISPLAY FUNCTION ====

void displayAllHouses() {
    clear_screen();
    if (!houseHead) {
        printf(RED_COLOR "No houses available!\n" RESET_COLOR);
        return;
    }

    printf(BLUE_COLOR "========================\n");
    printf("     AVAILABLE HOUSES    \n");
    printf("========================\n" RESET_COLOR);

    int index = 1;
    House *current = houseHead;
    while (current) {
        printf(YELLOW_COLOR "\nResult #%d\n" RESET_COLOR, index++);
        printf(WHITE_COLOR "Name: " RESET_COLOR "%s\n", current->name);
        printf(WHITE_COLOR "Province: " RESET_COLOR "%s\n", current->province);
        printf(WHITE_COLOR "Price: " RESET_COLOR "%.2f\n", current->price);
        printf(WHITE_COLOR "Rating: " RESET_COLOR "%.1f\n", current->rating);
        printf(WHITE_COLOR "Bedrooms: " RESET_COLOR "%d\n", current->bedrooms);
        printf(WHITE_COLOR "Beds: " RESET_COLOR "%d\n", current->beds);
        printf(WHITE_COLOR "Bathrooms: " RESET_COLOR "%d\n", current->bathrooms);
        printf(WHITE_COLOR "Kitchens: " RESET_COLOR "%d\n", current->kitchens);
        current = current->next;
    }

    printf(YELLOW_COLOR "\nPress Enter to return to menu..." RESET_COLOR);
    getchar();
    getchar();
}

int main() {
    // Placeholder for testing
    House h1 = {"HH1", "Palm Villa", "Pattaya", 3500.0, 8.9, 3, 4, 2, 1, 1};
    House h2 = {"HH2", "Sunset Villa", "Huahin", 4000.0, 9.2, 4, 5, 3, 1, 1};

    insertHouse(h1);
    insertHouse(h2);

    displayAllHouses();
    return 0;
}


// ==== LOAD DATA FROM CSV FILES INTO LINKED LISTS ====

void load_houses_from_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf(RED_COLOR "Failed to open %s\n" RESET_COLOR, filename);
        return;
    }

    char line[512];
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) {
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
        h.is_available = token ? atoi(token) : 1;

        insertHouse(h);
    }

    fclose(file);
}

void save_houses_to_csv(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf(RED_COLOR "Failed to save house data to %s\n" RESET_COLOR, filename);
        return;
    }

    fprintf(file, "Code,Name,Province,Price,Rating,Bedrooms,Beds,Bathrooms,Kitchens,Availability\n");
    House *curr = houseHead;
    while (curr) {
        fprintf(file, "%s,%s,%s,%.2f,%.1f,%d,%d,%d,%d,%d\n",
                curr->code, curr->name, curr->province,
                curr->price, curr->rating, curr->bedrooms,
                curr->beds, curr->bathrooms, curr->kitchens,
                curr->is_available);
        curr = curr->next;
    }

    fclose(file);
    printf(GREEN_COLOR "Saved houses to CSV: %s\n" RESET_COLOR, filename);
}

// ==== DELETE HOUSE BY CODE ====

void delete_house_by_code(const char *code) {
    House *curr = houseHead, *prev = NULL;
    while (curr) {
        if (strcmp(curr->code, code) == 0) {
            if (prev) {
                prev->next = curr->next;
            } else {
                houseHead = curr->next;
            }
            free(curr);
            printf(GREEN_COLOR "House with code %s deleted.\n" RESET_COLOR, code);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf(RED_COLOR "House with code %s not found.\n" RESET_COLOR, code);
}


// ==== LOAD CALENDAR FROM CSV ====

void load_calendar_from_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf(RED_COLOR "Failed to open calendar file %s\n" RESET_COLOR, filename);
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file);  // Skip header

    while (fgets(line, sizeof(line), file)) {
        CalendarEntry c;
        sscanf(line, "%9[^,],%19[^,],%19s", c.code, c.date, c.status);
        insertCalendar(c);
    }

    fclose(file);
}

// ==== SAVE CALENDAR TO CSV ====

void save_calendar_to_csv(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf(RED_COLOR "Failed to save calendar data\n" RESET_COLOR);
        return;
    }

    fprintf(file, "Code,Date,Status\n");
    CalendarEntry *curr = calendarHead;
    while (curr) {
        fprintf(file, "%s,%s,%s\n", curr->code, curr->date, curr->status);
        curr = curr->next;
    }

    fclose(file);
}

// ==== CHECK AVAILABILITY FOR A DATE RANGE ====

int is_date_range_available(const char *house_code, Date checkin, Date checkout) {
    struct tm start_tm = {0}, end_tm = {0};
    start_tm.tm_year = checkin.year - 1900;
    start_tm.tm_mon = checkin.month - 1;
    start_tm.tm_mday = checkin.day;
    end_tm.tm_year = checkout.year - 1900;
    end_tm.tm_mon = checkout.month - 1;
    end_tm.tm_mday = checkout.day;

    time_t start_time = mktime(&start_tm);
    time_t end_time = mktime(&end_tm);

    CalendarEntry *curr = calendarHead;
    char date_buf[20];
    while (start_time < end_time) {
        strftime(date_buf, sizeof(date_buf), "%Y-%m-%d", localtime(&start_time));
        int found = 0;
        curr = calendarHead;
        while (curr) {
            if (strcmp(curr->code, house_code) == 0 &&
                strcmp(curr->date, date_buf) == 0 &&
                strcmp(curr->status, "Available") == 0) {
                found = 1;
                break;
            }
            curr = curr->next;
        }

        if (!found) return 0;
        start_time += 86400;
    }

    return 1;
}

// ==== MARK DATES AS BOOKED ====

void mark_dates_booked(const char *house_code, Date checkin, Date checkout) {
    struct tm start_tm = {0}, end_tm = {0};
    start_tm.tm_year = checkin.year - 1900;
    start_tm.tm_mon = checkin.month - 1;
    start_tm.tm_mday = checkin.day;
    end_tm.tm_year = checkout.year - 1900;
    end_tm.tm_mon = checkout.month - 1;
    end_tm.tm_mday = checkout.day;

    time_t start_time = mktime(&start_tm);
    time_t end_time = mktime(&end_tm);

    CalendarEntry *curr;
    char date_buf[20];
    while (start_time < end_time) {
        strftime(date_buf, sizeof(date_buf), "%Y-%m-%d", localtime(&start_time));
        curr = calendarHead;
        while (curr) {
            if (strcmp(curr->code, house_code) == 0 &&
                strcmp(curr->date, date_buf) == 0) {
                strcpy(curr->status, "Booked");
                break;
            }
            curr = curr->next;
        }
        start_time += 86400;
    }
}

// ==== ADD BOOKING TO LIST ====

void add_booking_to_list(int id, const char *house_code, int customer_id, Date date, const char *status) {
    Booking b;
    b.id = id;
    strcpy(b.house_code, house_code);
    b.customer_id = customer_id;
    b.date = date;
    strcpy(b.status, status);
    insertBooking(b);
}


// ==== SEARCH FOR HOUSE BY CODE ====

House* find_house_by_code(const char *code) {
    House *curr = houseHead;
    while (curr) {
        if (strcmp(curr->code, code) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

Property* find_property_by_code(const char *code) {
    Property *curr = propertyHead;
    while (curr) {
        if (strcmp(curr->code, code) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

// ==== ADD HOUSE BY MANAGER ====

void manager_add_house() {
    clear_screen();

    Property *new_prop = (Property *)malloc(sizeof(Property));
    if (!new_prop) {
        printf(RED_COLOR "Memory allocation failed.\n" RESET_COLOR);
        return;
    }
    memset(new_prop, 0, sizeof(Property));

    int max_id = 0;
    Property *pcur = propertyHead;
    while (pcur) {
        if (pcur->id > max_id) max_id = pcur->id;
        pcur = pcur->next;
    }
    new_prop->id = max_id + 1;

    printf(GREEN_COLOR "Enter house name: " RESET_COLOR);
    flush_input();
    fgets(new_prop->name, sizeof(new_prop->name), stdin);
    new_prop->name[strcspn(new_prop->name, "\n")] = 0;

    printf(GREEN_COLOR "Enter house address: " RESET_COLOR);
    fgets(new_prop->address, sizeof(new_prop->address), stdin);
    new_prop->address[strcspn(new_prop->address, "\n")] = 0;

    int province_choice;
    while (1) {
        printf(GREEN_COLOR "Select house province:\n" RESET_COLOR);
        printf("1. Huahin/Cha-am\n2. Pattaya\n3. Kanchanaburi\nChoose (1-3): ");
        scanf("%d", &province_choice);
        flush_input();

        if (province_choice == 1) strcpy(new_prop->province, "Huahin/Cha-am");
        else if (province_choice == 2) strcpy(new_prop->province, "Pattaya");
        else if (province_choice == 3) strcpy(new_prop->province, "Kanchanaburi");
        else {
            printf(RED_COLOR "Invalid choice!\n" RESET_COLOR);
            continue;
        }
        break;
    }

    printf(GREEN_COLOR "Enter price: " RESET_COLOR);
    scanf("%f", &new_prop->price); flush_input();

    printf(GREEN_COLOR "Enter area (sqm): " RESET_COLOR);
    scanf("%f", &new_prop->area); flush_input();

    printf(GREEN_COLOR "Enter beds: " RESET_COLOR);
    scanf("%d", &new_prop->beds); flush_input();

    printf(GREEN_COLOR "Enter bedrooms: " RESET_COLOR);
    scanf("%d", &new_prop->bedrooms); flush_input();

    printf(GREEN_COLOR "Enter bathrooms: " RESET_COLOR);
    scanf("%d", &new_prop->bathrooms); flush_input();

    printf(GREEN_COLOR "Enter max guests: " RESET_COLOR);
    scanf("%d", &new_prop->max_guests); flush_input();

    printf(GREEN_COLOR "Enter facilities: " RESET_COLOR);
    fgets(new_prop->facilities, sizeof(new_prop->facilities), stdin);
    new_prop->facilities[strcspn(new_prop->facilities, "\n")] = 0;

    printf(GREEN_COLOR "Enter landmark: " RESET_COLOR);
    fgets(new_prop->landmark, sizeof(new_prop->landmark), stdin);
    new_prop->landmark[strcspn(new_prop->landmark, "\n")] = 0;

    printf(GREEN_COLOR "Enter transportation: " RESET_COLOR);
    fgets(new_prop->transportation, sizeof(new_prop->transportation), stdin);
    new_prop->transportation[strcspn(new_prop->transportation, "\n")] = 0;

    printf(GREEN_COLOR "Enter essentials: " RESET_COLOR);
    fgets(new_prop->essential, sizeof(new_prop->essential), stdin);
    new_prop->essential[strcspn(new_prop->essential, "\n")] = 0;

    while (1) {
        printf(GREEN_COLOR "Enter rating (1.0 - 10.0): " RESET_COLOR);
        scanf("%f", &new_prop->rating); flush_input();
        if (new_prop->rating >= 1.0 && new_prop->rating <= 10.0) break;
        printf(RED_COLOR "Invalid rating.\n" RESET_COLOR);
    }

    // Generate House Code
    char prefix[4];
    if (province_choice == 1) strcpy(prefix, "HH");
    else if (province_choice == 2) strcpy(prefix, "P");
    else if (province_choice == 3) strcpy(prefix, "K");
    else strcpy(prefix, "U");

    int max_suffix = 0;
    House *hc = houseHead;
    while (hc) {
        if (strncmp(hc->code, prefix, strlen(prefix)) == 0) {
            int suffix = atoi(hc->code + strlen(prefix));
            if (suffix > max_suffix) max_suffix = suffix;
        }
        hc = hc->next;
    }

    int next_num = max_suffix + 1;
    House *new_house = (House *)malloc(sizeof(House));
    snprintf(new_house->code, sizeof(new_house->code), "%s%d", prefix, next_num);

    strcpy(new_prop->code, new_house->code);
    strcpy(new_house->name, new_prop->name);
    strcpy(new_house->province, new_prop->province);
    new_house->price = new_prop->price;
    new_house->rating = new_prop->rating;
    new_house->bedrooms = new_prop->bedrooms;
    new_house->beds = new_prop->beds;
    new_house->bathrooms = new_prop->bathrooms;
    new_house->kitchens = 1;
    new_house->is_available = 1;

    new_house->next = houseHead;
    houseHead = new_house;

    new_prop->next = propertyHead;
    propertyHead = new_prop;

    printf(BLUE_COLOR "\nHouse successfully added with code: %s\n" RESET_COLOR, new_house->code);
    printf(YELLOW_COLOR "\nPress Enter to return to Manager Menu..." RESET_COLOR);
    getchar();
}


// ==== EDIT HOUSE ====

void manager_edit_house() {
    char target_code[10];
    printf(GREEN_COLOR "Enter the house code to edit (e.g. HH1, P3): " RESET_COLOR);
    flush_input();
    fgets(target_code, sizeof(target_code), stdin);
    target_code[strcspn(target_code, "\n")] = 0;
    to_uppercase(target_code);

    House *h = find_house_by_code(target_code);
    Property *p = find_property_by_code(target_code);

    if (!h || !p) {
        printf(RED_COLOR "House not found.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    char buffer[256];
    printf(GREEN_COLOR "\nEditing house: %s (%s)\n" RESET_COLOR, h->code, p->name);

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

    printf("Current province: %s\n", p->province);
    printf("Change province (1-Huahin, 2-Pattaya, 3-Kanchanaburi, Enter to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        int choice = atoi(buffer);
        if (choice == 1) { strcpy(p->province, "Huahin/Cha-am"); strcpy(h->province, "Huahin/Cha-am"); }
        else if (choice == 2) { strcpy(p->province, "Pattaya"); strcpy(h->province, "Pattaya"); }
        else if (choice == 3) { strcpy(p->province, "Kanchanaburi"); strcpy(h->province, "Kanchanaburi"); }
    }

    printf("Current price: %.2f\nNew price: ", p->price);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n' && sscanf(buffer, "%f", &p->price)) h->price = p->price;

    printf("Current area: %.2f\nNew area: ", p->area);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') sscanf(buffer, "%f", &p->area);

    printf("Current beds: %d\nNew beds: ", p->beds);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n' && sscanf(buffer, "%d", &p->beds)) h->beds = p->beds;

    printf("Current bedrooms: %d\nNew bedrooms: ", p->bedrooms);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n' && sscanf(buffer, "%d", &p->bedrooms)) h->bedrooms = p->bedrooms;

    printf("Current bathrooms: %d\nNew bathrooms: ", p->bathrooms);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n' && sscanf(buffer, "%d", &p->bathrooms)) h->bathrooms = p->bathrooms;

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

    printf("Current transport: %s\nNew transport: ", p->transportation);
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

    printf("Current rating: %.1f\nNew rating (1-10): ", p->rating);
    fgets(buffer, sizeof(buffer), stdin);
    float r;
    if (buffer[0] != '\n' && sscanf(buffer, "%f", &r) == 1 && r >= 1 && r <= 10) {
        p->rating = r;
        h->rating = r;
    }

    printf(GREEN_COLOR "\nHouse updated successfully.\n" RESET_COLOR);
    getchar();
}

// ==== DELETE HOUSE ====

void manager_delete_house() {
    char target_code[10];
    printf(GREEN_COLOR "Enter the house code to delete (e.g. HH1): " RESET_COLOR);
    scanf("%s", target_code);
    to_uppercase(target_code);

    // Remove from house list
    House *curr = houseHead, *prev = NULL;
    while (curr && strcmp(curr->code, target_code) != 0) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) {
        printf(RED_COLOR "House not found.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }
    if (prev) prev->next = curr->next;
    else houseHead = curr->next;
    free(curr);

    // Remove from property list
    Property *pcurr = propertyHead, *pprev = NULL;
    while (pcurr && strcmp(pcurr->code, target_code) != 0) {
        pprev = pcurr;
        pcurr = pcurr->next;
    }
    if (pcurr) {
        if (pprev) pprev->next = pcurr->next;
        else propertyHead = pcurr->next;
        free(pcurr);
    }

    // Remove from calendar
    CalendarEntry *ccurr = calendarHead, *cprev = NULL;
    while (ccurr) {
        if (strcmp(ccurr->code, target_code) == 0) {
            if (cprev) cprev->next = ccurr->next;
            else calendarHead = ccurr->next;
            CalendarEntry *to_free = ccurr;
            ccurr = ccurr->next;
            free(to_free);
        } else {
            cprev = ccurr;
            ccurr = ccurr->next;
        }
    }

    printf(GREEN_COLOR "House successfully deleted.\n" RESET_COLOR);
    getchar(); getchar();
}


// ==== DISPLAY ALL AVAILABLE HOUSES FOR CUSTOMER ====

void customer_view_all_houses() {
    clear_screen();
    if (!houseHead) {
        printf(RED_COLOR "No houses available!\n" RESET_COLOR);
        return;
    }

    printf(BLUE_COLOR "========================\n");
    printf("     AVAILABLE HOUSES    \n");
    printf("========================\n" RESET_COLOR);

    int index = 1;
    House *curr = houseHead;
    while (curr) {
        if (curr->is_available) {
            printf(YELLOW_COLOR "\nHouse #%d\n" RESET_COLOR, index++);
            printf(WHITE_COLOR "Code: " RESET_COLOR "%s\n", curr->code);
            printf(WHITE_COLOR "Name: " RESET_COLOR "%s\n", curr->name);
            printf(WHITE_COLOR "Province: " RESET_COLOR "%s\n", curr->province);
            printf(WHITE_COLOR "Price: " RESET_COLOR "%.2f\n", curr->price);
            printf(WHITE_COLOR "Rating: " RESET_COLOR "%.1f\n", curr->rating);
            printf(WHITE_COLOR "Bedrooms: " RESET_COLOR "%d\n", curr->bedrooms);
            printf(WHITE_COLOR "Beds: " RESET_COLOR "%d\n", curr->beds);
            printf(WHITE_COLOR "Bathrooms: " RESET_COLOR "%d\n", curr->bathrooms);
            printf(WHITE_COLOR "Kitchens: " RESET_COLOR "%d\n", curr->kitchens);
        }
        curr = curr->next;
    }

    printf(YELLOW_COLOR "\nEnter the house code to book or 0 to return: " RESET_COLOR);
    char input_code[10];
    flush_input();
    fgets(input_code, sizeof(input_code), stdin);
    input_code[strcspn(input_code, "\n")] = 0;
    to_uppercase(input_code);
    if (strcmp(input_code, "0") == 0) return;

    House *selected = find_house_by_code(input_code);
    if (!selected) {
        printf(RED_COLOR "House not found.\n" RESET_COLOR);
        getchar();
        return;
    }

    // Dummy call to booking page (you will plug in full logic later)
    printf(GREEN_COLOR "Proceeding to booking for house: %s\n" RESET_COLOR, selected->name);
    getchar();
}

// ==== CUSTOMER MENU ====

    int choice;
    while (1) {
        clear_screen();
        printf(YELLOW_COLOR "========================\n");
        printf("       CUSTOMER MENU      \n");
        printf("========================\n" RESET_COLOR);
        printf("1. View All Houses\n");
        printf("0. Return\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: customer_view_all_houses(); break;
            case 0: return;
            default: printf(RED_COLOR "Invalid choice!\n" RESET_COLOR); getchar(); getchar();
        }
    }
}


// ==== CHECK DATE OVERLAP BETWEEN TWO RANGES ====

int is_overlap(Date start1, Date end1, Date start2, Date end2) {
    struct tm tm1 = {.tm_mday = start1.day, .tm_mon = start1.month - 1, .tm_year = start1.year - 1900};
    struct tm tm2 = {.tm_mday = end1.day, .tm_mon = end1.month - 1, .tm_year = end1.year - 1900};
    struct tm tm3 = {.tm_mday = start2.day, .tm_mon = start2.month - 1, .tm_year = start2.year - 1900};
    struct tm tm4 = {.tm_mday = end2.day, .tm_mon = end2.month - 1, .tm_year = end2.year - 1900};

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    time_t t3 = mktime(&tm3);
    time_t t4 = mktime(&tm4);

    return !(t2 <= t3 || t4 <= t1);
}

// ==== DUMMY CUSTOMER BOOKING FUNCTION ====

void customer_booking_page(const char *house_code) {
    House *house = find_house_by_code(house_code);
    if (!house || !house->is_available) {
        printf(RED_COLOR "This house is not available for booking.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    Date checkin, checkout;
    printf(GREEN_COLOR "Enter Check-in Date (DD MM YYYY): " RESET_COLOR);
    scanf("%d %d %d", &checkin.day, &checkin.month, &checkin.year);
    printf(GREEN_COLOR "Enter Check-out Date (DD MM YYYY): " RESET_COLOR);
    scanf("%d %d %d", &checkout.day, &checkout.month, &checkout.year);

    if (!is_date_range_available(house_code, checkin, checkout)) {
        printf(RED_COLOR "This house is not available for the selected date range.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    int guests;
    printf(GREEN_COLOR "Enter number of guests: " RESET_COLOR);
    scanf("%d", &guests);

    float total_price = house->price * 1.0f;  // assume one night
    printf(GREEN_COLOR "Total Price: %.2f\n" RESET_COLOR, total_price);

    float payment;
    printf(GREEN_COLOR "Enter amount to pay: " RESET_COLOR);
    scanf("%f", &payment);

    if (payment < total_price) {
        printf(RED_COLOR "Insufficient payment. Booking failed.\n" RESET_COLOR);
        return;
    }

    mark_dates_booked(house_code, checkin, checkout);
    house->is_available = 0;

    printf(GREEN_COLOR "\nBooking Confirmed!\n" RESET_COLOR);
    float refund = total_price * 0.9f;
    printf(YELLOW_COLOR "If cancelled, refund is 90%% = %.2f\n" RESET_COLOR, refund);
    getchar(); getchar();
}


// ==== CANCEL A BOOKING ====

void customer_cancel_booking(const char *house_code, Date checkin, Date checkout) {
    House *house = find_house_by_code(house_code);
    if (!house) {
        printf(RED_COLOR "House not found.\n" RESET_COLOR);
        return;
    }

    // Calculate refund
    float refund = house->price * 0.9f;
    printf(GREEN_COLOR "Refund amount: %.2f\n" RESET_COLOR, refund);

    // Mark dates as Available again
    struct tm tm_in = {.tm_year = checkin.year - 1900, .tm_mon = checkin.month - 1, .tm_mday = checkin.day};
    struct tm tm_out = {.tm_year = checkout.year - 1900, .tm_mon = checkout.month - 1, .tm_mday = checkout.day};
    time_t in_time = mktime(&tm_in);
    time_t out_time = mktime(&tm_out);

    CalendarEntry *curr = calendarHead;
    char date_buf[20];
    while (in_time < out_time) {
        strftime(date_buf, sizeof(date_buf), "%Y-%m-%d", localtime(&in_time));
        curr = calendarHead;
        while (curr) {
            if (strcmp(curr->code, house_code) == 0 && strcmp(curr->date, date_buf) == 0) {
                strcpy(curr->status, "Available");
                break;
            }
            curr = curr->next;
        }
        in_time += 86400;
    }

    house->is_available = 1;
    printf(GREEN_COLOR "Booking cancelled and dates unblocked.\n" RESET_COLOR);
    getchar(); getchar();
}


// ==== ADD TO FAVORITES ====

void add_to_favorites(const char *house_code) {
    FILE *file = fopen("favorites.csv", "a+");
    if (!file) {
        printf(RED_COLOR "Failed to open favorites.csv\n" RESET_COLOR);
        return;
    }

    House *h = find_house_by_code(house_code);
    if (!h) {
        printf(RED_COLOR "House not found.\n" RESET_COLOR);
        fclose(file);
        return;
    }

    fprintf(file, "%s,%s,%s,%.2f,%.1f\n", h->code, h->name, h->province, h->price, h->rating);
    fclose(file);
    printf(GREEN_COLOR "House added to favorites!\n" RESET_COLOR);
}

// ==== DISPLAY FAVORITES ====

void view_favorites() {
    FILE *file = fopen("favorites.csv", "r");
    if (!file) {
        printf(RED_COLOR "No favorites saved.\n" RESET_COLOR);
        return;
    }

    char line[256];
    printf(BLUE_COLOR "\nYour Favorite Houses:\n" RESET_COLOR);
    while (fgets(line, sizeof(line), file)) {
        char code[10], name[50], province[50];
        float price, rating;
        sscanf(line, "%[^,],%[^,],%[^,],%f,%f", code, name, province, &price, &rating);
        printf(YELLOW_COLOR "\nCode: %s\n" RESET_COLOR, code);
        printf("Name: %s\nProvince: %s\nPrice: %.2f\nRating: %.1f\n", name, province, price, rating);
    }
    fclose(file);
    getchar(); getchar();
}

// ==== DISPLAY CALENDAR GRID FOR A HOUSE ====

void display_calendar_grid(const char *house_code, int year, int month) {
    const char *days[] = { "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };

    printf(BLUE_COLOR "\n%s %d - Availability for House %s\n" RESET_COLOR, month_name(month), year, house_code);
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

    for (int day = 1; day <= total_days; day++) {
        char date_str[20];
        snprintf(date_str, sizeof(date_str), "%04d-%02d-%02d", year, month, day);

        char status_char = ' ';
        CalendarEntry *curr = calendarHead;
        while (curr) {
            if (strcmp(curr->code, house_code) == 0 &&
                strcmp(curr->date, date_str) == 0) {
                if (strcmp(curr->status, "Available") == 0)
                    status_char = 'A';
                else if (strcmp(curr->status, "Booked") == 0)
                    status_char = 'B';
                else
                    status_char = 'X';
                break;
            }
            curr = curr->next;
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
    getchar(); getchar();
}


// ==== EXTENDED CUSTOMER MENU WITH FAVORITES & BOOKING ====

    int choice;
    while (1) {
        clear_screen();
        printf(YELLOW_COLOR "========================\n");
        printf("       CUSTOMER MENU      \n");
        printf("========================\n" RESET_COLOR);
        printf("1. View All Houses\n");
        printf("2. View Favorite Houses\n");
        printf("3. Book a House\n");
        printf("4. Show Calendar for House\n");
        printf("0. Return\n");
        printf("Choose: ");
        scanf("%d", &choice);

        flush_input();
        switch (choice) {
            case 1:
                customer_view_all_houses();
                break;
            case 2:
                view_favorites();
                break;
            case 3: {
                char hcode[10];
                printf(GREEN_COLOR "Enter House Code to Book: " RESET_COLOR);
                fgets(hcode, sizeof(hcode), stdin);
                hcode[strcspn(hcode, "\n")] = 0;
                to_uppercase(hcode);
                customer_booking_page(hcode);
                break;
            }
            case 4: {
                char hcode[10];
                int year, month;
                printf(GREEN_COLOR "Enter House Code: " RESET_COLOR);
                fgets(hcode, sizeof(hcode), stdin);
                hcode[strcspn(hcode, "\n")] = 0;
                to_uppercase(hcode);
                printf(GREEN_COLOR "Enter month and year (MM YYYY): " RESET_COLOR);
                scanf("%d %d", &month, &year);
                flush_input();
                display_calendar_grid(hcode, year, month);
                break;
            }
            case 0:
                return;
            default:
                printf(RED_COLOR "Invalid choice!\n" RESET_COLOR);
                getchar(); getchar();
        }
    }
}


void manager_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf(YELLOW_COLOR "========================\n");
        printf("        MANAGER MENU     \n");
        printf("========================\n" RESET_COLOR);
        printf("1. Add House\n");
        printf("2. Edit House\n");
        printf("3. Delete House\n");
        printf("4. View All Houses\n");
        printf("5. View Calendar for House\n");
        printf("0. Return\n");
        printf("Choose: ");
        scanf("%d", &choice);
        flush_input();

        switch (choice) {
            case 1:
                manager_add_house();
                break;
            case 2:
                manager_edit_house();
                break;
            case 3:
                manager_delete_house();
                break;
            case 4: {
                House *curr = houseHead;
                if (!curr) {
                    printf(RED_COLOR "No houses available.\n" RESET_COLOR);
                }
                int i = 1;
                while (curr) {
                    printf(YELLOW_COLOR "\nHouse #%d\n" RESET_COLOR, i++);
                    printf("Code: %s\nName: %s\nProvince: %s\n", curr->code, curr->name, curr->province);
                    printf("Price: %.2f\nRating: %.1f\n", curr->price, curr->rating);
                    printf("Beds: %d | Bedrooms: %d | Bathrooms: %d\n", curr->beds, curr->bedrooms, curr->bathrooms);
                    curr = curr->next;
                }
                getchar(); getchar();
                break;
            }
            case 5: {
                char hcode[10];
                int month, year;
                printf(GREEN_COLOR "Enter House Code: " RESET_COLOR);
                fgets(hcode, sizeof(hcode), stdin);
                hcode[strcspn(hcode, "\n")] = 0;
                to_uppercase(hcode);
                printf(GREEN_COLOR "Enter month and year (MM YYYY): " RESET_COLOR);
                scanf("%d %d", &month, &year);
                flush_input();
                display_calendar_grid(hcode, year, month);
                break;
            }
            case 0:
                return;
            default:
                printf(RED_COLOR "Invalid choice!\n" RESET_COLOR);
                getchar(); getchar();
        }
    }
}


// ==== BOOKING STRUCT AS LINKED LIST ====

typedef struct BookingNode {
    Booking data;
    struct BookingNode *next;
} BookingNode;

BookingNode *bookingHead = NULL;

// ==== ADD BOOKING TO LIST ====

void add_booking(Booking new_booking) {
    BookingNode *node = (BookingNode *)malloc(sizeof(BookingNode));
    if (!node) return;
    node->data = new_booking;
    node->next = bookingHead;
    bookingHead = node;
}


// ==== PRINT ALL BOOKINGS FOR NAME FROM LINKED LIST ====

void customer_my_booking() {
    clear_screen();
    if (!bookingHead) {
        printf(RED_COLOR "No bookings in system.\n" RESET_COLOR);
        getchar(); getchar();
        return;
    }

    char name_input[100];
    getchar();
    printf(GREEN_COLOR "Enter your full name to check your bookings: " RESET_COLOR);
    fgets(name_input, sizeof(name_input), stdin);
    name_input[strcspn(name_input, "\n")] = 0;

    BookingNode *curr = bookingHead;
    int found = 0, index = 1;
    while (curr) {
        if (strcmp(curr->data.name, name_input) == 0) {
            found = 1;
            Booking b = curr->data;
            printf(YELLOW_COLOR "\nBooking #%d\n" RESET_COLOR, index++);
            printf("House Code: %s\n", b.house_code);
            printf("Guests: %d\n", b.customer_id);
            printf("Date: %02d/%02d/%d\n", b.date.day, b.date.month, b.date.year);
            printf("Status: %s\n", b.status);
        }
        curr = curr->next;
    }

    if (!found) {
        printf(RED_COLOR "\nNo bookings found for "%s".\n" RESET_COLOR, name_input);
    }

    printf(YELLOW_COLOR "\nPress Enter to return...\n" RESET_COLOR);
    getchar();
}


// ==== HELPER FUNCTIONS MISSING FROM EARLIER ====

void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

const char* month_name(int month) {
    static const char *names[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    return (month >= 1 && month <= 12) ? names[month - 1] : "Unknown";
}

int is_leap_year(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

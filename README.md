
# Pool Villa Booking System

This terminal-based Pool Villa Booking System allows **customers** to search, view, favorite, and book pool villas, and allows **managers** to manage listings, availability, and booking records. It uses structured data, a calendar system, and CSV files for data persistence.

## Features

### Customer
- View all available houses
- Search & filter by location, price, rating, rooms, etc.
- View detailed information
- Add to/remove from favorites (`favorites.csv`)
- Book a house (with payment and refund logic)
- View and cancel bookings (`Booking_history.csv`, `Cancelled_bookings.csv`)
- Save houses as favorites
- See calendar availability

### Manager
- Add, Edit, and Delete villa listings (`Briefly_Info.csv`, `Detail.csv`)
- Set availability and block dates (e.g., for maintenance) in `Calendar.csv`
- View booking history per house
- Manage CSV records (`Briefly_Info.csv`, `Detail.csv`, `Calendar.csv`)

## CSV File Structure

| File Name              | Purpose                          |
|------------------------|----------------------------------|
| `Briefly_Info.csv`     | Summary of houses for listing    |
| `Detail.csv`           | Detailed house information       |
| `Calendar.csv`         | Availability per house per date  |
| `Booking_history.csv`  | Stores all booking records       |
| `Cancelled_bookings.csv` | Stores canceled bookings with refund |
| `favorites.csv`        | Stores customer favorite houses  |

---

## Data Structures Used

- `House`, `Property`, `DetailedHouse`: store summary and detailed info of villas
- `CalendarEntry`: for tracking availability per day
- `Booking`: structure for internal booking requests
- Arrays and dynamic memory for temporary and persistent data handling

## Sample Input / Output

### Customer: View All Houses

```
Result #1
Name: Sea Breeze Villa
Province: Pattaya
Price: 5200.00
Rating: 9.2
Bedrooms: 3
Beds: 4
Bathrooms: 2
Kitchens: 1

Result #2
Name: Mountain Retreat
Province: Kanchanaburi
Price: 4500.00
Rating: 8.7
Bedrooms: 2
Beds: 3
Bathrooms: 2
Kitchens: 1
```

### Customer: Book House

### Example: Customer views and books a house

**Input:**
```
1  // Select "Customer"
1  // View All Houses
2  // Select House #2
1  // Book this house
John Doe
0912345678
2
10 5 2025  // Check-in
12 5 2025  // Check-out
5000      // Payment
Y         // Confirm
```

**Output:**
```
Booking period allowed: 2025-05-01 to 2025-05-31

Name: Beachfront Villa
Province: Pattaya
Price: 2500.00
Rating: 9.2
Max Guests: 6

Price per Night: 2500.00
Number of Nights: 2
Total Price: 5000.00

Cancellation Notice:
If you cancel later, only 90% (4500.00) will be refunded.
Booking confirmed!
```

---

### Manager: Add House

```
Enter house name: Paradise Pool
Enter address: 99 Palm Rd.
Select province: 2 (Pattaya)
Enter price: 6500
Enter area (sqm): 250
Enter beds: 3
Enter bedrooms: 2
Enter bathrooms: 2
Enter max guests: 6
Enter facilities: Pool, BBQ, WiFi
Enter landmark: Near Beach
Enter transport: Taxi, Tuk-tuk
Enter essentials: Towels, Soap
Enter rating: 9.5

House successfully added with code: P4
```

### Manager deletes a house

**Input:**
```
2  // Select "Manager"
3  // Delete House
P3
Y  // Confirm deletion
```

**Output:**
```
Warning: This house has booked dates!
Are you sure you want to delete house P3 - "Hilltop Villa"? (Y/N): Y
House successfully deleted.
```

---

### Manager: Set Availability

```
Enter house code: P4
Enter month and year: 5 2025

01A 02A 03B 04X 05A 06A ...
Legend:
A = Available
B = Booked
X = Blocked by Manager

Choose date to block/unblock: 4
Reason to block: MAINTENANCE

Calendar updated.
```

## Test Data (Sample from `Briefly_Info.csv`)

```
Code,Name,Province,Price,Rating,Bedrooms,Beds,Bathrooms,Kitchens,Availability
P1,Sea Breeze Villa,Pattaya,5200.00,9.2,3,4,2,1,1
K1,Mountain Retreat,Kanchanaburi,4500.00,8.7,2,3,2,1,1
HH1,Harmony Hideout,Huahin/Cha-am,4800.00,9.0,3,3,2,1,1
```

## Team
- Pawarut Kumnungwut – Manager functionalities
- Kawinpob – Customer functionalities
- Phureeruch – CSV handling, code integration



# 🚗 RideX – Ride Sharing System (C++)

![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Paradigm](https://img.shields.io/badge/Paradigm-Object--Oriented%20Programming-success.svg)
![Build](https://img.shields.io/badge/Build-MinGW%20%2F%20GCC-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%2F%20Cross--Platform-lightgrey.svg)

**RideX** is a modular, console-based ride-sharing simulation built in **modern C++17**. It models the core mechanics of real-world mobility platforms (like Uber and Ola)—such as rider dispatch, driver assignment, vehicle polymorphism, distance-based dynamic pricing, lifecycle state machines, and CSV flat-file persistence.

Developed as a showcase of clean **Object-Oriented Programming (OOP)**, modern memory management (RAII), and software engineering best practices.

---

## 🏛️ System Architecture & Class Hierarchy

```
                               +-------------------+
                               |    RideManager    |  (Controller / State Container)
                               +---------+---------+
                                         |
            +----------------------------+-----------------------------+
            |                            |                             |
            v                            v                             v
   +------------------+         +------------------+         +-------------------+
   |   User (Base)    |         |  Vehicle (Base)  |         |       Ride        |
   +--------+---------+         +---+----+----+----+         +---------+---------+
            |                       |    |    |                        |
     +------+------+                |    |    |                        v
     |             |                |    |    |                  +-----------+
     v             v                v    v    v                  |  Payment  |
  Rider          Driver            Car  Bike Auto                +-----------+
                   |
                   +--- (owns via unique_ptr) ---> Vehicle
```

---

## 🎯 OOP Concepts Demonstrated

| Concept | Project Implementation | Rationale & Interview Insight |
| :--- | :--- | :--- |
| **Classes & Objects** | Concrete instances of `Rider`, `Driver`, `Car`, `Ride`, and `Payment`. | Clean domain modeling separating state and behavior. |
| **Encapsulation** | Private members (`status`, `fare`, `currentRideId`, `isAvailable`) accessible only via validated methods. | Prevents illegal state jumps (e.g. completing an unaccepted ride). |
| **Inheritance** | `User` → `Rider`, `Driver`<br>`Vehicle` → `Car`, `Bike`, `Auto` | Promotes code reusability; avoids redundant definitions of shared identity and transport attributes. |
| **Abstraction** | Pure virtual methods: `User::displayProfile() = 0` and `Vehicle::getType() = 0`. | Hides concrete subtype specifics behind clean interface contracts. |
| **Runtime Polymorphism** | Virtual method `Vehicle::calculateFare(distance)` called through base pointers (`Vehicle*`). | Dynamic dispatch via the compiler's **vtable** mechanism resolves vehicle-specific fares at runtime. |
| **Virtual Destructors** | `virtual ~User() = default;`<br>`virtual ~Vehicle() = default;` | Ensures safe polymorphic destruction, preventing undefined behavior and memory leaks when deleting derived objects via base pointers. |
| **Composition & RAII** | `Driver` owns a `std::unique_ptr<Vehicle>`. | Single-ownership semantics; vehicle memory is automatically reclaimed when the driver is destroyed without manual `delete`. |
| **Smart Pointers & Memory** | `std::shared_ptr` used in `RideManager` across maps and histories; `std::unique_ptr` for exclusive vehicle ownership. | Modern C++ resource management with zero raw pointer leaks. |
| **STL Containers** | `std::unordered_map` for $O(1)$ fast ID lookups; `std::vector` for histories and ledgers. | Practical mastery of C++ standard containers over raw arrays. |
| **Custom Exception Handling** | Exception hierarchy: `RideException` → `EntityNotFoundException`, `InvalidOperationException`, `FileStorageException`. | Graceful error recovery without crashing the CLI process. |
| **File Persistence** | Serializes/deserializes entities to CSV in `data/` via `std::ifstream`, `std::ofstream`, and `std::stringstream`. | Flat-file storage maintaining system state across application restarts. |

---

## 📁 Project Structure

```
RideX/
├── include/
│   ├── User.h             # Abstract base class for system users
│   ├── Rider.h            # Passenger entity & trip tracking
│   ├── Driver.h           # Driver entity & vehicle ownership
│   ├── Vehicle.h          # Abstract base class for vehicles & fares
│   ├── Car.h              # 4-seater car implementation
│   ├── Bike.h             # 1-passenger bike implementation
│   ├── Auto.h             # 3-passenger auto implementation
│   ├── Ride.h             # Trip lifecycle state machine
│   ├── Payment.h          # Billing transaction & invoice receipt
│   ├── RideManager.h      # Central controller & dispatch engine
│   └── Exceptions.h       # Custom domain exception hierarchy
│
├── src/
│   ├── User.cpp
│   ├── Rider.cpp
│   ├── Driver.cpp
│   ├── Vehicle.cpp
│   ├── Car.cpp
│   ├── Bike.cpp
│   ├── Auto.cpp
│   ├── Ride.cpp
│   ├── Payment.cpp
│   ├── RideManager.cpp
│   └── main.cpp           # Interactive CLI menu loop & input validation
│
├── data/                  # Persistent CSV storage directory
│   ├── riders.csv
│   ├── drivers.csv
│   └── rides.csv
│
├── build.bat              # One-click Windows build script
├── PRD.md                 # Product Requirements Document
├── TECHNICAL_DESIGN.md    # Technical Design Document
└── README.md              # Project Documentation
```

---

## ⚡ How to Build and Run

### Prerequisites
- GCC / MinGW supporting C++17 (e.g. `g++ 6.3+` or `9+`) or Clang / MSVC.

### Option 1: 1-Click Batch Script (Windows)
Double-click or run from terminal:
```powershell
.\build.bat
```

### Option 2: Manual Terminal Compilation
```powershell
# Compile all source files with warnings enabled
g++ -std=c++17 -Wall -Wextra -Iinclude src\*.cpp -o ridex.exe

# Run the executable
.\ridex.exe
```

---

## 🎮 CLI Walkthrough

### 1. Main Menu
```text
================== MAIN MENU =======================
1. Rider Portal (Book trips, view history)
2. Driver Portal (Accept rides, manage vehicle)
3. System Overview (View all users & rides)
4. Load Demo Data (Quick start with sample users)
5. Save Data to CSV (Persist session)
6. Load Data from CSV (Restore session)
7. Exit RideX
====================================================
```

### 2. Vehicle Fare Model
| Vehicle | Base Fare | Rate / km | Capacity | Example (10 km) |
| :--- | :---: | :---: | :---: | :---: |
| 🏍️ **Bike** | ₹20.00 | ₹8.00 | 1 | ₹100.00 |
| 🛺 **Auto** | ₹30.00 | ₹11.00 | 3 | ₹140.00 |
| 🚗 **Car** | ₹50.00 | ₹15.00 | 4 | ₹200.00 |

### 3. Generated Invoice Sample
```text
----------------------------------------------------
                 RideX INVOICE RECEIPT              
----------------------------------------------------
 Payment ID     : PAY-5001
 Ride ID        : #1001
 Payment Mode   : UPI
 Amount Payable : Rs 275.00
 Payment Status : PAID [SUCCESS]
----------------------------------------------------
```

---

## 🚀 Step-by-Step Demonstration Guide

Follow this concise walkthrough to demonstrate the full application lifecycle:

### Step 1: Launch Application & View State
1. Run `.\build.bat` (or `.\ridex.exe`).
2. Press **`3` (System Overview)** on the Main Menu:
   - Displays all registered riders, active drivers with vehicles, and past completed trips loaded from CSV files.
   - *(Note: If starting with an empty database, press **`4` (Load Demo Data)** to load sample riders and drivers).*

### Step 2: Book a Ride as a Passenger (Rider Portal)
1. Press **`1` (Rider Portal)** from the Main Menu.
2. Choose **`2` (Select Active Rider)** and enter Rider ID `101` (Aarav Sharma).
3. Press **`2` (Book a Ride)**:
   - Enter Pickup Location: `Cyber City`
   - Enter Drop Location: `Sector 29`
   - Enter Distance: `12.0`
   - Select Vehicle Type: `1` (Car)
4. A new Ride is generated in `REQUESTED` status.
5. Press **`6`** to logout and return to the Main Menu.

### Step 3: Accept, Start & Complete Trip (Driver Portal)
1. Press **`2` (Driver Portal)** from the Main Menu.
2. Choose **`2` (Select Active Driver)** and enter Driver ID `201` (Vikram Singh - Car).
3. Press **`4` (View Pending Requests)** to see matching trip requests.
4. Press **`5` (Accept Ride)** and enter the Ride ID (Driver is now marked `UNAVAILABLE`).
5. Press **`6` (Start Ride)** — Status transitions to `IN_PROGRESS`.
6. Press **`7` (Complete Ride & Collect Payment)**:
   - Select payment mode: `2` (UPI).
   - An invoice receipt is generated with dynamically computed fare (`Rs 50 base + 12 km * Rs 15 = Rs 230.00`).
   - Driver is released back to `AVAILABLE`.
7. Press **`8`** to verify updated trip history and cumulative net earnings.

### Step 4: Verify Exception Handling & Persistence
1. **Exception Handling:**
   - In Rider Portal, attempt to book a ride with a negative distance (e.g. `-5`).
   - The application gracefully catches the error:
     `[ERROR] [Invalid Operation] Distance must be greater than zero.`
2. **CSV Flat-File Persistence:**
   - Choose **`7` (Exit RideX)** from the Main Menu. The session is auto-saved to `data/`.
   - Relaunch `.\ridex.exe` and press **`3` (System Overview)** to demonstrate that all data persisted across sessions.

---

## 💡 Key Interview Questions & Answers (C++ OOP)

### Q1: Why did you declare the destructor in `User` and `Vehicle` as `virtual`?
> **Answer:** When an object of a derived class (e.g., `Car`) is deleted through a base class pointer (`Vehicle*`), declaring the base class destructor as `virtual` guarantees that the derived class destructor is called first, followed by the base class destructor. Without `virtual`, only the base destructor would execute, leading to resource leaks or undefined behavior.

### Q2: What is the difference between `std::unique_ptr` and `std::shared_ptr`, and where did you use each?
> **Answer:**
> - `std::unique_ptr` represents exclusive ownership with zero overhead. We used it in `Driver` (`std::unique_ptr<Vehicle>`) because a driver exclusively owns their assigned vehicle.
> - `std::shared_ptr` represents shared ownership via reference counting. We used it in `RideManager` for users and rides because multiple systems (active lookups, ride histories, matching queues) need concurrent access to the same entity instances without dangling references.

### Q3: How does runtime polymorphism work behind the scenes in C++?
> **Answer:** When a class contains at least one virtual function, the compiler generates a **Virtual Method Table (vtable)** containing function pointers to the virtual methods for that specific class. Each object of that class contains an invisible pointer (**vptr**) pointing to its class's vtable. At runtime, calling `vehicle->calculateFare(dist)` looks up the function pointer via `vptr -> vtable`, resolving to `Car::calculateFare` or `Bike::calculateFare` dynamically.

### Q4: Why use `std::unordered_map` instead of `std::map` in `RideManager`?
> **Answer:** `std::map` is implemented as a Red-Black Tree (Self-Balancing BST) with $O(\log N)$ search time. `std::unordered_map` is implemented using a Hash Table providing average $O(1)$ constant-time lookups, which is optimal for frequent ID-based entity retrieval in dispatch platforms.

---

## 📜 License
This project is open-source under the MIT License — feel free to use, modify, and build upon it for learning and academic portfolios.

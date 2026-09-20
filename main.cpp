#include <iostream>
#include <string>
#include <limits>
#include <iomanip>

#include "RideManager.h"
#include "Exceptions.h"

// --- Input Utilities ---

static void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInputBuffer();
            return value;
        }
        std::cout << "Invalid input. Please enter a valid number.\n";
        clearInputBuffer();
    }
}

static double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInputBuffer();
            return value;
        }
        std::cout << "Invalid input. Please enter a valid decimal number.\n";
        clearInputBuffer();
    }
}

static std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

// --- Rider Portal Handlers ---

static void handleActiveRiderMenu(RideManager& manager, int riderId) {
    while (true) {
        auto rider = manager.getRider(riderId);
        if (!rider) break;

        std::cout << "\n----------------------------------------------------\n";
        std::cout << " Rider: " << rider->getName() << " (ID: " << riderId << ")\n";
        std::cout << "----------------------------------------------------\n";
        std::cout << "1. View Available Drivers\n";
        std::cout << "2. Book a Ride\n";
        std::cout << "3. View Current Active Ride\n";
        std::cout << "4. Cancel Current Ride\n";
        std::cout << "5. View My Ride History\n";
        std::cout << "6. Switch / Logout Rider Profile\n";

        int choice = readInt("Select an option (1-6): ");

        try {
            if (choice == 1) {
                std::cout << "\nFilter by vehicle? (1. Car, 2. Bike, 3. Auto, 4. Show All): ";
                int vChoice = readInt("");
                std::string vFilter = "";
                if (vChoice == 1) vFilter = "Car";
                else if (vChoice == 2) vFilter = "Bike";
                else if (vChoice == 3) vFilter = "Auto";

                auto drivers = manager.getAvailableDrivers(vFilter);
                std::cout << "\n--- Available Drivers (" << drivers.size() << ") ---\n";
                if (drivers.empty()) {
                    std::cout << "No drivers currently available for this category.\n";
                } else {
                    for (const auto& d : drivers) {
                        std::cout << " - Driver ID: " << d->getId() 
                                  << " | Name: " << d->getName() 
                                  << " | Vehicle: " << d->getVehicle()->getType() 
                                  << " (" << d->getVehicle()->getLicensePlate() << ")"
                                  << " | Rate/km: Rs " << d->getVehicle()->getPerKmRate() << "\n";
                    }
                }
            } else if (choice == 2) {
                if (rider->hasActiveRide()) {
                    std::cout << "\nYou already have an active ride (ID: #" << rider->getCurrentRideId() << ").\n";
                    continue;
                }

                std::string pickup = readLine("Enter Pickup Location: ");
                std::string drop = readLine("Enter Drop Location: ");
                double distance = readDouble("Enter Distance in km: ");

                std::cout << "Select Vehicle Type:\n 1. Car (Rs 50 base, Rs 15/km)\n 2. Bike (Rs 20 base, Rs 8/km)\n 3. Auto (Rs 30 base, Rs 11/km)\n";
                int vChoice = readInt("Choice (1-3): ");
                std::string vType = "Car";
                if (vChoice == 2) vType = "Bike";
                else if (vChoice == 3) vType = "Auto";

                int rideId = manager.bookRide(riderId, pickup, drop, distance, vType);
                std::cout << "\n[SUCCESS] Ride booked successfully! Your Ride ID is: #" << rideId << "\n";
                std::cout << "Waiting for an available " << vType << " driver to accept...\n";
            } else if (choice == 3) {
                if (!rider->hasActiveRide()) {
                    std::cout << "\nYou have no active rides right now.\n";
                } else {
                    auto ride = manager.getRide(rider->getCurrentRideId());
                    if (ride) {
                        std::cout << "\n--- Active Ride Status ---\n";
                        ride->displayRideDetails();
                    }
                }
            } else if (choice == 4) {
                if (!rider->hasActiveRide()) {
                    std::cout << "\nNo active ride to cancel.\n";
                } else {
                    int activeRideId = rider->getCurrentRideId();
                    if (manager.cancelRide(activeRideId, riderId)) {
                        std::cout << "\n[SUCCESS] Ride #" << activeRideId << " has been cancelled.\n";
                    }
                }
            } else if (choice == 5) {
                auto history = manager.getRiderRideHistory(riderId);
                std::cout << "\n--- My Ride History (" << history.size() << " trips) ---\n";
                if (history.empty()) {
                    std::cout << "No past rides recorded.\n";
                } else {
                    for (const auto& r : history) {
                        r->displayRideDetails();
                    }
                }
            } else if (choice == 6) {
                std::cout << "Logging out from Rider profile...\n";
                break;
            } else {
                std::cout << "Invalid choice. Please choose 1-6.\n";
            }
        } catch (const RideException& ex) {
            std::cout << "\n[ERROR] " << ex.what() << "\n";
        }
    }
}

static void handleRiderPortal(RideManager& manager) {
    while (true) {
        std::cout << "\n====================================================\n";
        std::cout << "                 RIDER PORTAL                       \n";
        std::cout << "====================================================\n";
        std::cout << "1. Register as New Rider\n";
        std::cout << "2. Select Active Rider by ID\n";
        std::cout << "3. Return to Main Menu\n";

        int option = readInt("Select an option (1-3): ");

        if (option == 1) {
            std::string name = readLine("Enter Full Name: ");
            std::string phone = readLine("Enter Phone Number: ");
            int id = manager.registerRider(name, phone);
            std::cout << "\n[SUCCESS] Registered Rider: " << name << " | Assigned ID: " << id << "\n";
            handleActiveRiderMenu(manager, id);
        } else if (option == 2) {
            int id = readInt("Enter Rider ID: ");
            auto rider = manager.getRider(id);
            if (!rider) {
                std::cout << "Rider ID " << id << " not found!\n";
            } else {
                handleActiveRiderMenu(manager, id);
            }
        } else if (option == 3) {
            break;
        } else {
            std::cout << "Invalid choice. Please choose 1-3.\n";
        }
    }
}

// --- Driver Portal Handlers ---

static void handleActiveDriverMenu(RideManager& manager, int driverId) {
    while (true) {
        auto driver = manager.getDriver(driverId);
        if (!driver) break;

        std::cout << "\n----------------------------------------------------\n";
        std::cout << " Driver: " << driver->getName() << " (ID: " << driverId << ")\n";
        std::cout << " Vehicle: " << (driver->hasVehicle() ? driver->getVehicle()->getType() : "NONE")
                  << " | Status: " << (driver->getAvailability() ? "AVAILABLE" : "UNAVAILABLE") << "\n";
        std::cout << "----------------------------------------------------\n";
        std::cout << "1. View Profile & Vehicle\n";
        std::cout << "2. Register / Change Vehicle (Car / Bike / Auto)\n";
        std::cout << "3. Toggle Availability (Online / Offline)\n";
        std::cout << "4. View Pending Ride Requests\n";
        std::cout << "5. Accept a Ride Request\n";
        std::cout << "6. Start Accepted Ride\n";
        std::cout << "7. Complete Ride & Collect Payment\n";
        std::cout << "8. View Ride History & Total Earnings\n";
        std::cout << "9. Switch / Logout Driver Profile\n";

        int choice = readInt("Select an option (1-9): ");

        try {
            if (choice == 1) {
                std::cout << "\n--- Driver Profile ---\n";
                driver->displayProfile();
            } else if (choice == 2) {
                std::cout << "Select Vehicle Type:\n 1. Car (4 seats)\n 2. Bike (1 seat)\n 3. Auto (3 seats)\n";
                int vChoice = readInt("Choice (1-3): ");
                std::string vType = "Car";
                if (vChoice == 2) vType = "Bike";
                else if (vChoice == 3) vType = "Auto";

                std::string plate = readLine("Enter License Plate Number: ");
                manager.assignVehicleToDriver(driverId, vType, plate);
                std::cout << "\n[SUCCESS] Vehicle " << vType << " (" << plate << ") assigned to you!\n";
            } else if (choice == 3) {
                if (!driver->hasVehicle()) {
                    std::cout << "\nCannot go online without an assigned vehicle! Please register a vehicle first.\n";
                    continue;
                }
                if (driver->getCurrentRideId() != -1) {
                    std::cout << "\nCannot toggle availability while executing an active ride!\n";
                    continue;
                }
                bool newStatus = !driver->getAvailability();
                driver->setAvailability(newStatus);
                std::cout << "\n[STATUS UPDATED] You are now: " << (newStatus ? "AVAILABLE (Online)" : "UNAVAILABLE (Offline)") << "\n";
            } else if (choice == 4) {
                if (!driver->hasVehicle()) {
                    std::cout << "\nPlease add a vehicle first to view matching ride requests.\n";
                    continue;
                }
                std::string myType = driver->getVehicle()->getType();
                auto pending = manager.getPendingRides(myType);
                std::cout << "\n--- Pending " << myType << " Ride Requests (" << pending.size() << ") ---\n";
                if (pending.empty()) {
                    std::cout << "No pending requests for " << myType << " right now.\n";
                } else {
                    for (const auto& r : pending) {
                        r->displayRideDetails();
                    }
                }
            } else if (choice == 5) {
                if (!driver->isAvailableForRide()) {
                    std::cout << "\nYou must be AVAILABLE and have no ongoing rides to accept new requests.\n";
                    continue;
                }
                int rideId = readInt("Enter Ride ID to Accept: ");
                if (manager.acceptRide(driverId, rideId)) {
                    std::cout << "\n[SUCCESS] You have accepted Ride #" << rideId << "!\n";
                }
            } else if (choice == 6) {
                int rideId = driver->getCurrentRideId();
                if (rideId == -1) {
                    std::cout << "\nYou have no assigned ride to start.\n";
                    continue;
                }
                if (manager.startRide(driverId, rideId)) {
                    std::cout << "\n[SUCCESS] Ride #" << rideId << " is now IN PROGRESS. Drive safely!\n";
                }
            } else if (choice == 7) {
                int rideId = driver->getCurrentRideId();
                if (rideId == -1) {
                    std::cout << "\nYou have no ongoing ride to complete.\n";
                    continue;
                }
                std::cout << "Select Payment Collection Mode:\n 1. Cash\n 2. UPI\n 3. Card\n";
                int pChoice = readInt("Choice (1-3): ");
                std::string pMode = "UPI";
                if (pChoice == 1) pMode = "Cash";
                else if (pChoice == 3) pMode = "Card";

                if (manager.completeRide(driverId, rideId, pMode)) {
                    std::cout << "\n[SUCCESS] Ride #" << rideId << " completed and payment settled!\n";
                }
            } else if (choice == 8) {
                auto history = manager.getDriverRideHistory(driverId);
                std::cout << "\n--- My Trip History (" << history.size() << " trips) ---\n";
                for (const auto& r : history) {
                    r->displayRideDetails();
                }
                std::cout << "Total Net Earnings: Rs " << std::fixed << std::setprecision(2)
                          << manager.calculateDriverEarnings(driverId) << "\n";
            } else if (choice == 9) {
                std::cout << "Logging out from Driver profile...\n";
                break;
            } else {
                std::cout << "Invalid choice. Please choose 1-9.\n";
            }
        } catch (const RideException& ex) {
            std::cout << "\n[ERROR] " << ex.what() << "\n";
        }
    }
}

static void handleDriverPortal(RideManager& manager) {
    while (true) {
        std::cout << "\n====================================================\n";
        std::cout << "                 DRIVER PORTAL                      \n";
        std::cout << "====================================================\n";
        std::cout << "1. Register as New Driver\n";
        std::cout << "2. Select Active Driver by ID\n";
        std::cout << "3. Return to Main Menu\n";

        int option = readInt("Select an option (1-3): ");

        try {
            if (option == 1) {
                std::string name = readLine("Enter Full Name: ");
                std::string phone = readLine("Enter Phone Number: ");
                int id = manager.registerDriver(name, phone);
                std::cout << "\n[SUCCESS] Registered Driver: " << name << " | Assigned ID: " << id << "\n";

                std::cout << "Would you like to register a vehicle now? (1. Yes, 2. No): ";
                int addV = readInt("");
                if (addV == 1) {
                    std::cout << "Select Vehicle Type:\n 1. Car\n 2. Bike\n 3. Auto\n";
                    int vChoice = readInt("Choice (1-3): ");
                    std::string vType = "Car";
                    if (vChoice == 2) vType = "Bike";
                    else if (vChoice == 3) vType = "Auto";

                    std::string plate = readLine("Enter License Plate Number: ");
                    manager.assignVehicleToDriver(id, vType, plate);
                    std::cout << "Vehicle registered and marked AVAILABLE!\n";
                }
                handleActiveDriverMenu(manager, id);
            } else if (option == 2) {
                int id = readInt("Enter Driver ID: ");
                auto driver = manager.getDriver(id);
                if (!driver) {
                    std::cout << "Driver ID " << id << " not found!\n";
                } else {
                    handleActiveDriverMenu(manager, id);
                }
            } else if (option == 3) {
                break;
            } else {
                std::cout << "Invalid choice. Please choose 1-3.\n";
            }
        } catch (const RideException& ex) {
            std::cout << "\n[ERROR] " << ex.what() << "\n";
        }
    }
}

// --- Main CLI Entrypoint ---

int main() {
    RideManager manager;

    std::cout << "====================================================\n";
    std::cout << "            Welcome to RideX Terminal               \n";
    std::cout << "          Ride-Sharing System Simulation            \n";
    std::cout << "====================================================\n";

    // Auto-load saved records if available
    try {
        manager.loadFromFile("data");
    } catch (...) {
        // First run: ignore if data directory empty
    }

    while (true) {
        std::cout << "\n================== MAIN MENU =======================\n";
        std::cout << "1. Rider Portal (Book trips, view history)\n";
        std::cout << "2. Driver Portal (Accept rides, manage vehicle)\n";
        std::cout << "3. System Overview (View all users & rides)\n";
        std::cout << "4. Load Demo Data (Quick start with sample users)\n";
        std::cout << "5. Save Data to CSV (Persist session)\n";
        std::cout << "6. Load Data from CSV (Restore session)\n";
        std::cout << "7. Exit RideX\n";
        std::cout << "====================================================\n";

        int choice = readInt("Select an option (1-7): ");

        try {
            if (choice == 1) {
                handleRiderPortal(manager);
            } else if (choice == 2) {
                handleDriverPortal(manager);
            } else if (choice == 3) {
                manager.displaySystemSummary();
                manager.displayAllRiders();
                manager.displayAllDrivers();
                manager.displayAllRides();
            } else if (choice == 4) {
                manager.seedDemoData();
            } else if (choice == 5) {
                if (manager.saveToFile("data")) {
                    std::cout << "\n[SUCCESS] All data successfully saved to 'data/' folder (CSV files).\n";
                }
            } else if (choice == 6) {
                if (manager.loadFromFile("data")) {
                    std::cout << "\n[SUCCESS] All data successfully restored from 'data/' folder.\n";
                }
            } else if (choice == 7) {
                // Auto-save on exit
                try {
                    manager.saveToFile("data");
                    std::cout << "[Session auto-saved to CSV]\n";
                } catch (...) {}
                std::cout << "Thank you for using RideX! Safe travels!\n";
                break;
            } else {
                std::cout << "Invalid option. Please choose between 1 and 7.\n";
            }
        } catch (const RideException& ex) {
            std::cout << "\n[ERROR] " << ex.what() << "\n";
        } catch (const std::exception& ex) {
            std::cout << "\n[STANDARD EXCEPTION] " << ex.what() << "\n";
        }
    }

    return 0;
}

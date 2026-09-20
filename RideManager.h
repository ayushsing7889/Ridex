#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "User.h"
#include "Rider.h"
#include "Driver.h"
#include "Vehicle.h"
#include "Car.h"
#include "Bike.h"
#include "Auto.h"
#include "Ride.h"
#include "Payment.h"
#include "Exceptions.h"

/**
 * @brief Central Controller & State Manager for the RideX system.
 * Orchestrates business rules and state transitions between Users, Vehicles, Rides, and Payments.
 */
class RideManager {
private:
    std::unordered_map<int, std::shared_ptr<Rider>> riders;
    std::unordered_map<int, std::shared_ptr<Driver>> drivers;
    std::unordered_map<int, std::shared_ptr<Ride>> rides;
    std::vector<std::shared_ptr<Payment>> payments;

    int nextRiderId;
    int nextDriverId;
    int nextRideId;
    int nextPaymentId;

    void updateNextIds();

public:
    RideManager();

    // Factory & Registration
    int registerRider(const std::string& name, const std::string& phone);
    int registerDriver(const std::string& name, const std::string& phone);
    bool assignVehicleToDriver(int driverId, const std::string& vehicleType, const std::string& plate);

    // Lookups (Returns nullptr if not found)
    std::shared_ptr<Rider> getRider(int riderId) const;
    std::shared_ptr<Driver> getDriver(int driverId) const;
    std::shared_ptr<Ride> getRide(int rideId) const;

    // Queries
    std::vector<std::shared_ptr<Driver>> getAvailableDrivers(const std::string& vehicleType = "") const;
    std::vector<std::shared_ptr<Ride>> getPendingRides(const std::string& vehicleType = "") const;
    std::vector<std::shared_ptr<Ride>> getRiderRideHistory(int riderId) const;
    std::vector<std::shared_ptr<Ride>> getDriverRideHistory(int driverId) const;
    double calculateDriverEarnings(int driverId) const;

    // Core Ride Lifecycle Actions (May throw RideException / InvalidOperationException)
    int bookRide(int riderId, const std::string& pickup, const std::string& drop, double distanceKm, const std::string& vehicleType);
    bool acceptRide(int driverId, int rideId);
    bool startRide(int driverId, int rideId);
    bool completeRide(int driverId, int rideId, const std::string& paymentMethod);
    bool cancelRide(int rideId, int requesterId);

    // Reporting & Summary
    void displayAllRiders() const;
    void displayAllDrivers() const;
    void displayAllRides() const;
    void displaySystemSummary() const;

    // Seed Demo Data for Instant Interactive Testing
    void seedDemoData();

    // File Persistence (CSV Serialization / Deserialization)
    bool saveToFile(const std::string& dataDir = "data") const;
    bool loadFromFile(const std::string& dataDir = "data");
};

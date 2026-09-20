#pragma once

#include "User.h"
#include "Vehicle.h"
#include <memory>
#include <vector>

/**
 * @brief Represents a driver in the RideX system.
 */
class Driver : public User {
private:
    std::unique_ptr<Vehicle> vehicle;
    bool isAvailable;
    int currentRideId;
    std::vector<int> rideHistory;

public:
    Driver(int userId, std::string userName, std::string userPhone);

    void displayProfile() const override;

    // Vehicle management
    void assignVehicle(std::unique_ptr<Vehicle> newVehicle);
    Vehicle* getVehicle() const;
    bool hasVehicle() const;

    // Availability & Ride state management
    bool isAvailableForRide() const;
    void setAvailability(bool status);
    bool getAvailability() const;

    int getCurrentRideId() const;
    void setCurrentRide(int rideId);
    void clearCurrentRide();

    // History
    void addRideToHistory(int rideId);
    const std::vector<int>& getRideHistory() const;
};

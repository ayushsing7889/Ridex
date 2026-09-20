#pragma once

#include "User.h"
#include <vector>

/**
 * @brief Represents a passenger/rider in the RideX system.
 */
class Rider : public User {
private:
    int currentRideId;
    std::vector<int> rideHistory;

public:
    Rider(int userId, std::string userName, std::string userPhone);

    void displayProfile() const override;

    // Ride state management
    bool hasActiveRide() const;
    int getCurrentRideId() const;
    void setCurrentRide(int rideId);
    void clearCurrentRide();

    // Ride history
    void addRideToHistory(int rideId);
    const std::vector<int>& getRideHistory() const;
};

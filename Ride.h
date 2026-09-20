#pragma once

#include <string>

/**
 * @brief Scoped enumeration representing the lifecycle states of a Ride.
 */
enum class RideStatus {
    REQUESTED,
    ACCEPTED,
    IN_PROGRESS,
    COMPLETED,
    CANCELLED
};

/**
 * @brief Represents a trip/ride and manages its lifecycle state machine.
 */
class Ride {
private:
    int rideId;
    int riderId;
    int driverId; // -1 if not assigned yet
    std::string pickupLocation;
    std::string dropLocation;
    double distanceKm;
    std::string requestedVehicleType;
    RideStatus status;
    double fare;

public:
    Ride(int id, int rId, std::string pickup, std::string drop, double distance, std::string vType);

    // State Machine Transitions (Returns true if successful, false if transition is invalid)
    bool assignDriver(int dId);
    bool startRide();
    bool completeRide(double finalFare);
    bool cancelRide();

    // Inspector / Getter Methods
    int getRideId() const;
    int getRiderId() const;
    int getDriverId() const;
    std::string getPickupLocation() const;
    std::string getDropLocation() const;
    double getDistanceKm() const;
    std::string getRequestedVehicleType() const;
    RideStatus getStatus() const;
    double getFare() const;

    // Helpers
    std::string getStatusString() const;
    void displayRideDetails() const;
};

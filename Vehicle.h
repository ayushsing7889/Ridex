#pragma once

#include <string>

/**
 * @brief Abstract Base Class representing a generic Vehicle.
 * Encapsulates common attributes (license plate, capacity, fare rates)
 * and defines the interface for vehicle-specific fare calculations.
 */
class Vehicle {
protected:
    std::string licensePlate;
    int capacity;
    double baseFare;
    double perKmRate;

public:
    Vehicle(std::string plate, int cap, double base, double rate);
    virtual ~Vehicle() = default;

    virtual std::string getType() const = 0;
    virtual double calculateFare(double distanceKm) const;

    // Getters
    std::string getLicensePlate() const;
    int getCapacity() const;
    double getBaseFare() const;
    double getPerKmRate() const;
};

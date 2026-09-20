#pragma once

#include "Vehicle.h"

/**
 * @brief Concrete class representing a 1-passenger Bike.
 */
class Bike : public Vehicle {
public:
    explicit Bike(const std::string& plate);

    std::string getType() const override;
};

#pragma once

#include "Vehicle.h"

/**
 * @brief Concrete class representing a 3-passenger Auto-rickshaw.
 */
class Auto : public Vehicle {
public:
    explicit Auto(const std::string& plate);

    std::string getType() const override;
};

#pragma once

#include "Vehicle.h"

/**
 * @brief Concrete class representing a 4-seater Car.
 */
class Car : public Vehicle {
public:
    explicit Car(const std::string& plate);

    std::string getType() const override;
};

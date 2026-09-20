#pragma once

#include <string>

/**
 * @brief Abstract Base Class representing any User in the RideX system.
 */
class User {
protected:
    int id;
    std::string name;
    std::string phone;

public:
    User(int userId, std::string userName, std::string userPhone);
    virtual ~User() = default;

    virtual void displayProfile() const = 0;

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getPhone() const;
};

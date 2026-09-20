#pragma once

#include <string>

/**
 * @brief Represents the payment transaction record for a completed ride.
 */
class Payment {
private:
    int paymentId;
    int rideId;
    double amount;
    std::string paymentMethod; // e.g. "Cash", "UPI", "Card"
    bool isPaid;

public:
    Payment(int pId, int rId, double amt, std::string method);

    void processPayment();
    bool isCompleted() const;

    // Getters
    int getPaymentId() const;
    int getRideId() const;
    double getAmount() const;
    std::string getPaymentMethod() const;

    // Receipt formatting
    void displayReceipt() const;
};

#pragma once

#include <exception>
#include <string>
#include <utility>

/**
 * @brief Base custom exception for all RideX domain errors.
 */
class RideException : public std::exception {
protected:
    std::string message;

public:
    explicit RideException(std::string msg) : message(std::move(msg)) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

/**
 * @brief Thrown when a requested entity (Rider, Driver, or Ride) does not exist.
 */
class EntityNotFoundException : public RideException {
public:
    explicit EntityNotFoundException(const std::string& msg)
        : RideException("[Entity Not Found] " + msg) {}
};

/**
 * @brief Thrown when a business rule or state transition is violated.
 */
class InvalidOperationException : public RideException {
public:
    explicit InvalidOperationException(const std::string& msg)
        : RideException("[Invalid Operation] " + msg) {}
};

/**
 * @brief Thrown when file reading or writing fails.
 */
class FileStorageException : public RideException {
public:
    explicit FileStorageException(const std::string& msg)
        : RideException("[Storage Error] " + msg) {}
};

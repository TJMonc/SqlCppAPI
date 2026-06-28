#pragma once
#include <stdexcept>

class DatabaseException : public std::runtime_error{
    public:
        DatabaseException(const std::string& errorSrc, const std::string& errorType, const std::string& msg) : std::runtime_error("DATABASE EXCEPTION: " + errorSrc + " | " + errorType + " - " + msg){};
};
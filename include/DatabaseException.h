#pragma once
#include <stdexcept>

class DatabaseException : public std::runtime_error{
    public:
        DatabaseException(const std::string& msg) : std::runtime_error("DATABASE EXCEPTION: " + msg){};
};
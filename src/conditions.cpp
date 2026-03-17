#include "sqlite3c++.h"
using QueryCondition = Database::QueryCondition;
using Value = Database::Value;



template <typename T>
QueryCondition operator<(const Value& val, T other){
    Database::QueryCondition condition("<", {val.toString(), std::to_string(other)});
    return condition;
}

template <typename T>
QueryCondition operator>(const Value& val, T other){
    Database::QueryCondition condition(">", {val.toString(), std::to_string(other)});
    return condition;
}

template <typename T>
QueryCondition operator<=(const Value& val, T other){
    Database::QueryCondition condition("<=", {val.toString(), std::to_string(other)});
    return condition;
}

template <typename T>
QueryCondition operator>=(const Value& val, T other){
    Database::QueryCondition condition(">=", {val.toString(), std::to_string(other)});
    return condition;
}

template <typename T>
QueryCondition operator==(const Value& val, T other){
    Database::QueryCondition condition("=", {val.toString(), std::to_string(other)});
    return condition;
}

template <typename T>
QueryCondition operator!=(const Value& val, T other){
    Database::QueryCondition condition("!=", {val.toString(), std::to_string(other)});
    return condition;
}

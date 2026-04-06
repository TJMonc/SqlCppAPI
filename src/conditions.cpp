#include "sqlite3c++.h"
using CQueryCondition = Database::CQueryCondition;
using Value = Database::Value;



 Database::CQueryCondition operator<(const Database::IntValue& v, int other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), "<");
}
 Database::CQueryCondition operator>(const Database::IntValue& v, int other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), ">");
}
 Database::CQueryCondition operator<=(const Database::IntValue& v, int other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), "<=");
}
 Database::CQueryCondition operator>=(const Database::IntValue& v, int other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), ">=");
}
 Database::CQueryCondition operator==(const Database::IntValue& v, int other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), "=");
}
 Database::CQueryCondition operator!=(const Database::IntValue& v, int other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), "!=");
}

 Database::ANDQueryCondition operator&&(Database::QueryCondition& v, Database::QueryCondition& other){
    return Database::ANDQueryCondition(v, other);
}

 Database::ORQueryCondition operator||(Database::QueryCondition& v, Database::QueryCondition& other){
    return Database::ORQueryCondition(v, other);
}



 Database::CQueryCondition operator<(const Database::FloatValue& v, double other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), "<");
}
 Database::CQueryCondition operator>(const Database::FloatValue& v, double other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), ">");
}
 Database::CQueryCondition operator<=(const Database::FloatValue& v, double other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), "<=");
}
 Database::CQueryCondition operator>=(const Database::FloatValue& v, double other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), ">=");
}
 Database::CQueryCondition operator==(const Database::FloatValue& v, double other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), "=");
}
 CQueryCondition operator!=(const Database::FloatValue& v, double other){
    return Database::CQueryCondition(v.fieldName, std::to_string(other), "!=");
}

 CQueryCondition operator==(const Database::TextValue& v, std::string other){

    std::string escaped = other;
    size_t pos = 0;
    while ((pos = escaped.find('\'', pos)) != std::string::npos){
        escaped.replace(pos, 1, "''");
        pos += 2;
    }
    return Database::CQueryCondition(v.fieldName, "'" + escaped + "'", "=");
}
 CQueryCondition operator!=(const Database::TextValue& v, std::string other){
    std::string escaped = other;
    size_t pos = 0;
    while ((pos = escaped.find('\'', pos)) != std::string::npos)
    {
        escaped.replace(pos, 1, "''");
        pos += 2;
    }
    return Database::CQueryCondition(v.fieldName, "'" + escaped + "'", "!=");
}

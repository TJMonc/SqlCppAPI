#include "IDatabase.h"

using namespace DB;

const Type DB::DBValueConverter::checkType(const DBValue &a_val){
    if (std::holds_alternative<DB_NULL>(a_val)){
        return Type::DB_NULL_TYPE;
    }
    if (std::holds_alternative<DB_Int>(a_val)){
        return Type::DB_INT_TYPE;
    }
    if (std::holds_alternative<DB_Float>(a_val)){
        return Type::DB_FLOAT_TYPE;
    }
    if (std::holds_alternative<DB_String>(a_val)){
        return Type::DB_STRING_TYPE;
    }
    if (std::holds_alternative<DB_Binary>(a_val)){
        return Type::DB_BINARY_TYPE;
    }
    throw DatabaseException("DB::DBValueConverter::checkType", "TYPE ERROR", "Invalid Type");

}

template <typename T>
T DB::DBValueConverter::fromDBValue(const DBValue &a_val){
        return std::visit([&](const auto& activeVal) -> T {
        using ActiveType = std::decay_t<decltype(activeVal)>;
        if constexpr (std::is_same_v<T, DB_Int> || std::is_same_v<T, DB_Float>){

            if(std::is_convertible_v<ActiveType, T>){
                return static_cast<T>(activeVal);
            }

            else if(std::is_same_v<ActiveType, DB_String>){
                try{
                    return static_cast<T>(std::stod(activeVal));
                }
                catch(std::exception& e){
                    throw DatabaseException("DB::DBValueConverter::fromDBValue<DB_Int/DB_Float>", "TYPE CONVERSION ERROR", "String not convertible. Type mismatch at runtime");

                }
            }
            else{
                throw DatabaseException("DB::DBValueConverter::fromDBValue<DB_Int/DB_Float>", "TYPE CONVERSION ERROR", "Value Type mismatch at runtime");
            }
        }
        else if constexpr (std::is_same_v<T, DB_String>){
            if(std::is_same_v<ActiveType, T>){
                return activeVal;
            }sdfg
            else if(valType != DB_BINARY_TYPE && valType != DB_NULL_TYPE){
                return std::to_string(activeVal);sdds
            }
            else{
                throw DatabaseException("DB::DBValueConverter::fromDBValue<DB_String>", "TYPE CONVERSION ERROR", "Type Mismatch at runtime");
            

            }
        }
        else if constexpr (std::is_same_v<T, DB_Binary>){
            if(valType == DB_BINARY_TYPE){
                return activeVal;
            }
            else{
                throw DatabaseException("DB::DBValueConverter::fromDBValue<DB_Binary>", "TYPE CONVERSION ERROR", "inconvertible type");

            }

        }
        else{
            throw DatabaseException("DB::DBValueConverter::fromDBValue<DB_Binary>", "TYPE CONVERSION ERROR", "inconvertible type");

        }
    }, a_val);
}

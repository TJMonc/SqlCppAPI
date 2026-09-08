#pragma once
#include "DatabaseException.h"
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <any>



namespace DB{
    using DB_Int = int64_t;
    using DB_String = std::string;
    using DB_Float = double;
    using DB_Binary = std::vector<char>;
    using DB_NULL = std::monostate;
    using DBValue = std::variant<DB_Int, DB_String, DB_Float, DB_Binary, DB_NULL>;

    enum Type {DB_INT_TYPE, DB_STRING_TYPE, DB_FLOAT_TYPE, DB_BINARY_TYPE, DB_NULL_TYPE};

    struct Row{
        std::unordered_map<std::string, DBValue> values;

        DBValue& operator[](const std::string& field){
            if(!values.contains(field)){
                throw DatabaseException("DB::Row::operator[]",  "OUT OF BOUNDS ERROR", "Row data indexed out of range. Row field does not exist.");
            }
            return values.at(field);
        }
        void insert(const std::pair<std::string, DBValue>& a_val){
            if(values.contains(a_val.first)){
                throw DatabaseException(
                    "DB::Row::insert",
                     "COLUMN NAME COLLISION ERROR",
                      "Result set cannot have duplicate column names. Column names potentially not properlly aliased\nConflicting Column Name: " + a_val.first);
            }
            values.insert(a_val);
        }

    };

    struct QuerySet{
        std::vector<std::string> colNames;
        std::vector<Type> colTypes;
        std::unordered_map<std::string, Type> typeMap;

        std::vector<Row> data;

        Row& operator[](const size_t index){
            if(data.size() <= index){
                throw DatabaseException("DB::QuerySet::operator[]", "OUT OF BOUNDS ERROR", "Query Set data indexed out of range.");
            }
            return data.at(index);
        }
        Type getType(std::string colName){
            if(typeMap.contains(colName)){
                return typeMap.at(colName);
            }
            else{
                throw DatabaseException("DB::QuerySet::getType", "OUT OF BOUNDS ERROR", "Query Set data indexed out of range. Column Name doesn't exist.");
            }
        }


    };

    class IDatabase {
        public:
            virtual ~IDatabase() = default;
            IDatabase() = default;
            IDatabase(const IDatabase& other) = delete;
            IDatabase(IDatabase&& other) = delete;

            virtual void execute(const std::string& a_query, const std::vector<DBValue>& params) = 0;
            virtual QuerySet select(const std::string& a_query, const std::vector<DBValue>& params) = 0;

    };

   
    struct DBValueConverter{
        template <typename T>
        static T fromDBValue(const DBValue& a_val){
            const Type valType = DBValueConverter::checkType(a_val);
            if constexpr (std::is_same_v<T, DB_Int> || std::is_same_v<T, DB_Float>){
                if(valType == DB_INT_TYPE){
                    return std::get<DB_Int>(a_val);
                }
                else if(valType == DB_FLOAT_TYPE){
                    return std::get<DB_Float>(a_val);
                }
                else if(valType == DB_STRING_TYPE){
                    try{
                        return std::stod(std::get<DB_String>(a_val));
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
                if(valType == DB_STRING_TYPE){
                    return std::get<DB_String>(a_val);
                }
                else if(valType != DB_BINARY_TYPE && valType != DB_NULL_TYPE){
                    if(valType == DB_INT_TYPE){
                        return std::to_string(std::get<DB_Int>(a_val));
                    }
                    if(valType == DB_FLOAT_TYPE){
                        return std::to_string(std::get<DB_Float>(a_val));
                    }
                }
                else if(isNull(a_val)){
                    return std::string("");
                }
                else{
                    throw DatabaseException("DB::DBValueConverter::fromDBValue<DB_String>", "TYPE CONVERSION ERROR", "Type Mismatch at runtime");

                }
            }
            else if constexpr (std::is_same_v<T, DB_Binary>){
                if(valType == DB_BINARY_TYPE){
                    return std::get<DB_Binary>(a_val);
                }
                throw DatabaseException("DB::DBValueConverter::fromDBValue<DB_Binary>", "TYPE CONVERSION ERROR", "Binary type not convertible");

            }
            else if constexpr (std::is_same_v<T, DB_NULL>){
                return std::monostate();
            }

            throw DatabaseException("DB::DBValueConverter::fromDBValue<T>", "TYPE CONVERSION ERROR", "Type not recognized");

        }
        template <typename T>
        static DBValue toDBValue(const T& a_val){
            try{
                return DBValue(a_val);
            }
            catch(std::exception& e){
                throw DatabaseException("DB::DBValueConverter<T>::toDBValue", "TYPE ERROR", "Invalid type");
            }
        };
        static const Type checkType(const DBValue& a_val) {
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

            throw DatabaseException("DB::DBValueConverter::checkType", "TYPE ERROR", "Value Type not recognized");
        }
        static bool isNull(const DBValue& a_val) { return std::holds_alternative<DB_NULL>(a_val); }


    };



}
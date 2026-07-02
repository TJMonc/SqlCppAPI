#pragma once
#include "DatabaseException.h"
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <optional>



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
    };

    struct QuerySet{
        std::vector<std::string> colNames;
        std::vector<Row> data;

        Row& operator[](const size_t index){
            if(data.size() <= index){
                throw DatabaseException("DB::QuerySet::operator[]", "OUT OF BOUNDS ERROR", "Query Set data indexed out of range.");
            }
            return data.at(index);
        }

    };

    class IDatabase {
        public:
            virtual ~IDatabase() = default;
            IDatabase() = default;
            IDatabase(const IDatabase& other) = delete;
            IDatabase(IDatabase&& other) = delete;

            virtual int execute(const std::string& a_query, const std::vector<DBValue>& params) = 0;
            virtual QuerySet select(const std::string& a_query, const std::vector<DBValue>& params) = 0;

    };

   
    struct DBValueConverter{
        template <typename T>
        static T fromDBValue(const DBValue& a_val);
        template <typename T>
        static DBValue toDBValue(const T& a_val){return DBValue();};

        static const Type checkType(const DBValue& a_val);
        static bool isNull(const DBValue& a_val) { return std::holds_alternative<DB_NULL>(a_val); }
    };

}
#pragma once
#include "DatabaseException.h"
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>



namespace DB{
    using Value = std::variant<int64_t, std::string, double, std::vector<char>, std::monostate>;

    struct Row{
        std::unordered_map<std::string, Value> values;

        Value& operator[](const std::string& field){
            if(!values.contains(field)){
                throw DatabaseException("Row data indexed out of range. Row field does not exist.");
            }
            return values.at(field);
        }
    };

    struct QuerySet{
        std::vector<std::string> colNames;
        std::vector<Row> data;

        Row& operator[](const size_t index){
            if(data.size() <= index){
                throw DatabaseException("Query Set data indexed out of range.");
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

            virtual int execute(const std::string& query, const std::vector<std::string>& params);
            virtual QuerySet select(const std::string& query, const std::vector<std::string>& params);
    };

}
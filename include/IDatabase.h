#pragma once
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>


namespace DB{
    using Value = std::variant<int64_t, double, std::vector<char>, std::string, std::monostate>;

    struct Row {
        std::unordered_map<std::string, Value> values;
    };
    struct ResultSet{
        std::vector<std::string> colNames;
        std::vector<Row> rows;
    };

    enum class Hint{
        UUID,
        Email,
        Timestamp,
        Date,
        Json
    };


    class IDatabase{
        public:
            virtual int execute(const std::string& a_query, const std::vector<Value>& a_vals) = 0;
            virtual ResultSet select(const std::string& a_query, const std::vector<Value>& a_vals) = 0;
            virtual bool supports(Hint h) = 0;

            IDatabase() = default;
            virtual ~IDatabase() = 0;
    };

    template<typename T>
    struct ValueConverter{
        static T fromIValue(const Value& a_val);

        static Value toIValue(const T& a_val);
    };

}

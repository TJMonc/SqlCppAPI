#include <iostream>
#include <optional>
#include <vector>
#include <memory>
#include "sqlite3.h"
#include <map>


class Database{
    public:
        enum Type {NONE, INT = SQLITE_INTEGER, DECIMAL = SQLITE_FLOAT, TEXT = SQLITE_TEXT, BLOB = SQLITE_BLOB, NULLVAL=SQLITE_NULL, BOOL};

    private:
        sqlite3* db = nullptr;
        char* errorMsg = nullptr;
        const std::string path;
        std::string sqlCode;
    public:

    class Value {

        public:
            //Column name
            const std::string fieldName;
            //
            const bool isUnique;

            //an integer representing the value type. {INT = SQLITE_INTEGER, TEXT = SQLITE_TEXT, DECIMAL = SQLITE_FLOAT, etc}
            const Type type;

            Value(std::string aFieldName, Type aType = NONE, bool aIsUnique = false);
            virtual ~Value() = default;
        public:

            virtual std::string getValue(){
                return fieldName;
            }
            virtual std::ostream& write(std::ostream& os){
                os << fieldName;
                return os;
            };
            friend std::ostream& operator<<(std::ostream& os, std::unique_ptr<Value>& val){
    
                return val->write(os);
            }

        };

    public:
        class TextValue : public Value{
            public:
            std::string value;
            TextValue(Value& other);
            TextValue(std::string fieldName, bool aIsUnique = false);
            TextValue() = delete;

            std::string getValue(){
                return value;
            }

            std::ostream& write(std::ostream& os){
                os << value;
                return os;
            }

        };
        class IntValue : public Value{
            public:
            int value;
            IntValue(Value& other);
            IntValue(std::string fieldName, bool aIsUnique = false);


            IntValue() = delete;

            std::string getValue(){
                return std::to_string(value);
            }
            std::ostream& write(std::ostream& os){
                os << value;
                return os;
            }
        };
        class DecimalValue : public Value{
            public:        
            double value;
            DecimalValue(Value& other);
            DecimalValue(std::string fieldName, bool aIsUnique = false);
            DecimalValue() = delete;

            std::string getValue(){
                return std::to_string(value);
            }

            std::ostream& write(std::ostream& os){
                os << value;
                return os;
            }
        };
        class BoolValue : public Value{
            public:
            bool value;
            BoolValue(Value& other);
            BoolValue() = delete;

            std::string getValue(){
                return std::to_string(value);
            }

            std::ostream& write(std::ostream& os){
                os << value;
                return os;
            }
        };
        class BlobValue : public Value{
        public:
            std::vector<std::byte> data;

            BlobValue(std::string aFieldName, const void* data, size_t size);

            BlobValue(Value& other);
            std::string getValue(){
                return std::string((const char*)data.data());
            }

            std::ostream& write(std::ostream& os){
                os << data.data();
                return os;
            }
        };

        //Note that None isn't a NULL value. It's meant for values that aren't analogs to the basic SQLITE values, like table names, column names, etc.
        class NoneValue : public Value{
            public:
            std::string value;
            NoneValue(std::string aVal);
            std::string getValue(){
                return value;
            }
        };
    private:
        class Record;

        class RecordContainer{
            private:
                std::vector<std::vector<Record>> records;
                std::string baseSqlCode;
            public:
                //Chains a query with AND
                RecordContainer& filter(std::string condition);
                //chains a query with OR
                RecordContainer& add(std::string condition);


        };
        public:
        class Model{
            //the table name
            std::string name;
            Database* db;
            IntValue id;

        public:
            int insert(std::vector<std::string> values, std::vector<std::string> columns = {""});
            virtual RecordContainer get(std::vector<std::string> fields, std::vector<std::pair<std::string, std::string>> conditionals = {{"", ""}});
            Model(std::string name, Database* db);

            //The makeValue functions are meant to be used in the constructor any derived classes for models in order to have something similar to the Field constructors in django

            static IntValue makeInt(std::string tableName, std::string fieldName, bool isUnique = false, bool isNull = true, int defaultVal = 0);
            static DecimalValue makeDecimal(std::string tableName, std::string fieldName, bool isUnique = false, bool isNull = true, double defaultVal = 0);
            static TextValue makeText(std::string tableName, std::string fieldName, bool isUnique = false, bool isNull = true, std::string defaultVal = "");
            static BlobValue makeBlob(std::string tableName, std::string fieldName);

        };
        private:

        class Record {
            Model& table;
        private:
            int insert(std::vector<std::string> values, std::vector<std::string> columns = {""});
            RecordContainer get(std::vector<std::string> fields, std::vector<std::pair<std::string, std::string>> conditionals = {{"", ""}});
        public:
            bool save();


        };

    public:

        //@param a file path.
        Database(std::string dbPath);
        Database() = delete;
        Database(Database&) = delete;
        ~Database();



        //Callback function for simple select queries
        static int callback(void* args, int argc, char** argv, char** colName);

        /*
        @brief Use for simple select queries. It's not recomended to use this for any binary data handling(images, videos, any files, etc)

        @return A vector of a vector of strings where the first row ([0][x]) are the column names and every thing else is the returned query data
        */
        std::vector<std::vector<std::optional<std::string>>> selectQuery(std::string query);

        /*
        @brief Parameterized select queries. It's generally recomended to use this over selectQuery
        @param query: any parametrized select query sql code. Parametrized must be marked with @identifier or ?.

        @return A vector of a vector of Values where the first row ([0][x]) are the column names and every thing else is the returned query data
        */
        std::vector<std::vector<std::unique_ptr<Value>>> preparedSelectQuery(std::string paramQuery, std::vector<std::string> vals = {});
        
        /*
            @brief Use for simple execute queries. It's not recomended to use this for any binary data handling(images, videos, any files, etc)

         *   @param query: any sql code. It's recomended to use selectQuery() for select queries though.
         *   @return The full sql string of code that will be executed upon calling execute()
        */
        bool query(std::string query);

        bool preparedQuery(std::string paramQuery, std::vector<std::string> vals);

};

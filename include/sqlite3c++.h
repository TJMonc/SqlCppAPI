#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include <memory>
#include <map>
#include <format>
#include <variant>
#include "sqlite3.h"


class Database{
    public:
        enum Type {NONE, INT = SQLITE_INTEGER, FLOAT = SQLITE_FLOAT, TEXT = SQLITE_TEXT, BLOB = SQLITE_BLOB, NULLVAL=SQLITE_NULL, BOOL};

    private:
        sqlite3* db = nullptr;
        char* errorMsg = nullptr;
        const std::string path;
        std::string sqlCode;
    public:

    class Value {

        public:
            //Column name
            const std::string fieldName = "";
            //
            const bool isUnique = false;

            //an integer representing the value type. {INT = SQLITE_INTEGER, TEXT = SQLITE_TEXT, FLOAT = SQLITE_FLOAT, etc}
            const Type type = NONE;

            Value(std::string aFieldName, Type aType = NONE, bool aIsUnique = false);
            Value& operator=(const Value&) = delete;

            Value() = default;

            virtual ~Value() = default;
        public:

            virtual std::string toString(){
                return fieldName;
            }

            std::string getValue(){
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
            TextValue() = default;


            std::string toString(){
                return value;
            }

            std::string& getValue(){return value; }

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
            IntValue() = default;
            

            std::string toString(){
                return std::to_string(value);
            }

            int& getValue(){return value; }

            std::ostream& write(std::ostream& os){
                os << value;
                return os;
            }
        };
        class FloatValue : public Value{
            public:        
            double value;
            FloatValue(Value& other);
            FloatValue(std::string fieldName, bool aIsUnique = false);

            double& getValue(){return value; }

            std::string toString(){
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
            BoolValue() = default;


            std::string toString(){
                return std::to_string(value);
            }

            bool getValue(){
                return value;
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
            BlobValue() = default;

            std::vector<std::byte>& getValue(){
                return data;
            }
            std::string toString(){
                return "N/A";
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
            std::string toString(){
                return value;
            }

            std::string& getValue(){
                return value;
            }
        };
    private:
        class Record;

        class RecordContainer{
            private:
                std::vector<std::unique_ptr<Record>> records;
                std::string baseSqlCode;
            public:
                //Chains a query with AND
                RecordContainer& filter(std::string condition);
                //chains a query with OR
                RecordContainer& add(std::string condition);

                //Sorts the record container in ascending order
                RecordContainer& sortAsc();
                //sorts the record container in descending order
                RecordContainer& sortDesc();

                Record& at(const int index);
                Record& operator[](const int index);



        };
        public:
        class Model{
            //the table name
            public:
            std::string name;
            Database* db;
            IntValue id;

        public:
            std::vector<std::unique_ptr<Value>> fields;

            int insert(std::vector<std::string> values, std::vector<std::string> columns = {});
            RecordContainer get(std::vector<std::string> fields, std::vector<std::pair<std::string, std::string>> conditionals = {{"", ""}});
            Model(std::string name);

            //The makeValue functions are meant to be used in the constructor any derived classes for models in order to create unique tables without rewriting the whole class

            IntValue& makeInt(Database& db, std::string tableName, std::string fieldName, bool isUnique = false, bool isNull = true, int defaultVal = 0);
            FloatValue& makeFloat(Database& db, std::string tableName, std::string fieldName, bool isUnique = false, bool isNull = true, double defaultVal = 0);
            TextValue& makeText(Database& db, std::string tableName, std::string fieldName, bool isUnique = false, bool isNull = true, std::string defaultVal = "");
            BlobValue& makeBlob(Database& db, std::string tableName, std::string fieldName);

        };
        private:

        class Record {
            Model& table;
        public:
            bool save();
            Model* operator->(){return &table;};
            Record(Model& table);

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
        int query(std::string query);

        int preparedQuery(std::string paramQuery, std::vector<std::string> vals);

        int preparedQuery(std::string paramQuery, std::vector<Value*> vals);

        //Returns the results of PRAGMA table_info( 'table_name' )
        std::vector<std::vector<std::optional<std::string>>> getTableInfo(std::string tableName);

        //checks if a field exists in a table
        bool doesColumnExist(std::string tableName, std::string columnName);

        static std::variant<TextValue*, IntValue*, FloatValue*, BlobValue*, NoneValue*> convert(Value& val);

};


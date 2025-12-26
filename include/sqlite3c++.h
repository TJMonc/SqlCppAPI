#include <iostream>
#include <optional>
#include <vector>
#include <memory>
#include "sqlite3.h"
#include <map>


class Database{
    private:
        sqlite3* db;
        char* errorMsg;
        std::string path;
        std::string sqlCode;

    class Value {
        protected:
            std::string tableName;
            std::string fieldName;

            Database *db;
            enum Type {TEXT, INT, DECIMAL, BOOL};
            Type type;
        public:
            //returns an integer representing the value type. {TEXT = 0, INT = 1, DECIMAL = 2, BOOL = 3}
            Type getType();

        };

    public:
        class TextValue : virtual Value{
            public:
            std::string value;
        };
        class IntValue : virtual Value{
            public:

            int value;
        };
        class DecimalValue : virtual Value{
            public:        
            double value;
        };
        class BoolValue : virtual Value{
            public:
           
            bool value;
        };

    private:
        class Record;

        class RecordContainer{
            private:
                std::vector<std::vector<std::unique_ptr<Record>>> records;
                std::string baseSqlCode;
            public:
                //Chains a query with AND
                RecordContainer filter(std::string condition);
                //chains a query with OR
                RecordContainer add(std::string condition);


        };
        public:
        class Model{
            //the table name
            std::string name;
            Database* db;
            IntValue id;

        public:
            virtual int insert(std::vector<std::string> values, std::vector<std::string> columns = {""});
            virtual RecordContainer get(std::vector<std::string> fields, std::vector<std::pair<std::string, std::string>> conditionals = {{"", ""}});
            Model(std::string name, Database* db);
        };
        private:
        class Record : public Model{
        private:
            int insert(std::vector<std::string> values, std::vector<std::string> columns = {""}) override;
            RecordContainer get(std::vector<std::string> fields, std::vector<std::pair<std::string, std::string>> conditionals = {{"", ""}}) override;
        public:
            bool save();


        };



    public:

        //@param a file path.
        Database(std::string dbPath);
        Database() = delete;
        Database(Database&) = delete;
        ~Database();


        static int callback(void* args, int argc, char** argv, char** colName);

        /*
        @param query: any parametrized select query sql code. Parametrized must be marked with @identifier or ?.

        @return A vector of a vector of strings where the first row ([0][x]) are the column names and every thing else is the returned query data
        */
        std::vector<std::vector<std::string>> selectQuery(std::string query);
        
        /*
         *   @param query: any sql code. It's recomended to use selectQuery() for select queries though.
         *   @return The full sql string of code that will be executed upon calling execute()
        */
        bool query(std::string query);

};

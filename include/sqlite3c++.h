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

        struct QueryCondition{
                std::string condition;
                std::vector<std::string> values;

                QueryCondition(std::string aCondition, std::vector<std::string> aVals) : condition(aCondition), values(aVals) {};
                QueryCondition() = delete;
                
            

        };

    class Value {

        public:
            //Column name
            std::string fieldName = "";
            //
            bool isUnique = false;

            //an integer representing the value type. {INT = SQLITE_INTEGER, TEXT = SQLITE_TEXT, FLOAT = SQLITE_FLOAT, etc}
            Type type = NONE;

            Value(std::string aFieldName, Type aType = NONE, bool aIsUnique = false);

            Value() = default;

            Value& operator=(Value& other){
                return clone(other);
            }

            virtual Value& clone(Value& other) {

                if(this->type == other.type){
                    this->fieldName = other.fieldName;
                    this->isUnique = other.isUnique;
                    this->type = other.type;
                }
                else{
                    throw std::runtime_error("Type mismatch");
                }
                return *this;
            }

            virtual ~Value() = default;
        public:

            virtual std::string toString() const{
                return fieldName;
            }

            std::string getValue(){
                return fieldName;
            }


            virtual std::ostream& write(std::ostream& os){
                os << fieldName;
                return os;
            };

            friend std::ostream& operator<<(std::ostream& os, Value& val){
    
                return val.write(os);
            }

        };


    public:
        class TextValue : public Value{
            public:
            std::string value;
            TextValue(Value& other);
            TextValue(std::string fieldName, bool aIsUnique = false);
            TextValue() = default;

            operator std::string() const{
                return value;

            }




            Value& clone(Value& other) {

                if(this->type == other.type){
                    TextValue& otherVal = static_cast<TextValue&>(other);
                    this->fieldName = other.fieldName;
                    this->isUnique = other.isUnique;
                    this->type = other.type;
                    this->value = otherVal.value;
                }
                else{
                    throw std::runtime_error("Type mismatch");
                }
                return *this;
            }

            Value& operator=(const std::string& other){
                this->value = other;
                return *this;
            }

            std::string toString() const{
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

            Value& operator=(const int& other){
                this->value = other;
                return *this;
            }

            operator int() const{
                return value;
            }


            
            Value& clone(Value& other) {

                if(this->type == other.type){
                    IntValue& otherVal = static_cast<IntValue&>(other);
                    this->fieldName = other.fieldName;
                    this->isUnique = other.isUnique;
                    this->type = other.type;
                    this->value = otherVal.value;
                }
                else{
                    throw std::runtime_error("Type mismatch");
                }
                return *this;
            } 

            std::string toString() const{
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

            operator double() const{
                return value;
            }

            Value& operator=(const double& other){
                this->value = other;
                return *this;
            }

            Value& clone(Value& other) {

                if(this->type == other.type){
                    FloatValue& otherVal = static_cast<FloatValue&>(other);
                    this->fieldName = other.fieldName;
                    this->isUnique = other.isUnique;
                    this->type = other.type;
                    this->value = otherVal.value;
                }
                else{
                    throw std::runtime_error("Type mismatch");
                }
                return *this;
            } 
            double& getValue(){return value; }

            std::string toString() const{
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

            operator bool() const{
                return value;
            }

            Value& operator=(const bool& other){
                this->value = other;
                return *this;
            }

            Value& clone(Value& other) {

                if(this->type == other.type){
                    BoolValue& otherVal = static_cast<BoolValue&>(other);
                    this->fieldName = other.fieldName;
                    this->isUnique = other.isUnique;
                    this->type = other.type;
                    this->value = otherVal.value;
                }
                else{
                    throw std::runtime_error("Type mismatch");
                }
                return *this;
            } 
            std::string toString(){
                return std::to_string(value);
            }

            bool& getValue(){
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

            operator std::vector<std::byte>() const{
                return data;
            }

            Value& clone(Value& other) {

                if(this->type == other.type){
                    BlobValue& otherVal = static_cast<BlobValue&>(other);
                    this->fieldName = other.fieldName;
                    this->isUnique = other.isUnique;
                    this->type = other.type;
                    this->data = otherVal.data;
                }
                else{
                    throw std::runtime_error("Type mismatch");
                }
                return *this;
            }

            Value& operator=(const std::vector<std::byte>& other){
                this->data = other;
                return *this;
            }

            std::vector<std::byte>& getValue(){
                return data;
            }
            std::string toString() const{
                return "Cannot turn blob value into string";
            }

            std::ostream& write(std::ostream& os){
                os.write((char*)data.data(), data.size());
                return os;
            }
        };

        //Note that None isn't a NULL value. It's meant for values that aren't analogs to the basic SQLITE values, like table names, column names, etc.
        class NoneValue : public Value{
            public:
            std::string value;
            NoneValue(std::string aVal);
            std::string toString() const{
                return value;
            }

            Value& operator=(const std::string& other){
                this->value = other;
                return *this;
            }

            std::string& getValue(){
                return value;
            }
        };
    public:
        class Record;
        class Model;

        class RecordContainer{
            private:
                std::vector<std::unique_ptr<Record>> records;
                std::vector<int> ids;
                std::string baseSqlCode;
                std::vector<std::string> values;
                Model& model;
            public:
                RecordContainer(Model& aModel, std::string baseSql, std::vector<std::string> values);
                //Chains a query with AND
                RecordContainer filter(std::string var, char op = '=', std::string condition = "0");
                //chains a query with OR
                RecordContainer add(std::string condition);

                //Sorts the record container in ascending order
                RecordContainer sortAsc();
                //sorts the record container in descending order
                RecordContainer sortDesc();

                size_t size(){ return records.size(); }
                void removeAll();

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
            int remove(IntValue& aId);
            int remove(int aID);
            int remove(std::string condition);

            RecordContainer get();
            Model(Database* aDb, std::string name);

            //The makeValue functions are meant to be used in the constructor any derived classes for models in order to create unique tables without rewriting the whole class

            IntValue& makeInt(Database& db, std::string tableName, std::string fieldName, bool isUnique = false, bool isNull = true, int defaultVal = 0);
            FloatValue& makeFloat(Database& db, std::string tableName, std::string fieldName, bool isUnique = false, bool isNull = true, double defaultVal = 0);
            TextValue& makeText(Database& db, std::string tableName, std::string fieldName, bool isUnique = false, bool isNull = true, std::string defaultVal = "");
            BlobValue& makeBlob(Database& db, std::string tableName, std::string fieldName);

        };

        class Record {
            Model& table;
            std::map<std::string, std::unique_ptr<Value>> fields;
            IntValue id = IntValue("", 0);
        public:
            friend class RecordContainer;
            bool save();
            int remove();
            inline Record(Model& aTable) : table(aTable) {
                id = table.id;
                fields.insert({id.fieldName, std::make_unique<IntValue>(id)});
                for(size_t i = 0; i < table.fields.size(); i++) {
                    fields.insert({table.fields.at(i)->fieldName, valToPtr(*table.fields.at(i))});
                }
            };


            template <typename T> T& operator[](const T& val){
                auto ptr = dynamic_cast<T*>(fields.at(val.fieldName).get());

                if(!ptr){
                    throw std::runtime_error("Type not recognized");
                }
                return *ptr;
            }

            template <typename T> const T& operator[](const T& val) const {
                auto ptr = dynamic_cast<const T*>(fields.at(val.fieldName).get());

                if(!ptr){
                    throw std::runtime_error("Type not recognized");
                }
                return (const T&)*ptr;
            }

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

        static Value& convertVal(Value& val){
            switch(val.type){
                case TEXT:{

                    return (TextValue&)val;
                    
                    break;
                }
                case INT:{
                    return (IntValue&)val;

                    break;
                }
                case FLOAT:{

                    return (FloatValue&)val;

                    break;
                }
                case BOOL:{

                    return (BoolValue&)val;

                    
                    break;
                }
                case BLOB:{

                    return (BlobValue&)val;
                    break;
                }
                case NONE:{

                    return (NoneValue&)val;
                    break;
                }
                case NULLVAL:{
                    return (NoneValue&)val;
                    break;
                }
                default:
                    throw std::runtime_error("ERROR: Could not disern type.");

        }
    }

    static std::unique_ptr<Value> valToPtr(Value& val){
        std::unique_ptr<Value> ptr = nullptr;
        switch(val.type){
            case TEXT:{
                ptr = std::make_unique<TextValue>(val);
                break;
            }
            case INT:{
                ptr = std::make_unique<IntValue>(val);

                break;
            }
            case FLOAT:{

                ptr = std::make_unique<FloatValue>(val);

                break;
            }
            case BOOL:{

                ptr = std::make_unique<BoolValue>(val);

                
                break;
            }
            case BLOB:{

                ptr = std::make_unique<BlobValue>(val);
                break;
            }
            case NONE:{

                ptr = std::make_unique<NoneValue>(val.fieldName);
                break;
            }
            case NULLVAL:{
                ptr = std::make_unique<NoneValue>("");
                break;
            }
            default:
                throw std::runtime_error("ERROR: Could not disern type.");

        }
        return ptr;
    }
};


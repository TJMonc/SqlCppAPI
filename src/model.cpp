#include "sqlite3c++.h"

int Database::Model::insert(std::vector<std::string> values, std::vector<std::string> columns) {
    std::string query = "INSERT INTO " + this->name;
    auto tableInfo = db->getTableInfo(name);

    if(values.size() > tableInfo.size()){
        throw std::runtime_error("Values size must not excede the amount of columns in the table");
    }

    if(columns.size() != 0){
        if(values.size() != columns.size() && values.size() != (tableInfo.size() - 1)){
            throw std::runtime_error("columns vector's size must be equal to values vector's size");
        }

        query += " (";
        for(size_t i = 0; i < columns.size(); i++){
            query += (" " + columns.at(i));

            if(i != columns.size() - 1){
                query += ", ";
            }
        }
        query += " )";
    }
    else if(values.size() != (tableInfo.size() - 1) && values.size() != (tableInfo.size() - 2)){
        throw std::runtime_error("Values size must equal the amount of columns in the table if a second argument isn't passed");
    }

    query += " VALUES (";
    for(size_t i = 0; i < values.size(); i++){
        query += (" ?");

        if(i != values.size() - 1){
            query += ", ";
        }
    }
    query += " )";



    return db->preparedQuery(query, values);
}

int Database::Model::remove(IntValue& aId)
{
    std::string query = std::format("DELETE FROM '{}' WHERE {} = ?", name, id.fieldName);
    std::vector<Value*> vals = {&aId};

    return db->preparedQuery(query, vals);
}

int Database::Model::remove(int aID)
{
    std::string query = std::format("DELETE FROM '{}' WHERE {} = ?", name, id.fieldName);
    std::vector<std::string> vals = {std::to_string(aID)};

    return db->preparedQuery(query, vals);}

int Database::Model::remove(std::string condition)
{
    std::string query = std::format("DELETE FROM '{}' WHERE {}", name, condition);

    return db->query(query);
}

Database::RecordContainer Database::Model::get() {
    std::string baseSql = "SELECT * FROM " + name;

    return RecordContainer(*this, baseSql, {});
}

Database::Model::Model(Database* aDb, std::string name) : db(aDb), id(name + "id", true), name(name)
{
    
    db->query(std::format("CREATE TABLE IF NOT EXISTS '{}' ( {} INTEGER PRIMARY KEY AUTOINCREMENT )", name, name + "id"));
}

Database::IntValue& Database::Model::makeInt(Database& db, std::string tableName, std::string fieldName, bool isUnique, bool isNull, int defaultVal) {
    IntValue integer(fieldName, isUnique);


    if(!db.doesColumnExist(tableName, fieldName)){
        std::string query = std::format("ALTER TABLE '{}' ADD COLUMN {} INTEGER", tableName, fieldName);
        std::cout << "\n" + query + "\n";

        if(isUnique){
            query += " UNIQUE";
        }
        if(!isNull){
            query += " NOT NULL";
        }
        query += " DEFAULT " + std::to_string(defaultVal);
        query += ";";

        db.query(query);
    }
    this->fields.push_back(std::make_unique<IntValue>(integer));
    IntValue* ptr = dynamic_cast<IntValue*>(fields.back().get());

    return *ptr;
}

Database::TextValue& Database::Model::makeText(Database& db, std::string tableName, std::string fieldName, bool isUnique, bool isNull, std::string defaultVal){
    TextValue text(fieldName, isUnique);

    if(!db.doesColumnExist(tableName, fieldName)){
        std::string query = std::format("ALTER TABLE '{}' ADD COLUMN {} TEXT", tableName, fieldName);
        std::cout << "\n" + query + "\n";

        if(isUnique){
            query += " UNIQUE";
        }
        if(!isNull){
            query += " NOT NULL";
        }
        query += " DEFAULT '" + defaultVal + "'";
        query += ";";

        db.query(query);

    }
    else{
        std::cout << "Column exists";
    }
    
    this->fields.push_back(std::make_unique<TextValue>(text));
    TextValue* ptr = dynamic_cast<TextValue*>(fields.back().get());

    return *ptr;
}

Database::FloatValue& Database::Model::makeFloat(Database& db, std::string tableName, std::string fieldName, bool isUnique, bool isNull, double defaultVal){
    FloatValue floatVal(fieldName, isUnique);

    if(!db.doesColumnExist(tableName, fieldName)){
        std::string query = std::format("ALTER TABLE '{}' ADD COLUMN {} REAL", tableName, fieldName);

        if(isUnique){
            query += " UNIQUE";
        }
        if(!isNull){
            query += " NOT NULL";
        }
        query += " DEFAULT '" + std::to_string(defaultVal) + "'";
        query += ";";

        db.query(query);
    }

    this->fields.push_back(std::make_unique<FloatValue>(floatVal));

    FloatValue* ptr = dynamic_cast<FloatValue*>(fields.back().get());

    return *ptr;
}

Database::BlobValue& Database::Model::makeBlob(Database& db, std::string tableName, std::string fieldName){
    BlobValue blob(fieldName, nullptr, 0);

    if(!db.doesColumnExist(tableName, fieldName)){
        std::string query = std::format("ALTER TABLE '{}' ADD COLUMN {} BLOB", tableName, fieldName);

        query += ";";

        db.query(query);
    }
    this->fields.push_back(std::make_unique<BlobValue>(blob));

    BlobValue* ptr = dynamic_cast<BlobValue*>(fields.back().get());

    return *ptr;
}

bool Database::Record::save() {
    std::string query = "UPDATE " + table.name + " SET";

    std::vector<Value*> values;

    for(auto& vals : fields){
        query += std::format(" {} = ?,", vals.first);
        values.push_back(vals.second.get());
    }
    values.push_back(&id);
    query.pop_back();

    query += " WHERE " + id.fieldName + " = ?"; 
    if(table.db->preparedQuery(query, values) == SQLITE_OK){
        return true;
    }
    else{
        return false;
    }
}

int Database::Record::remove() {
    std::string query = std::format("REMOVE FROM {} WHERE {} = ?", table.name, id.fieldName);
    return table.db->preparedQuery(query, {&id});
}

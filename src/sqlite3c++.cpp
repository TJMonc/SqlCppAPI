#include "sqlite3c++.h"

Database::Database(std::string dbPath): path(dbPath){
    if(sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK){
        throw std::runtime_error("SQLITE ERROR: Database not opened");
    }
    
}
Database::~Database(){
    sqlite3_close_v2(db);
    if(errorMsg != nullptr){
        sqlite3_free(errorMsg);
    }


}

int Database::callback(void *args, int argc, char **argv, char **colName)
{
    auto* queryDat = (std::vector<std::vector<std::optional<std::string>>>*)args;
    std::vector<std::optional<std::string>> temp;
    if(queryDat->size() == 0){

        for(int i = 0; i < argc; i++){
            temp.push_back(colName[i]);
        }
        queryDat->push_back(temp);
        temp.clear();
    }

    for(int i = 0; i < argc; i++){
        if(argv[i] != nullptr){
            temp.push_back(argv[i]);
        }
        else{
            temp.push_back(std::nullopt);
        }
    }
    queryDat->push_back(temp);
    temp.clear();


    return 0;
}

std::vector<std::vector<std::optional<std::string>>> Database::selectQuery(std::string query)
{
    std::vector<std::vector<std::optional<std::string>>> data;

    int rc = sqlite3_exec(db, query.c_str(), callback, (void*)&data, &errorMsg);

    if(rc != SQLITE_OK){
        sqlite3_free(errorMsg);

        
        throw std::runtime_error(errorMsg);
    }

    return data;
}

std::vector<std::vector<std::unique_ptr<Database::Value>>> Database::preparedSelectQuery(std::string paramQuery, std::vector<std::string> vals)
{
    std::vector<std::vector<std::unique_ptr<Value>>> result;

    std::vector<std::unique_ptr<Value>> row;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, paramQuery.c_str(), -1, &stmt, NULL);
    
    if(rc != SQLITE_OK){
        sqlite3_finalize(stmt);

        throw std::runtime_error(sqlite3_errmsg(db));
    }

    const int columnCount = sqlite3_column_count(stmt);

    for(int i = 0; i < columnCount; i++){
        std::string colName = sqlite3_column_name(stmt, i);

        row.emplace_back(std::make_unique<NoneValue>(colName));


    }

    result.emplace_back(std::move(row));
    row = std::vector<std::unique_ptr<Value>>();


    for(size_t i = 0; i < vals.size(); i++){

        sqlite3_bind_text(stmt, i + 1, vals.at(i).c_str(), -1, SQLITE_TRANSIENT);
        
        
    }

    while(sqlite3_step(stmt) == SQLITE_ROW){
        for(int i = 0; i < columnCount; i++){
            switch(sqlite3_column_type(stmt, i)){
                case TEXT:{

                    Value val(result.at(0).at(i)->fieldName, NULLVAL);

                    auto text = std::make_unique<TextValue>(val);

                   const char* ptr = (const char*)sqlite3_column_text(stmt, i);

                    if(ptr != nullptr){
                        text->value = ptr;
                        row.emplace_back(std::move(text));

                    }
                    else{
                        row.emplace_back(std::make_unique<Value>(val));
                    }

                    break;
                }
                case INT:{
                    Value val(result.at(0).at(i)->fieldName, NULLVAL);
                    auto integer = std::make_unique<IntValue>(val);

                    integer->value = sqlite3_column_int(stmt, i);

                    row.emplace_back(std::move(integer));
                    break;
                }
                case DECIMAL:{
                    Value val(result.at(0).at(i)->fieldName, NULLVAL);
                    auto real = std::make_unique<DecimalValue>(val);

                    real->value = sqlite3_column_double(stmt, i);

                    row.emplace_back(std::move(real));
                    break;
                }
                case BLOB:{
                    const void* blob = sqlite3_column_blob(stmt, i);
                    int size = sqlite3_column_bytes(stmt, i);

                    
                    row.emplace_back(std::make_unique<BlobValue>(result.at(0).at(i)->fieldName, blob, size));
                
                    break;
                }
                default:{
                    sqlite3_finalize(stmt);

                    throw std::runtime_error("ERROR: Could not parse type");
                }
            }
        }
        result.emplace_back(std::move(row));
        row = std::vector<std::unique_ptr<Value>>();
    }
    sqlite3_finalize(stmt);

    return result;
}

bool Database::query(std::string query){
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &errorMsg);

    if(rc != SQLITE_OK){
         sqlite3_free(errorMsg);

        throw std::runtime_error(errorMsg);
    }

    return true;
}

bool Database::preparedQuery(std::string paramQuery, std::vector<std::string> vals){
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, paramQuery.c_str(), -1, &stmt, NULL);

    if(rc != SQLITE_OK){
        sqlite3_finalize(stmt);

        throw std::runtime_error("Prepared Statement failed");
    }

    for(int i = 0; i < vals.size(); i++){
        sqlite3_bind_text(stmt, i + 1, vals.at(i).c_str(), -1, SQLITE_TRANSIENT);
    }

    while((rc = sqlite3_step(stmt)) == SQLITE_ROW);

    if(rc != SQLITE_DONE){
        sqlite3_finalize(stmt);

        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);

    return true;
}



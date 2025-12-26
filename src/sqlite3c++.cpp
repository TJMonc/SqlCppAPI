#include "sqlite3c++.h"

Database::Database(std::string dbPath): path(dbPath){
    if(sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK){
        throw std::runtime_error("SQLITE ERROR: Database not opened");
    }
    
}
Database::~Database(){
    sqlite3_close(db);
    sqlite3_free(errorMsg);


}

int Database::callback(void *args, int argc, char **argv, char **colName)
{
    auto* queryDat = (std::vector<std::vector<std::string>>*)args;
    std::vector<std::string> temp;
    if(queryDat->size() == 0){

        for(int i = 0; i < argc; i++){
            temp.push_back(colName[i]);
        }
        queryDat->push_back(temp);
        temp.clear();
    }

    for(int i = 0; i < argc; i++){
        temp.push_back(argv[i]);
    }
    queryDat->push_back(temp);
    temp.clear();


    return 0;
}

std::vector<std::vector<std::string>> Database::selectQuery(std::string query)
{
    std::vector<std::vector<std::string>> data;

    int rc = sqlite3_exec(db, query.c_str(), callback, (void*)&data, &errorMsg);

    if(rc != SQLITE_OK){
        
        throw std::runtime_error(errorMsg);
    }

    return data;
}

bool Database::query(std::string query){
    return 0;
}

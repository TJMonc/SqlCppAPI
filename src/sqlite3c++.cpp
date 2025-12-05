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

std::vector<std::pair<std::string, std::vector<std::string>>> Database::selectQuery(std::string query, std::vector<std::string> paramValues, std::vector<std::string> paramIDs = {""}){
    
}

std::string Database::query(std::string query){

}

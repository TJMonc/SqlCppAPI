#include <iostream>
#include <sqlite3.h>
#include <format>
#include "sqlite3c++.h"


int main(int argc, char** argv){
    sqlite3* db;
    sqlite3_stmt* stmt;

    if(sqlite3_open("test.db", &db) != SQLITE_OK){
        throw std::runtime_error("LOL");
    };
    std::string sql = R"(SELECT * FROM )";

    Database k("db");
    k.query("D");
     



}
#include "SQLiteDatabase.h"
#include <string.h>

DB::SQLiteDatabase::~SQLiteDatabase()
{
}

DB::SQLiteDatabase::SQLiteDatabase(const std::string &dbPath)
{
    if(sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK){
        throw DatabaseException("DB::SQLiteDatabase::SQLiteDatabase", "FILE ERROR", sqlite3_errmsg(db));
    }
}

DB::SQLiteDatabase::SQLiteDatabase(const SQLiteDatabase &other) {
    const char* schemaName = sqlite3_db_name(other.db, 0);
    if(schemaName == NULL){
        throw DatabaseException("DB::SQLiteDatabase::SQLiteDatabase - Copy Constructor", "COPY ERROR", sqlite3_errmsg(db));
    }

    const char* dbPath = sqlite3_db_filename(other.db, schemaName);
    if(dbPath == NULL || strcmp(dbPath, "") == 0){
        throw DatabaseException("DB::SQLiteDatabase::SQLiteDatabase - Copy Constructor", "COPY ERROR", sqlite3_errmsg(db));
    }

    if(sqlite3_open(dbPath, &this->db) != SQLITE_OK){
        throw DatabaseException("DB::SQLiteDatabase::SQLiteDatabase - Copy Constructor", "FILE ERROR", sqlite3_errmsg(db));
    }

}

DB::SQLiteDatabase::SQLiteDatabase(SQLiteDatabase &&other) {
    this->db = other.db;
    other.db = nullptr;
}

int DB::SQLiteDatabase::execute(const std::string &query, const std::vector<DBValue> &params)
{
    return 0;
}

DB::QuerySet DB::SQLiteDatabase::select(const std::string &query, const std::vector<DBValue> &params)
{
    QuerySet resultSet;
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    if(rc != SQLITE_OK){
        sqlite3_finalize(stmt);

        throw DatabaseException("SB::SQLiteDatabase::select", "QUERY ERROR", sqlite3_errmsg(db));
    }

    const int columnCount = sqlite3_column_count(stmt);

    for(int i = 0; i < columnCount; i++){
        resultSet.colNames.push_back(sqlite3_column_name(stmt, i));
    }

    Row row;
    for(int i = 1; i < params.size() + 1; i++){
        
    }

    return QuerySet();
}

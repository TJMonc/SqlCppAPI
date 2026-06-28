#include "SQLiteDatabase.h"
#include <string.h>

DB::SQLiteDatabase::SQLiteDatabase(const std::string &dbPath) {
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
        throw DatabaseException("DB::SQLiteDatabase::SQLiteDatabase", "FILE ERROR", sqlite3_errmsg(db));
    }

}

DB::SQLiteDatabase::SQLiteDatabase(SQLiteDatabase &&other) {
    this->db = other.db;
    other.db = nullptr;
}



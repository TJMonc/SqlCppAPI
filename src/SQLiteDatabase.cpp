#include "SQLiteDatabase.h"
#include <string.h>
#include <iostream>

DB::SQLiteDatabase::~SQLiteDatabase(){
    if(db != nullptr){
        sqlite3_close(db);
    }
    db = nullptr;
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
        throw DatabaseException("DB::SQLiteDatabase::SQLiteDatabase - Copy Constructor", "COPY ERROR", "Could not get Schema name for copied database");
    }

    const char* dbPath = sqlite3_db_filename(other.db, schemaName);
    if(dbPath == NULL || strcmp(dbPath, "") == 0){
        throw DatabaseException("DB::SQLiteDatabase::SQLiteDatabase - Copy Constructor", "COPY ERROR", sqlite3_errmsg(other.db));
    }

    if(sqlite3_open(dbPath, &this->db) != SQLITE_OK){
        throw DatabaseException("DB::SQLiteDatabase::SQLiteDatabase - Copy Constructor", "FILE ERROR", sqlite3_errmsg(other.db));
    }

}

DB::SQLiteDatabase::SQLiteDatabase(SQLiteDatabase &&other) {
    this->db = other.db;
    other.db = nullptr;
}

void DB::SQLiteDatabase::execute(const std::string &query, const std::vector<DBValue> &params) {
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    if(rc != SQLITE_OK){
        sqlite3_finalize(stmt);
        throw DatabaseException("SB::SQLiteDatabase::execute", "QUERY ERROR", sqlite3_errmsg(db));

    }

    for(int i = 0; i < params.size(); i++){
        auto& val = params.at(i);

        Type valType = DBValueConverter::checkType(val);
        switch(valType){
            case Type::DB_NULL_TYPE: {
                sqlite3_bind_null(stmt, i + 1);
                break;
            }
            case Type::DB_STRING_TYPE: {
                DB_String convertedType = DBValueConverter::fromDBValue<DB_String>(val);
                sqlite3_bind_text(stmt, i + 1, convertedType.c_str(), convertedType.size(), SQLITE_TRANSIENT);
                break;
            }
            case Type::DB_INT_TYPE: {
                DB_Int convertedType = DBValueConverter::fromDBValue<DB_Int>(val);
                sqlite3_bind_int64(stmt, i + 1, convertedType);
                break;
            }
            case Type::DB_FLOAT_TYPE: {
                DB_Float convertedType = DBValueConverter::fromDBValue<DB_Float>(val);
                sqlite3_bind_double(stmt, i + 1, convertedType);
                break;
            }
            case Type::DB_BINARY_TYPE: {
                DB_Binary convertedType = DBValueConverter::fromDBValue<DB_Binary>(val);
                sqlite3_bind_blob(stmt, i + 1, static_cast<const void*>(convertedType.data()), convertedType.size(), SQLITE_TRANSIENT);
                break;
            }
            default:{
                sqlite3_finalize(stmt);
                throw DatabaseException("DB::SQLiteDatabase::select", "TYPE ERROR", "Could not deduce Parameter types.");
            }
        }

    }

    while((rc = sqlite3_step(stmt)) == SQLITE_ROW);

    if(rc != SQLITE_DONE){
        sqlite3_finalize(stmt);

        throw DatabaseException("DB::SQLiteDatabase::execute", "QUERY ERROR", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

DB::QuerySet DB::SQLiteDatabase::select(const std::string &query, const std::vector<DBValue> &params) {
    QuerySet resultSet;
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    if(rc != SQLITE_OK){
        sqlite3_finalize(stmt);

        throw DatabaseException("DB::SQLiteDatabase::select", "QUERY ERROR", sqlite3_errmsg(db));
    }

    const int columnCount = sqlite3_column_count(stmt);

    for(int i = 0; i < columnCount; i++){
        resultSet.colNames.push_back(sqlite3_column_name(stmt, i));
    }

    for(int i = 0; i < params.size(); i++){
        auto& val = params.at(i);

        Type valType = DBValueConverter::checkType(val);
        switch(valType){
            case Type::DB_NULL_TYPE: {
                sqlite3_bind_null(stmt, i + 1);
                break;
            }
            case Type::DB_STRING_TYPE: {
                DB_String convertedType = DBValueConverter::fromDBValue<DB_String>(val);
                sqlite3_bind_text(stmt, i + 1, convertedType.c_str(), convertedType.size(), SQLITE_TRANSIENT);
                break;
            }
            case Type::DB_INT_TYPE: {
                DB_Int convertedType = DBValueConverter::fromDBValue<DB_Int>(val);
                sqlite3_bind_int64(stmt, i + 1, convertedType);
                break;
            }
            case Type::DB_FLOAT_TYPE: {
                DB_Float convertedType = DBValueConverter::fromDBValue<DB_Float>(val);
                sqlite3_bind_double(stmt, i + 1, convertedType);
                break;
            }
            case Type::DB_BINARY_TYPE: {
                DB_Binary convertedType = DBValueConverter::fromDBValue<DB_Binary>(val);
                sqlite3_bind_blob(stmt, i + 1, static_cast<const void*>(convertedType.data()), convertedType.size(), SQLITE_TRANSIENT);
                break;
            }
            default:{
                throw DatabaseException("DB::SQLiteDatabase::select", "TYPE ERROR", "Could not deduce Parameter types.");
            }
        }
    }


    while(sqlite3_step(stmt) == SQLITE_ROW){
        Row row;

        for(int i = 0; i < columnCount; i++){

            switch(sqlite3_column_type(stmt, i)){
                case SQLITE_TEXT: {
                    const char* val = (const char*)sqlite3_column_text(stmt, i);
                    int errCode = sqlite3_errcode(db);
                    if(val == nullptr){
                        row.insert({resultSet.colNames[i], DBValue()});


                    }
                    else{

                        row.insert({resultSet.colNames[i], DBValue(DB_String(val))});
                    }
                    
                    break;
                } 
                case SQLITE_INTEGER: {
                    DB_Int val = sqlite3_column_int64(stmt, i);


                    row.insert({resultSet.colNames[i], DBValue(DB_Int(val))});
                    break;  
                } 
                case SQLITE_FLOAT: {

                    DB_Float val = sqlite3_column_double(stmt, i);
                    row.insert({resultSet.colNames[i], DBValue(val)});


                    break;
                }
                case SQLITE_BLOB: {
                    
                    const char* blob = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, i));
                    int size = sqlite3_column_bytes(stmt, i);
                    row.insert({resultSet.colNames[i], DB_Binary(blob, blob + size)});

                    
                    break;
                }
                case SQLITE_NULL:{
                    row.insert({resultSet.colNames[i], DB_NULL({})});
                }
                
            }
        }

        resultSet.data.push_back(row);
        
    }
    


    return resultSet;
}

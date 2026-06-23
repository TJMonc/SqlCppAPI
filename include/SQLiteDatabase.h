#pragma once

#include "IDatabase.h"
#include "sqlite3.h"

namespace DB{
    
    class SQLiteDatabase : public IDatabase{
        private:
            sqlite3* db;
        public:
            ~SQLiteDatabase();
            SQLiteDatabase() = delete;
            SQLiteDatabase(const std::string& dbPath);
            SQLiteDatabase(const SQLiteDatabase& other);
            SQLiteDatabase(SQLiteDatabase&& other);

            virtual int execute(const std::string& query, const std::vector<std::string>& params);
            virtual QuerySet select(const std::string& query, const std::vector<std::string>& params);

    };

}

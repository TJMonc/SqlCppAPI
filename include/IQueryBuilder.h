#pragma once
#include "Schema.h"

namespace DB {



    class IQueryBuilder{
        std::string query;
        std::vector<DBValue>& aggregateParams;
        std::string prevState = "START";

        public:
            virtual ~IQueryBuilder() = default;
            virtual IQueryBuilder& select(std::vector<FieldSchema> cols, TableSchema fromTable, bool isDistinct = false) = 0;
            virtual IQueryBuilder& select(std::vector<std::string> colNames, std::string tableName, bool isDistinct = false) = 0;
            virtual IQueryBuilder& where(FieldSchema field, std::string op, DBValue val) = 0;
            virtual IQueryBuilder& where(std::string cond) = 0;
            virtual IQueryBuilder& limit(int num) = 0;
            virtual IQueryBuilder& order(FieldSchema col, bool isDesc) = 0;
            virtual IQueryBuilder& order(std::string colName, bool isDesc) = 0;
            virtual IQueryBuilder& innerJoin(TableSchema initTable, std::string onCond) = 0;
            virtual IQueryBuilder& rightJoin(TableSchema initTable, std::string onCond) = 0;
            virtual IQueryBuilder& leftJoin(TableSchema initTable, std::string onCond) = 0;
            virtual IQueryBuilder& fullJoin(TableSchema initTable, std::string onCond) = 0;


    };
}
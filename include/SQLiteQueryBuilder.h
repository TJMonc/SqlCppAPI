#pragma once
#include "IQueryBuilder.h"

namespace DB{
    class SQLiteQueryBuilder : public IQueryBuilder{
        public:
            SQLiteQueryBuilder() = default;
            virtual IQueryBuilder& select(TableSchema fromTable, bool isDistinct = false, std::vector<FieldSchema> cols = {});
            virtual IQueryBuilder& select(std::string tableName, bool isDistinct = false, std::vector<std::string> colNames = {});
            virtual IQueryBuilder& where(std::string cond);
            virtual IQueryBuilder& where(std::unique_ptr<Condition> cond);
            virtual IQueryBuilder& limit(int num);
            virtual IQueryBuilder& order(FieldSchema col, bool isDesc);
            virtual IQueryBuilder& order(std::string colName, bool isDesc);
            virtual IQueryBuilder& innerJoin(TableSchema initTable, std::unique_ptr<Condition> onCond);
            virtual IQueryBuilder& rightJoin(TableSchema initTable, std::unique_ptr<Condition> onCond);
            virtual IQueryBuilder& leftJoin(TableSchema initTable, std::unique_ptr<Condition> onCond);
            virtual IQueryBuilder& fullJoin(TableSchema initTable, std::unique_ptr<Condition> onCond);
            virtual IQueryBuilder& innerJoin(TableSchema initTable, std::string onCond);
            virtual IQueryBuilder& rightJoin(TableSchema initTable, std::string onCond);
            virtual IQueryBuilder& leftJoin(TableSchema initTable, std::string onCond);
            virtual IQueryBuilder& fullJoin(TableSchema initTable, std::string onCond);

            virtual std::string interpretCondition(std::unique_ptr<Condition>);
    };

}
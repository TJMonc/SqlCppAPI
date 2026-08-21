#pragma once
#include "Schema.h"
#include "AST_Tree.h"

namespace DB {



    class IQueryBuilder{
        protected:
            enum QueryStates {START, SELECT, WHERE, IN, LIMIT, ORDER, JOIN, JOIN_ON};
            std::string query;
            std::vector<DBValue> aggregateParams;
            QueryStates prevState = START;

        public:
            virtual ~IQueryBuilder() = default;
            IQueryBuilder() = default;
            virtual IQueryBuilder& select(TableSchema fromTable, bool isDistinct = false, std::vector<FieldSchema> cols = {}) = 0;
            virtual IQueryBuilder& select(std::string tableName, bool isDistinct = false, std::vector<std::string> colNames = {}) = 0;
            virtual IQueryBuilder& where(std::string cond) = 0;
            virtual IQueryBuilder& where(std::unique_ptr<Condition> cond) = 0;
            virtual IQueryBuilder& limit(int num) = 0;
            virtual IQueryBuilder& order(FieldSchema col, bool isDesc) = 0;
            virtual IQueryBuilder& order(std::string colName, bool isDesc) = 0;
            virtual IQueryBuilder& innerJoin(TableSchema initTable, std::unique_ptr<Condition> onCond) = 0;
            virtual IQueryBuilder& rightJoin(TableSchema initTable, std::unique_ptr<Condition> onCond) = 0;
            virtual IQueryBuilder& leftJoin(TableSchema initTable, std::unique_ptr<Condition> onCond) = 0;
            virtual IQueryBuilder& fullJoin(TableSchema initTable, std::unique_ptr<Condition> onCond) = 0;
            virtual IQueryBuilder& innerJoin(TableSchema initTable, std::string onCond) = 0;
            virtual IQueryBuilder& rightJoin(TableSchema initTable, std::string onCond) = 0;
            virtual IQueryBuilder& leftJoin(TableSchema initTable, std::string onCond) = 0;
            virtual IQueryBuilder& fullJoin(TableSchema initTable, std::string onCond) = 0;

            virtual std::string interpretCondition(std::unique_ptr<Condition>) = 0;


    };
}
#pragma once
#include "Schema.h"
#include "AST_Tree.h"

namespace DB {



    class IQueryBuilder{
        protected:
            std::string query;
            std::vector<DBValue> aggregateParams;
            
            

        public:
            virtual ~IQueryBuilder() = default;
            IQueryBuilder() = default;
            virtual IQueryBuilder& select(const TableSchema& fromTable, bool isDistinct = false, std::vector<FieldSchema> cols = {}) = 0;
            
            virtual IQueryBuilder& orUnion() = 0;
            virtual IQueryBuilder& andIntersection() = 0;
            virtual IQueryBuilder& except() = 0;

            virtual IQueryBuilder& where(std::string cond) = 0;
            virtual IQueryBuilder& where(std::unique_ptr<Condition> cond) = 0;
            virtual IQueryBuilder& limit(int limit, int offset = 0) = 0;

            virtual IQueryBuilder& order(const FieldSchema& col, bool isDesc) = 0;

            virtual IQueryBuilder& innerJoin(const TableSchema& initTable, std::unique_ptr<Condition> onCond) = 0;
            virtual IQueryBuilder& rightJoin(const TableSchema& initTable, std::unique_ptr<Condition> onCond) = 0;
            virtual IQueryBuilder& leftJoin(const TableSchema& initTable, std::unique_ptr<Condition> onCond) = 0;
            virtual IQueryBuilder& fullJoin(const TableSchema& initTable, std::unique_ptr<Condition> onCond) = 0;

            virtual IQueryBuilder& innerJoin(const TableSchema& initTable, std::string onCond) = 0;
            virtual IQueryBuilder& rightJoin(const TableSchema& initTable, std::string onCond) = 0;
            virtual IQueryBuilder& leftJoin(const TableSchema& initTable, std::string onCond) = 0;
            virtual IQueryBuilder& fullJoin(const TableSchema& initTable, std::string onCond) = 0;

            virtual IQueryBuilder& insert(const TableSchema& table, std::vector<DBValue> vals, std::vector<FieldSchema> fieldName = {}) = 0;
            virtual IQueryBuilder& insert(const TableSchema& table, std::vector<std::vector<DBValue>> vals, std::vector<FieldSchema> fieldNames = {}) = 0;

            virtual IQueryBuilder& update(const TableSchema& table, const FieldSchema& setField, DBValue setValue) = 0;

            virtual std::string interpretCondition(std::unique_ptr<Condition> cond) = 0;


    };
}
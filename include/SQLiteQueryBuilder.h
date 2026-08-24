#pragma once
#include "IQueryBuilder.h"

namespace DB{
    class SQLiteQueryBuilder : public IQueryBuilder{
        public:
            SQLiteQueryBuilder() = default;
            
            virtual IQueryBuilder& select(const TableSchema& fromTable, bool isDistinct = false, std::vector<FieldSchema> cols = {});
            
            virtual IQueryBuilder& orUnion();
            virtual IQueryBuilder& andIntersection();
            virtual IQueryBuilder& except();

            virtual IQueryBuilder& where(std::string cond);
            virtual IQueryBuilder& where(std::unique_ptr<Condition> cond);
            virtual IQueryBuilder& limit(int limit, int offset = 0);
            virtual IQueryBuilder& order(const FieldSchema& col, bool isDesc);

            virtual IQueryBuilder& innerJoin(const TableSchema& initTable, std::unique_ptr<Condition> onCond);
            virtual IQueryBuilder& rightJoin(const TableSchema& initTable, std::unique_ptr<Condition> onCond);
            virtual IQueryBuilder& leftJoin(const TableSchema& initTable, std::unique_ptr<Condition> onCond);
            virtual IQueryBuilder& fullJoin(const TableSchema& initTable, std::unique_ptr<Condition> onCond);

            virtual IQueryBuilder& innerJoin(const TableSchema& initTable, std::string onCond);
            virtual IQueryBuilder& rightJoin(const TableSchema& initTable, std::string onCond);
            virtual IQueryBuilder& leftJoin(const TableSchema& initTable, std::string onCond);
            virtual IQueryBuilder& fullJoin(const TableSchema& initTable, std::string onCond);

            virtual IQueryBuilder& insert(const TableSchema& table, std::vector<DBValue> vals, std::vector<FieldSchema> fieldName = {});
            virtual IQueryBuilder& insert(const TableSchema& table, std::vector<std::vector<DBValue>> vals, std::vector<FieldSchema> fieldNames = {});

            virtual IQueryBuilder& update(const TableSchema& table, const FieldSchema& setField, DBValue setValue);

            virtual std::string interpretCondition(std::unique_ptr<Condition> cond);

            
    };

}
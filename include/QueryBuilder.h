#pragma once
#include "IDatabase.h"
#include "Schema.h"
#include <memory>

namespace DB{
    class Condition{
        std::string op;
        std::vector<Value> params;
        public:
            virtual std::string interpret() = 0;

    };
    class CompareCondition : public Condition{
        private:
            Value m_left;
            std::string m_right;
        public:
            std::string interpret();
            CompareCondition(const Value& a_left, const std::string& a_right, const std::string& a_op);
    };
    class LogicCondition : public Condition{
        private:
            std::unique_ptr<Condition> m_left, m_right;
        public:
            LogicCondition(std::unique_ptr<Condition> a_left, std::unique_ptr<Condition> a_right, const std::string a_op);
            std::string interpret();
    };



    class QueryBuilder{
        IDatabase& m_db;
        const TableDef& m_schema;
        std::string m_sql;
        std::vector<Value> m_params;
        bool m_hasWhere;
        int m_wherePos;
        bool m_hasOrder;

        public:
            QueryBuilder& where(const Condition& a_cond);
            QueryBuilder& orderBy(const ColumnDef& a_cond, bool ascending=false);

            QueryBuilder& limit(int64_t);
            
            ResultSet all();
            size_t count();
    };
}
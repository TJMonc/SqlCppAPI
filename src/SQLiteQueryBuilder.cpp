#include "SQLiteQueryBuilder.h"
#include <format>
using namespace DB;

IQueryBuilder &DB::SQLiteQueryBuilder::select(const TableSchema& fromTable, bool isDistinct, std::vector<FieldSchema> cols)
{
    if(fromTable.fields.empty()){
        throw DatabaseException("DB::SQLiteQueryBuilder::select()", "TABLE ERROR", "TableSchema has no columns");
    }
    query += "SELECT ";
    if(isDistinct){
        query += "DISTINCT ";
    }
    if(cols.empty()){
        
        for(const auto& fields : fromTable.fields){
            query += std::format("{}.{} AS {}_{}", fields.tableName, fields.fieldName, fields.fieldName, fields.tableName);
            if(&fields != &fromTable.fields.back()){
                query += ", ";
            }
        }
        query = query.substr(0, query.size() - 2) + " ";
    }
    else{
        for(const auto& fields : cols){
            query += std::format("{}.{} AS {}_{}", fields.tableName, fields.fieldName, fields.fieldName, fields.tableName);
            if(&fields != &cols.back()){
                query += ", ";
            }
        }

    }

    query += "FROM " + fromTable.tableName + " ";

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::orUnion() {
    query += "UNION ";
    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::andIntersection()
{
    query += "INTERSECT ";
    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::except()
{
    query += "EXCEPT ";
    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::where(std::string cond)
{
    query += "WHERE " + cond + " ";
    return *this;

}

IQueryBuilder &DB::SQLiteQueryBuilder::where(std::unique_ptr<Condition> cond) {
    query += interpretCondition(std::move(cond)) + " ";

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::limit(int limit, int offset)
{
    query += std::format("LIMIT {} OFFSET {} ", limit, offset);
    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::order(const FieldSchema &col, bool isDesc)
{
    const char* descStr = (isDesc) ? "DESC" : "ASC";

    query += std::format("ORDER BY {} {} ", col.fieldName, descStr);
    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::innerJoin(const TableSchema &initTable, std::unique_ptr<Condition> onCond) {
    query += std::format("INNER JOIN {} ON {} ", initTable.tableName, interpretCondition(std::move(onCond)));

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::rightJoin(const TableSchema &initTable, std::unique_ptr<Condition> onCond)
{
    query += std::format("RIGHT JOIN {} ON {} ", initTable.tableName, interpretCondition(std::move(onCond)));

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::leftJoin(const TableSchema &initTable, std::unique_ptr<Condition> onCond)
{
    query += std::format("LEFT JOIN {} ON {} ", initTable.tableName, interpretCondition(std::move(onCond)));

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::fullJoin(const TableSchema &initTable, std::unique_ptr<Condition> onCond)
{
    query += std::format("FULL JOIN {} ON {} ", initTable.tableName, interpretCondition(std::move(onCond)));

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::innerJoin(const TableSchema &initTable, std::string onCond)
{
    query += std::format("INNER JOIN {} ON {} ", initTable.tableName, onCond);

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::rightJoin(const TableSchema &initTable, std::string onCond)
{
    query += std::format("RIGHT JOIN {} ON {} ", initTable.tableName, onCond);

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::leftJoin(const TableSchema &initTable, std::string onCond)
{
    query += std::format("LEFT JOIN {} ON {} ", initTable.tableName, onCond);

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::fullJoin(const TableSchema &initTable, std::string onCond)
{
    query += std::format("FULL JOIN {} ON {} ", initTable.tableName, onCond);

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::insert(const TableSchema &table, std::vector<DBValue> vals, std::vector<FieldSchema> fields)
{
    if(table.fields.empty()){
        throw DatabaseException(
            "DB::SQLiteQueryBuilder::insert", 
            "INVALID ARGUMENTS",
             "TableSchema has no columns");
    }
    if(vals.empty()){
        throw DatabaseException(
            "DB::SQLiteQueryBuilder::insert", 
            "INVALID ARGUMENTS",
             "Value vector is empty"); 
    }

    query += "INSERT INTO " + table.tableName + " ";
    if(fields.size() != 0){
        query += "(";
        for(size_t i = 0; i < fields.size(); i++){
            query += fields.at(i).fieldName;
            if(i < fields.size() - 1){
                query += ", ";
            }
        }
        query += ") ";
    }
    query += "VALUES ( ";
    for(size_t i = 0; i < vals.size(); i++){
        query += "? ";

        if(i < vals.size() - 1){
            query += ", ";
        }
    }
    query += ") ";

    aggregateParams.insert(aggregateParams.end(), vals.begin(), vals.end());

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::insert(const TableSchema& table, std::vector<std::vector<DBValue>> vals, std::vector<FieldSchema> fields){
    if(table.fields.empty()){
        throw DatabaseException(
            "DB::SQLiteQueryBuilder::insert", 
            "INVALID ARGUMENTS",
             "TableSchema has no columns");
    }
    if(vals.empty()){
        throw DatabaseException(
            "DB::SQLiteQueryBuilder::insert", 
            "INVALID ARGUMENTS",
             "Value vector is empty"); 
    }

    query += "INSERT INTO " + table.tableName + " ";
    if(fields.size() != 0){
        query += "(";
        for(size_t i = 0; i < fields.size(); i++){
            query += fields.at(i).fieldName;
            if(i < fields.size() - 1){
                query += ", ";
            }
        }
        query += ") ";
    }
    query += "VALUES ";

    for(size_t i = 0; i < vals.size(); i++){
        query += "(";

        for (size_t j = 0; j < vals.at(i).size(); j++){
            query += "? ";

            if (i < vals.at(i).size() - 1){
                query += ", ";
            }
        }
        query += ")";
        if (i < vals.size() - 1){
            query += ", ";
        }
        aggregateParams.insert(aggregateParams.end(), vals.at(i).begin(), vals.at(i).end());

    }
    query += " ";
    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::insertSelect(const TableSchema &insertTable, std::vector<FieldSchema> fieldNames, const TableSchema &selectTable, bool isDistinct, std::vector<FieldSchema> cols){
    if(insertTable.fields.empty() || selectTable.fields.empty()){
        throw DatabaseException("DB::SQLiteQueryBuilder::insertSelect()", "INVALID ARGUMENTS", "TableSchema has no Columns");
    }
    query += "INSERT INTO " + insertTable.tableName + " ";
    const std::vector<FieldSchema>* insertFields;
    const std::vector<FieldSchema>* selectFields;
    if(!fieldNames.empty()){
        insertFields = &insertTable.fields;
    }

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::update(const TableSchema &table, const FieldSchema &setField, DBValue setValue)
{
    query += std::format("UPDATE {} SET {} = ? ", table.tableName, setField.fieldName);
    aggregateParams.emplace_back(setValue);
    return *this;
}

IQueryBuilder& DB::SQLiteQueryBuilder::makeTable(const TableSchema& tableSchema){return *this;};
std::string DB::SQLiteQueryBuilder::interpretCondition(std::unique_ptr<Condition> cond){return "";};

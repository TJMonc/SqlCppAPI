#include "SQLiteQueryBuilder.h"
#include <format>
using namespace DB;

const std::string DB::SQLiteQueryBuilder::getTypeString(const FieldSchema& field)
{


    switch(field.fieldType){
        case Type::DB_NULL_TYPE:{
            return "NULL";
            break;
        }
        case Type::DB_INT_TYPE:{
            return "INTEGER";
            break;
        }
        case Type::DB_FLOAT_TYPE:{
            return "REAL";
            break;
        }
        case Type::DB_STRING_TYPE:{

            if(field.fieldType == DB_STRING_TYPE && field.charLimit != 0){
                return std::format("VARCHAR({})", field.charLimit);
            }
            else{
                return "TEXT";
            }

            break;
        }
        case Type::DB_BINARY_TYPE:{
            return "BLOB";
            break;
        }
        default:{
            throw DatabaseException("DB::SQLiteQueryBuilder::getTypeString()", "TYPE ERROR", "Field type not recognized from Schema");
        }
    }
}

IQueryBuilder &DB::SQLiteQueryBuilder::select(const TableSchema &fromTable, bool isDistinct, std::vector<FieldSchema> cols)
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
    query += "WHERE " + interpretCondition(std::move(cond)) + " ";

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
    
    insertFields = (fieldNames.empty()) ? &insertTable.fields : &fieldNames;
    selectFields = (cols.empty()) ? &selectTable.fields : &cols;

    if(insertFields->size() != selectFields->size()){
        throw DatabaseException("DB::SQLiteQueryBuilder::insertSelect()", "INVALID ARGUMENTS", "INSERT fields and SELECT field sizes must be equal");
    }
    query += "(";

    for(size_t i = 0; i < insertFields->size(); i++){
        query += insertFields->at(i).fieldName;
        if(i < insertFields->size() - 1){
            query += ", ";
        }

    }
    query += ") SELECT ";

    for(size_t i = 0; i < selectFields->size(); i++){
        query += selectFields->at(i).fieldName;
        if(i < selectFields->size() - 1){
            query += ", ";
        }

    }

    return *this;
}

IQueryBuilder &DB::SQLiteQueryBuilder::update(const TableSchema &table, const FieldSchema &setField, DBValue setValue)
{
    query += std::format("UPDATE {} SET {} = ? ", table.tableName, setField.fieldName);
    aggregateParams.emplace_back(setValue);
    return *this;
}

IQueryBuilder& DB::SQLiteQueryBuilder::makeTable(const TableSchema& tableSchema){
    if(tableSchema.fields.empty()){
        throw DatabaseException("DB::SQLiteQueryBuilder::makeTable()", "SCHEMA ERROR", "Table Schema must have at least one field");
    }
    query += "CREATE TABLE IF NOT EXISTS " + tableSchema.tableName + " (";

    for(const auto& field : tableSchema.fields){
        query += std::format("{} {}", field.fieldName, this->getTypeString(field));

        
        if(field.isPrimary){
            query += " PRIMARY KEY";
        }
        if(field.isAutoIncrement){
            query += " AUTOINCREMENT";
        }
        if(!field.isNullable){
            query += " NOT NULL";
        }
        if(!std::holds_alternative<DB_NULL>(field.defaultValue)){
            if(field.fieldType == DB_STRING_TYPE){
                query += " DEFAULT \'" + DBValueConverter::fromDBValue<DB_String>(field.defaultValue) + "\'";
            }
            else{
                query += " DEFAULT " + DBValueConverter::fromDBValue<DB_String>(field.defaultValue);
    
            }
        }
        if(field.isForeign){
            query += " FOREIGN KEY";
        }
        if(field.isUnique){
            query += " UNIQUE";
        }
        if(&field != &tableSchema.fields.back()){
            query += ", ";
        }
    }
    query += ") ";

    
    return *this;

}
IQueryBuilder &DB::SQLiteQueryBuilder::alterTable(const TableSchema &oldTableSchema, const TableSchema &newTableSchema) {
    return *this;
}
IQueryBuilder &DB::SQLiteQueryBuilder::endStatement() {
    query += ";\n";
    return *this;
};
std::string DB::SQLiteQueryBuilder::interpretCondition(const std::unique_ptr<Condition>& cond){
    const auto type = cond->conditionType;
    std::string resultStr;
    aggregateParams.insert(aggregateParams.end(), cond->values.begin(), cond->values.end());

    switch(cond->conditionType){
        case ConditionType::BINARY_NODE:{
            BinaryNode* ptr = dynamic_cast<BinaryNode*>(cond.get());
            resultStr = interpretCondition(ptr->left) + " " + ptr->op + " " + interpretCondition(ptr->right);

            break;
        }
        case ConditionType::BETWEEN_NODE:{
            BetweenNode* ptr = dynamic_cast<BetweenNode*>(cond.get());

            resultStr = std::format("{} BETWEEN ? AND ?", ptr->fieldName);
            break;
        }
        case ConditionType::IN_NODE:{
            InNode* ptr = dynamic_cast<InNode*>(cond.get());

            if(ptr->isCondition){
                resultStr = std::format("{} IN (SELECT {} FROM {} WHERE {})",
                     ptr->fieldName, ptr->conditionTarget.fieldName, ptr->conditionTarget.tableName, interpretCondition(ptr->inCondition));
            }
            else{
                size_t valSize = ptr->valEndOffset - ptr->valBeginOffset;
                resultStr = ptr->fieldName + " IN (";
                for(size_t i = 0; i < valSize; ++i){
                    resultStr += "?";
                    if(i < valSize - 1){
                        resultStr += ", ";
                    }
                }
                resultStr += ")";
            }

            break;
        }
        case ConditionType::UNARY_NODE:{
            UnaryNode* ptr = dynamic_cast<UnaryNode*>(cond.get());
            resultStr = std::format("NOT {}", interpretCondition(ptr->cond));
            break;
        }
        case LITERAL_NODE:{
            resultStr = "?";
            break;
        }
        case FIELD_NODE:{

            resultStr = dynamic_cast<FieldNode*>(cond.get())->fieldName;
        }
    }
    return resultStr;
};

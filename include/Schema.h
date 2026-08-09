#pragma once
#include "IDatabase.h"

namespace DB{
    struct FieldSchema{
        const Type fieldType;
        const std::string fieldName;
        const std::string tableName;
        const DBValue defaultValue = 0;
        const bool isPrimary = false;
        const bool isUnique = false;
        const bool isForeign = false;
        const bool isAutoIncrement = false;
        const bool isNullable = true;

    };

    struct TableSchema{
        std::string tableName;
        FieldSchema primary;
        std::vector<FieldSchema> fields;
    };

}


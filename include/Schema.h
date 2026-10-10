#pragma once
#include "IDatabase.h"

namespace DB{
    struct FieldSchema{
        Type fieldType;
        std::string fieldName;
        std::string tableName;
        DBValue defaultValue = 0;
        bool isPrimary = false;
        bool isUnique = false;
        bool isForeign = false;
        bool isAutoIncrement = false;
        bool isNullable = true;
        int charLimit = 0;



    };

    struct TableSchema{
        std::string tableName;
        FieldSchema primary;
        std::vector<FieldSchema> fields;

        inline void registerField(const FieldSchema& f){ fields.emplace_back(f); };

    };

}


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
        std::vector<FieldSchema> fields;
        int primarySize;

        TableSchema(std::string a_tableName, const std::vector<FieldSchema>& a_fields) : tableName(a_tableName), fields(a_fields){
            for(size_t i = 0; i < a_fields.size(); i++){
                if(a_fields.at(i).isPrimary){
                    primarySize++;
                }
            }

        }

        inline void registerField(const FieldSchema& f){
            if(f.isPrimary == true){
                primarySize++;
            }
            fields.emplace_back(f);
        };

    };

}


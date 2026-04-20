#pragma once
#include "Schema.h"


namespace DB{


    class Record{
        IDatabase& db;
        TableDef& m_schema;
        std::unordered_map<std::string, Value> m_fields;
        
        public:
            template <typename T>
            T get(const ColumnDef& col){
               if(!m_fields.contains(col.m_columnName)){
                std::runtime_error("Column does not exist in table");
               }

                return ValueConverter<T>::fromIValue(m_fields.at(col.m_columnName));
            }

            template <typename T>
            T get(const std::string& col){
               if(!m_fields.contains(col)){
                std::runtime_error("Column does not exist in table");
               }
                return ValueConverter<T>::fromIValue(m_fields.at(col));
            }

            template <typename T>
            void set(T a_val);

            Record(IDatabase& a_db, TableDef& a_schema);

            const Value &operator[](const std::string &colName) const{
                auto it = m_fields.find(colName);
                if (it == m_fields.end())
                    throw std::out_of_range("Column '" + colName + "' not found in record");
                return it->second;
            }

            bool save();
            bool remove();
    };
}
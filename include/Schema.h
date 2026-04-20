#include "IDatabase.h"
#include <optional>
namespace DB{
    enum class StorageType {
        Integer,
        Real,
        Text,
        Blob,
        Null
    };

    struct ColumnConstraints{
        bool unique = false;
        bool nullable = true;
        bool primaryKey = false;
    };

    struct ColumnDef{
        std::string m_tableName;
        std::string m_columnName;
        StorageType m_type;
        ColumnConstraints m_constraints;
        std::optional<Hint> m_hint;

    };

    class TableDef{
        public:
            const std::string m_tableName;
            const std::vector<ColumnDef> m_cols;
            const ColumnDef m_primaryKey;
        protected:
            TableDef(const std::string& a_name, const std::vector<ColumnDef>& a_cols) : m_tableName(a_name), m_cols(a_cols),
                m_primaryKey({
                    .m_tableName = a_name,
                    .m_columnName = a_name + "id",
                    .m_type = StorageType::Integer,
                    .m_constraints = {.unique = true, .nullable = false, .primaryKey = true},
                    .m_hint = std::nullopt
                }){}
    };
    

}
Structure:
    - Database Class +
        -  Value Class -
            - Name
            - Table Name
            - db path
            - Database* db
            - enum Type{TEXT, INT, DECIMAL, BOOL}
            - Type type
            - Type getType()
        - TEXT : Value +
            - std::string val
        - INT : Value : public
            - int val
        - DECIMAL : Value +
            - double val
        - BOOL : Value : public
            - bool val
        - RecordContainer class +
            - std::string tableName;
            - Database* db;
            - std::vector<std::vector<std::unique_ptr<Record>> records
            - RecordContainer add(std::string condition)
            - RecordContainer filter(std::string condition)
            - bool update(std::string field, std::string value)
        - Model Class +
            - Name
            - Database* db
            - db
            - int insert(std::vector<std::string> values, std::vector<std::string> columns = {""});
            - RecordContainer get(std::vector<std::string> fields, std::vector<std::pair<std::string, std::string>> conditionals = {{"", ""}});
            - Model(std::string name)
        - Record : Model -
            int insert(std::vector<std::string> values, std::vector<std::string> columns = {""}) = delete
            RecordContainer get(std::vector<std::string> fields, std::vector<std::pair<std::string, std::string>> conditionals = {{"", ""}}) = delete
            bool save()
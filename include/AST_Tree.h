   #pragma once
   #include "Schema.h"
    #include <memory>
    #include <array>


namespace DB{

    enum ConditionType {FIELD_NODE, LITERAL_NODE, BINARY_NODE, UNARY_NODE, IN_NODE, BETWEEN_NODE};
    struct Condition{
        ConditionType conditionType;
        std::vector<DBValue> values;

        virtual ~Condition() = default;
    };

    struct FieldNode : Condition{
        std::string fieldName;
        FieldNode(std::string a_fieldName);


    };

    struct LiteralNode : Condition{
        DBValue val;

        LiteralNode(DBValue a_val);

    };

    struct BinaryNode : Condition{
        std::unique_ptr<Condition> right;
        std::unique_ptr<Condition> left;
        std::string op;

        BinaryNode(std::unique_ptr<Condition> a_left, std::unique_ptr<Condition> a_right, std::string a_op);

    };

    struct UnaryNode : Condition{
        std::unique_ptr<Condition> cond;
        std::string op;

        UnaryNode(std::unique_ptr<Condition> cond, std::string op);
        
        
    };

    struct InNode : Condition{
        std::string fieldName;
        int valBeginOffset;
        int valEndOffset;
        std::unique_ptr<Condition> inCondition;
        bool isCondition;

        InNode(std::string fieldName, const std::vector<DBValue>& a_condVals);
        InNode(std::string fieldName, std::unique_ptr<Condition> inCondition);

    };

    struct BetweenNode : Condition{
        std::string fieldName;
        std::array<DBValue, 2> range;

        BetweenNode(std::string fieldName, const std::array<DBValue, 2>& range);
        BetweenNode(std::string fieldName, const DBValue& val1, const DBValue& val2);

    };

    struct SpecialOperators{
        static std::unique_ptr<Condition> like(const FieldSchema& field, DBValue literal);
        static std::unique_ptr<Condition> ilike(const FieldSchema& field, DBValue literal);
        static std::unique_ptr<Condition> in(const FieldSchema& field, std::vector<DBValue> literal);
        static std::unique_ptr<Condition> in(const FieldSchema& field, std::unique_ptr<Condition> cond);

        static std::unique_ptr<Condition> between(const FieldSchema& field, std::array<DBValue, 2> literal);


    };

    //Comparison
    std::unique_ptr<Condition> operator<(FieldSchema&, DBValue);
    std::unique_ptr<Condition> operator>(FieldSchema&, DBValue);

    std::unique_ptr<Condition> operator<(DBValue, FieldSchema&);
    std::unique_ptr<Condition> operator>(DBValue, FieldSchema&);


    std::unique_ptr<Condition> operator<=(FieldSchema&, DBValue);
    std::unique_ptr<Condition> operator>=(FieldSchema&, DBValue);

    std::unique_ptr<Condition> operator<=(DBValue, FieldSchema&);
    std::unique_ptr<Condition> operator>=(DBValue, FieldSchema&);


    std::unique_ptr<Condition> operator==(FieldSchema&, DBValue);
    std::unique_ptr<Condition> operator!=(FieldSchema&, DBValue);


    //Logical
    std::unique_ptr<Condition> operator&&(std::unique_ptr<Condition>, std::unique_ptr<Condition>);
    std::unique_ptr<Condition> operator||(std::unique_ptr<Condition>, std::unique_ptr<Condition>);
}

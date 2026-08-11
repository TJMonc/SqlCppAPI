    #include "Schema.h"
    #include <memory>


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

        DB::BinaryNode::BinaryNode(std::unique_ptr<Condition> a_left, std::unique_ptr<Condition> a_right, std::string a_op);
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

        InNode(std::string fieldName, std::vector<DBValue> a_condVals);
        InNode(std::string fieldName, std::unique_ptr<Condition> inCondition);

    };

    struct BetweenNode : Condition{
        std::string fieldName;
        DBValue range[2];

        BetweenNode(std::string fieldName, DBValue range[2]);
        BetweenNode(std::string fieldName, DBValue val1, DBValue val2);

    };

    struct SpecialOperators{
        static BinaryNode like(FieldSchema field, DBValue literal);
        static BinaryNode ilike(FieldSchema field, DBValue literal);
        static BinaryNode in(FieldSchema field, std::vector<DBValue> literal);
        static BinaryNode between(FieldSchema field, std::array<DBValue, 2> literal);
        static UnaryNode isNull(FieldSchema field);
        static UnaryNode isNotNull(FieldNode field);

    };
}

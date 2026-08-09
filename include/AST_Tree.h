    #include "Schema.h"
    #include <memory>


namespace DB{

    enum ConditionType {FIELD_NODE, LITERAL_NODE, BINARY_NODE, UNARY_NODE, IN_NODE, BETWEEN_NODE};
    struct Condition{
        ConditionType conditionType;
        std::vector<DBValue> values;

        virtual ~Condition() = default;
        virtual std::unique_ptr<Condition> resolve() = 0;
    };

    struct FieldNode : Condition{
        std::string fieldName;
        FieldNode(std::string a_fieldName);
        std::unique_ptr<Condition> resolve();

    };

    struct LiteralNode : Condition{
        DBValue val;

        LiteralNode(DBValue a_val);
        std::unique_ptr<Condition> resolve();

    };

    struct BinaryNode : Condition{
        std::unique_ptr<Condition> right;
        std::unique_ptr<Condition> left;
        std::string op;

        BinaryNode(Condition& a_left, Condition& a_right, std::string op);
        std::unique_ptr<Condition> resolve();
    };

    struct UnaryNode : Condition{
        std::unique_ptr<Condition> cond;
        std::string op;

        UnaryNode(Condition& cond, std::string op);
        std::unique_ptr<Condition> resolve();
        
    };

    struct InNode : Condition{
        std::string fieldName;
        int valBeginOffset;
        int valEndOffset;
        std::unique_ptr<Condition> inCondition;
        bool isCondition;

        InNode(std::string fieldName, std::vector<DBValue> a_condVals);
        InNode(std::string fieldName, Condition& inCondition);

        std::unique_ptr<Condition> resolve();
    };

    struct BetweenNode : Condition{

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

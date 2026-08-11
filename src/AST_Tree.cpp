#include "AST_Tree.h"
using namespace DB;

DB::FieldNode::FieldNode(std::string a_fieldName) : fieldName(a_fieldName) {
    conditionType = FIELD_NODE;

}


DB::LiteralNode::LiteralNode(DBValue a_val) : val(a_val) {
    values.emplace_back(a_val);
    conditionType = LITERAL_NODE;
}


DB::BinaryNode::BinaryNode(std::unique_ptr<Condition> a_left, std::unique_ptr<Condition> a_right, std::string a_op){
    left = std::move(a_left);
    right = std::move(a_right);
    op = std::move(a_op);

    values.insert(values.end(), left->values.begin(), left->values.end());
    left->values.clear();

    values.insert(values.end(), right->values.begin(), right->values.end());
    right->values.clear();

    conditionType = BINARY_NODE;

}


DB::InNode::InNode(std::string a_fieldName, std::vector<DBValue> a_condVals) : fieldName(a_fieldName), valBeginOffset(values.size()) {
    values.insert(values.end(), a_condVals.begin(), a_condVals.end());

    valEndOffset = values.size();
    isCondition = false;
    conditionType = IN_NODE;
}

DB::InNode::InNode(std::string a_fieldName, std::unique_ptr<Condition> a_inCondition) {
    fieldName = std::move(a_fieldName);
    inCondition = std::move(a_inCondition);

    values.insert(values.end(), inCondition->values.begin(), inCondition->values.end());
    inCondition->values.clear();

    isCondition = true;
    conditionType = IN_NODE;

}


DB::UnaryNode::UnaryNode(std::unique_ptr<Condition> a_cond, std::string a_op)
{
    this->cond = std::move(a_cond);
    op = std::move(a_op);

    this->values.insert(values.end(), cond->values.begin(), cond->values.end());
    cond->values.clear();

    conditionType = UNARY_NODE;
}




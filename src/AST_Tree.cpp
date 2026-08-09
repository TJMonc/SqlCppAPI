#include "AST_Tree.h"
using namespace DB;

DB::FieldNode::FieldNode(std::string a_fieldName) : fieldName(a_fieldName) {
    conditionType = FIELD_NODE;

}

std::unique_ptr<Condition> DB::FieldNode::resolve() {
    return std::make_unique<FieldNode>(*this);
    conditionType = FIELD_NODE;
}

DB::LiteralNode::LiteralNode(DBValue a_val) : val(a_val) {
    values.emplace_back(a_val);
    conditionType = LITERAL_NODE;
}

std::unique_ptr<Condition> DB::LiteralNode::resolve()
{
    return std::make_unique<LiteralNode>(*this);
}

DB::BinaryNode::BinaryNode(Condition& a_left, Condition& a_right, std::string a_op){
    left = a_left.resolve();
    right = a_right.resolve();
    op = std::move(a_op);

    values.insert(values.end(), left->values.begin(), left->values.end());
    left->values.clear();

    values.insert(values.end(), right->values.begin(), right->values.end());
    right->values.clear();

    conditionType = BINARY_NODE;

}

std::unique_ptr<Condition> DB::BinaryNode::resolve() {
    return std::make_unique<BinaryNode>(*this->left, *this->right, this->op);
}

DB::InNode::InNode(std::string a_fieldName, std::vector<DBValue> a_condVals) : fieldName(a_fieldName), valBeginOffset(values.size()) {
    values.insert(values.end(), a_condVals.begin(), a_condVals.end());
    valEndOffset = values.size();
    isCondition = false;
    conditionType = IN_NODE;
}

DB::InNode::InNode(std::string a_fieldName, Condition &a_inCondition) {
    fieldName = std::move(a_fieldName);
    inCondition = a_inCondition.resolve();
    values.insert(values.end(), inCondition->values.begin(), inCondition->values.end());

    isCondition = true;
    conditionType = IN_NODE;

}

std::unique_ptr<Condition> DB::InNode::resolve()
{
    if(!isCondition){
        return std::make_unique<InNode>(fieldName, std::vector<DBValue>(values.begin() + valBeginOffset, values.begin() + valEndOffset));
    }
    else{
        return std::make_unique<InNode>(fieldName, *this->inCondition);
    }
}

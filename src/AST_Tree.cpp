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


DB::InNode::InNode(std::string a_fieldName, const std::vector<DBValue>& a_condVals) : fieldName(a_fieldName), valBeginOffset(values.size()) {
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

DB::BetweenNode::BetweenNode(std::string a_fieldName, const std::array<DBValue, 2>& a_range) {
    fieldName = std::move(a_fieldName);
    range = a_range;
}

DB::BetweenNode::BetweenNode(std::string a_fieldName, const DBValue &a_val1, const DBValue &a_val2)
{
    fieldName = std::move(a_fieldName);
    range = {a_val1, a_val2};
}

std::unique_ptr<Condition> DB::SpecialOperators::like(const FieldSchema& a_field, DBValue a_literal) {
    return std::make_unique<BinaryNode>(std::make_unique<FieldNode>(a_field.fieldName), std::make_unique<LiteralNode>(a_literal), "LIKE");
}

std::unique_ptr<Condition> DB::SpecialOperators::ilike(const FieldSchema &a_field, DBValue a_literal)
{
    return std::make_unique<BinaryNode>(std::make_unique<FieldNode>(a_field.fieldName), std::make_unique<LiteralNode>(a_literal), "ILIKE");
}

std::unique_ptr<Condition> DB::SpecialOperators::in(const FieldSchema &field, std::vector<DBValue> literal) {
    return std::make_unique<InNode>(field.fieldName, literal);
}

std::unique_ptr<Condition> DB::SpecialOperators::in(const FieldSchema &field, std::unique_ptr<Condition> cond)
{
    return std::make_unique<InNode>(field.fieldName, std::move(cond));
}

std::unique_ptr<Condition> DB::SpecialOperators::between(const FieldSchema& field, std::array<DBValue, 2> literal)
{
    return std::make_unique<BetweenNode>(field.fieldName, literal);
}

std::unique_ptr<Condition> DB::operator<(FieldSchema& field, DBValue val)
{
    return std::make_unique<BinaryNode>(std::make_unique<FieldNode>(field.fieldName), std::make_unique<LiteralNode>(val), "<");
}

std::unique_ptr<Condition> DB::operator>(FieldSchema& field, DBValue val){
    return std::make_unique<BinaryNode>(std::make_unique<FieldNode>(field.fieldName), std::make_unique<LiteralNode>(val), ">");
}

std::unique_ptr<Condition> DB::operator<(DBValue val, FieldSchema& field)
{
    return operator>(field, val);
}

std::unique_ptr<Condition> DB::operator>(DBValue val, FieldSchema& field)
{
    return operator<(field, val);
}

std::unique_ptr<Condition> DB::operator<=(FieldSchema& field, DBValue val) {
    return std::make_unique<BinaryNode>(std::make_unique<FieldNode>(field.fieldName), std::make_unique<LiteralNode>(val), "<=");
}

std::unique_ptr<Condition> DB::operator>=(FieldSchema& field, DBValue val)
{
    return std::make_unique<BinaryNode>(std::make_unique<FieldNode>(field.fieldName), std::make_unique<LiteralNode>(val), ">=");
}

std::unique_ptr<Condition> DB::operator<=(DBValue val, FieldSchema& field)
{
    return operator>=(field, val);
}

std::unique_ptr<Condition> DB::operator>=(DBValue val, FieldSchema& field)
{
    return operator<=(field, val);
}

std::unique_ptr<Condition> DB::operator==(FieldSchema& field, DBValue val)
{
    return std::make_unique<BinaryNode>(std::make_unique<FieldNode>(field.fieldName), std::make_unique<LiteralNode>(val), "=");
}

std::unique_ptr<Condition> DB::operator!=(FieldSchema& field, DBValue val)
{
    return std::make_unique<BinaryNode>(std::make_unique<FieldNode>(field.fieldName), std::make_unique<LiteralNode>(val), "!=");
}

std::unique_ptr<Condition> DB::operator&&(std::unique_ptr<Condition> left, std::unique_ptr<Condition> right)
{
    return std::make_unique<BinaryNode>(std::move(left), std::move(right), "AND");
}

std::unique_ptr<Condition> DB::operator||(std::unique_ptr<Condition> left, std::unique_ptr<Condition> right)
{
    return std::make_unique<BinaryNode>(std::move(left), std::move(right), "OR");
}

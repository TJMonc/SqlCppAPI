#include "sqlite3c++.h"

Database::Value::Value(std::string aFieldName, Type aType, bool aIsUnique):
fieldName(aFieldName), type(aType), isUnique(aIsUnique){}


//Note that NONE isn't a NULL value. It just means that it doesn't represent a sqlite value(things like table names, column names, etc)
Database::TextValue::TextValue(Value& other): Value(other.fieldName, Type::TEXT) {
    switch(other.type){
        case TEXT:{

            TextValue* text = static_cast<TextValue*>(&other);
            this->value = text->value;
            break;
        }
        case INT:{

            IntValue* integer = static_cast<IntValue*>(&other);
            this->value = std::to_string(integer->value);
            break;
        }
        case DECIMAL:{

            DecimalValue* decimal = static_cast<DecimalValue*>(&other);
            this->value = std::to_string(decimal->value);
            break;
        }
        case BOOL:{

            BoolValue* boolean = static_cast<BoolValue*>(&other);
            this->value = std::to_string(boolean->value);
            
            break;
        }
        case BLOB:{

            throw std::runtime_error("ERROR: Attempted to convert BlobValue to TextValue");
            break;
        }
        case NONE:{

            NoneValue* none = static_cast<NoneValue*>(&other);
            this->value = none->value;
            break;
        }
        case NULLVAL:{
            this->value = "";
            break;
        }
        default:
            throw std::runtime_error("ERROR: Could not disern type.");

    }
}

Database::TextValue::TextValue(std::string fieldName, bool aIsUnique) : Value(fieldName, TEXT, aIsUnique) {
    this->value = "";
}

Database::IntValue::IntValue(Value& other): Value(other.fieldName, Type::INT) {
    switch(other.type){
        case TEXT:{
            TextValue* text = static_cast<TextValue*>(&other);
            this->value = std::stoi(text->value);
            break;
        }
        case INT:{
            IntValue* integer = static_cast<IntValue*>(&other);
            this->value = integer->value;
            break;
        }
        case DECIMAL:{
            DecimalValue* decimal = static_cast<DecimalValue*>(&other);
            this->value = decimal->value;
            break;
        }
        case BOOL:{

            BoolValue* boolean = static_cast<BoolValue*>(&other);
            this->value = boolean->value;
            break;
        }
        case BLOB:
            throw std::runtime_error("ERROR: Attempted to convert BlobValue to IntValue");
            break;
        case NONE:{
            this->value = 0;
            break;
        }
        case NULLVAL:{
            this->value = 0;
            break;
        }
        default:
            throw std::runtime_error("ERROR: Could not disern type.");

    }
}
Database::IntValue::IntValue(std::string fieldName, bool aIsUnique) : Value(fieldName, INT, aIsUnique) {
    value = 0;
}

Database::DecimalValue::DecimalValue(Value &other) : Value(other.fieldName, Type::DECIMAL)
{
    switch(other.type){
        case TEXT:{
            TextValue* text = static_cast<TextValue*>(&other);
            this->value = std::stod(text->value);
            break;
        }
        case INT:{
            IntValue* integer = static_cast<IntValue*>(&other);
            this->value = integer->value;
        }
            break;
        case DECIMAL:{
            DecimalValue* decimal = static_cast<DecimalValue*>(&other);
            this->value = decimal->value;
            break;
        }
        case BOOL:{

            BoolValue* boolean = static_cast<BoolValue*>(&other);
            this->value = boolean->value;
            break;
        }
        case BLOB:{
            throw std::runtime_error("ERROR: Attempted to convert BlobValue to DecimalValue");
            break;
        }
        case NONE:{
            this->value = 0;
            break;
        }
        case NULLVAL:{
            this->value = 0;
            break;
        }
        default:
            throw std::runtime_error("ERROR: Could not disern type.");

    }
}
Database::DecimalValue::DecimalValue(std::string fieldName, bool aIsUnique) : Value(fieldName, DECIMAL, aIsUnique) {
    value = 0.0;
}
Database::BoolValue::BoolValue(Value &other) : Value(other.fieldName, Type::BOOL)
{

    switch(other.type){
        case TEXT:{
            TextValue* text = static_cast<TextValue*>(&other);
            this->value = std::stoi(text->value);
            break;
        }
        case INT:{
            IntValue* integer = static_cast<IntValue*>(&other);
            this->value = integer->value;
            break;
        }
        case DECIMAL:{
            DecimalValue* decimal = static_cast<DecimalValue*>(&other);
            this->value = decimal->value;
            break;
        }
        case BOOL:{

            BoolValue* boolean = static_cast<BoolValue*>(&other);
            this->value = boolean->value;
            break;
        }
        case BLOB:{
            throw std::runtime_error("ERROR: Attempted to convert BlobValue to BoolValue");
            break;
        }
        case NONE:{
            this->value = 0;
            break;
        }
        case NULLVAL:{
            this->value = 0;
            break;
        }
        default:{
            throw std::runtime_error("ERROR: Could not disern type.");
        }

    }
}

Database::BlobValue::BlobValue(std::string aFieldName, const void *aData, size_t size): Value(aFieldName, Type::BLOB) {
    char* charDat = (char*)aData;

    for(size_t i = 0; i < size; i++){
        this->data.push_back(static_cast<std::byte>(charDat[i]));
    }

}
Database::BlobValue::BlobValue(Value &other) : Value(other.fieldName, Type::BLOB)
{
    BlobValue *temp;

    switch(other.type){
        case TEXT:{
            throw std::runtime_error("ERROR: Attempted to convert TextValue to BoolValue");
            break;
        }
        case INT:{
            throw std::runtime_error("ERROR: Attempted to convert IntValue to BoolValue");
            break;
        }
        case DECIMAL:{
            throw std::runtime_error("ERROR: Attempted to convert DecimalValue to BoolValue");
            break;
        }
        case BOOL:{
            throw std::runtime_error("ERROR: Attempted to convert BoolValue to BlobValue");
            break;
        }
        case BLOB:{
            temp = static_cast<BlobValue*>(&other);
            this->data = temp->data;
            break;

        }
        case NULLVAL:{
            this->data = {};
            break;
        }

        case NONE:
            throw std::runtime_error("ERROR: Attempted to convert BlobValue to NONE");
            break;
        default:
            throw std::runtime_error("ERROR: Could not disern type.");

    }
}

Database::NoneValue::NoneValue(std::string aVal): Value(aVal, Type::NONE){
    this->value = aVal;
}

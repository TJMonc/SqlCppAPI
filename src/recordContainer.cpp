#include "sqlite3c++.h"


Database::RecordContainer::RecordContainer(Model& aModel, std::string baseSql, std::vector<std::string> vals) : model(aModel), values(vals){
    auto dat = model.db->preparedSelectQuery(baseSql, values);
    for(size_t i = 1; i < dat.size(); i++){
        Record singleRecord(model);

        for(size_t j = 0; j < dat.at(i).size(); j++){
            auto& singleVal = dat.at(i).at(j);
            if(singleVal->fieldName == singleRecord.id.fieldName && singleVal->type == INT){
                singleRecord.id = *static_cast<IntValue*>(singleVal.get());
            }
            if(singleRecord.fields.contains(singleVal->fieldName)){
                singleRecord.fields.at(singleVal->fieldName) = std::move(valToPtr(*singleVal));
            }
            else{
                throw std::runtime_error("Could not find column");
            }
            
        }
        records.push_back(std::make_unique<Record>(std::move(singleRecord)));

    }
    
    
}

Database::RecordContainer Database::RecordContainer::filter(std::string var, char op, std::string condition)
{
    std::string queryCode = baseSqlCode + " AND " + var + op + "?";
    std::vector<std::string> newValues = values;
    newValues.push_back(condition);

    return RecordContainer(model, queryCode, newValues);
}

void Database::RecordContainer::removeAll() {
    for (size_t i = 0; i < records.size(); i++) {
        this->at(i).remove();
    }
    baseSqlCode = "";

    records.clear();
}

Database::Record &Database::RecordContainer::at(const int index)
{
    return *records.at(index);
}

Database::Record &Database::RecordContainer::operator[](const int index) {
    return this->at(index);
}

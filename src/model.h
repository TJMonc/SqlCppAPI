#pragma once
#include "sqlite3c++.h"

class Test : public Database::Model {
    public:
    Database::TextValue f_name = Database::TextValue("", false);
    Database::IntValue f_age = Database::IntValue("", false);
public:
    Test(Database* aDb) : Model(aDb, "newTest"){
        f_name = makeText(*db, name, "Name", false, false);
        f_age = makeInt(*db, name, "Age", false, false);
    };
};
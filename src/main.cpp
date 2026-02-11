#include <iostream>
#include <sqlite3.h>
#include <format>
#include <iomanip>
#include "model.h"



int main(int argc, char** argv){

    
    std::string name = argv[1];

    Database k(name);
    std::cout << "name";
    Test testTable(&k);

    std::vector<std::string> names = {"Terrance Jr", "Marquita", "Jordan", "Terrance Sr"};
    std::vector<int> ages = {21, 46, 19, 46};
    


   // k.preparedQuery("INSERT INTO test VALUES ( ?, ? )", {&text, &integer});


    auto dat = testTable.get();
        for(size_t i = 0; i < dat.size(); i++){
        auto& val = dat.at(i);
        Database::TextValue& name = (Database::TextValue&)val["Name"];
        Database::IntValue& age = (Database::IntValue&)val["Age"];
        name = "New";
        age = 100;
        
        val.save();
    }
    for(size_t i = 0; i < dat.size(); i++){
        auto& val = dat.at(i);
        std::cout << val["newTestid"] << "    ";
        std::cout << val["Name"] << "     ";
        std::cout << val["Age"] << "     \n ";

    }

     
    return 0;


}
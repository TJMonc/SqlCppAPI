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
    


    for(size_t i = 0; i < names.size(); i++){
        testTable.insert({names.at(i), std::to_string(ages.at(i))}, {"Name", "Age"});
    }

    std::cout << "\nInsert done\n";
    auto dat = testTable.get();
    std::cout << "\nGet done\n";

    for(size_t i = 0; i < dat.size(); i++){
        auto& val = dat.at(i);

        val.remove();

    }

     
    return 0;


}
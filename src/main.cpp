#include <iostream>
#include <sqlite3.h>
#include <format>
#include <iomanip>
#include "sqlite3c++.h"



int main(int argc, char** argv){


    std::string name = argv[1];
    std::string sql = argv[2];

    Database k(name);
    Database::TextValue text("Name");
    Database::IntValue integer("Age");

    text.value = "Claudia Moncure";
    integer.value = 71;

   // k.preparedQuery("INSERT INTO test VALUES ( ?, ? )", {&text, &integer});
    auto dat = k.preparedSelectQuery("SELECT * FROM test");

    for(int i = 0; i < dat.size(); i++){
        std::cout << i << ". ";
        for(int j = 0; j < dat.at(i).size(); j++){
            std::cout << dat.at(i).at(j)->toString();

            size_t len = dat.at(i).at(j)->toString().size();
            size_t padding = (len < 25) ? (25 - len) : 0;

            for(int z = 0; z < padding; z++){
                std::cout << " ";
            }
            
        }
        std::cout << "\n";
    }


     
    return 0;


}
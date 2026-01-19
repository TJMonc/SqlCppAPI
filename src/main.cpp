#include <iostream>
#include <sqlite3.h>
#include <format>
#include <iomanip>
#include "sqlite3c++.h"



int main(int argc, char** argv){

    if(argc != 3){
        throw std::runtime_error("Must have 2 arguments");
    }
    std::string name = argv[1];
    std::string sql = argv[2];

    Database k(name);
    auto rc = k.preparedQuery(sql, {"Ricky Moncure", "22"});

    auto dat = k.preparedSelectQuery("SELECT * FROM test");

    for(int i = 0; i < dat.size(); i++){
        std::cout << i << ". ";
        for(int j = 0; j < dat.at(i).size(); j++){
            std::cout << dat.at(i).at(j)->getValue();

            size_t len = dat.at(i).at(j)->getValue().size();
            size_t padding = (len < 25) ? (25 - len) : 0;

            for(int z = 0; z < padding; z++){
                std::cout << " ";
            }
            
        }
        std::cout << "\n";
    }

     
    return 0;


}
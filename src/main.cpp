#include <iostream>
#include <sqlite3.h>
#include <format>
#include "sqlite3c++.h"


int main(int argc, char** argv){

    std::string sql = argv[1];

    Database k("test.db");
    auto dat = k.selectQuery(sql);

    for(int i = 0; i < dat.size(); i++){
        std::cout << i << ". ";
        for(int j = 0; j < dat.at(i).size(); j++){
            std::cout << dat.at(i).at(j) << "   ";
        }
        std::cout << "\n";
    }


     



}
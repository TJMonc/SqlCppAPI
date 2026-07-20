#include <iostream>
#include <SQLiteDatabase.h>
using namespace DB;
int main(int argc, char** argv){
    DB::IDatabase* db = new DB::SQLiteDatabase("test.db");

    QuerySet select = db->select("SELECT * FROM new", {});
    auto lol = DB::DBValue("ddd");
    DB::DBValueConverter l;


    for(size_t i = 0; i < select.data.size(); i++){
        auto& dat = select.data[i];
        for(size_t j = 0; j < dat.values.size(); j++){
            std::cout << DB::DBValueConverter::fromDBValue<DB_String>(dat[select.colNames[j]]) << " | ";
        }
        std::cout << "\n";
    }

}
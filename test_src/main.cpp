#include <iostream>
#include <SQLiteDatabase.h>
using namespace DB;
int main(int argc, char** argv){
    DB::IDatabase* db = new DB::SQLiteDatabase("test.db");
    db->execute("CREATE TABLE IF NOT EXISTS people (name TEXT, age INTEGER)", {});
    db->execute("INSERT INTO people VALUES(?, ?)", {"Jordan Moncure", 20});
    db->execute("INSERT INTO people VALUES(?, ?)", {"Terrance Moncure Sr", 76});


    QuerySet select = db->select("SELECT * FROM people", {});
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
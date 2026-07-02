#include <iostream>
#include "IDatabase.h"
using namespace DB;
int main(int argc, char** argv){
    DBValue value = DB_String("123");
    int result = DBValueConverter::fromDBValue<DB_Int>(value);
}
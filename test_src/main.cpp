#include <iostream>
#include <SQLiteDatabase.h>
#include <SQLiteQueryBuilder.h>
using namespace DB;
int main(int argc, char** argv){
    std::vector<FieldSchema> fields = {
        {DB::Type::DB_INT_TYPE, "student_id", "Students", std::monostate{}, true, true, false, true, false, 0},
        {DB::Type::DB_STRING_TYPE, "first_name", "Students", std::monostate{}, false, false, false, false, false, 0},
        {DB::Type::DB_STRING_TYPE, "last_name", "Students", std::monostate{}, false, false, false, false, false, 0}
    };
    TableSchema table = {"Students", fields[0], fields};
    DB::IDatabase* db = new DB::SQLiteDatabase("test.db");
    DB::IQueryBuilder* qb = new SQLiteQueryBuilder();
    qb->makeTable(table).endStatement().insert(table, {DBValue("Terrance"), DBValue("Moncure")}, {fields[1], fields[2]});
    std::cout << qb->query + "\n\n";
    db->execute(qb->query, qb->aggregateParams);

    *qb = DB::SQLiteQueryBuilder();

    qb->select(table, false).where(fields[1] == "Terrance");
    std::cout << qb->query + "\n\n";
    db->execute(qb->query, qb->aggregateParams);




    std::cout << qb->query;

}
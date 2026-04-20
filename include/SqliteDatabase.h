#include "sqlite3.h"
#include "IDatabase.h"
namespace DB{
    class SqliteDatabase : public IDatabase{
        private:
            sqlite3* m_connection;

            int bind(sqlite3_stmt* a_stmt, int a_pos, const Value& a_val, bool isStatic);

        public:
            virtual int execute(const std::string a_query, const std::vector<Value>& a_vals);
            virtual ResultSet select(const std::string a_query, const std::vector<Value>& a_vals);
            virtual bool supports(Hint h);

            SqliteDatabase(const std::string dbPath);
            ~SqliteDatabase();

    };
    template<>
    struct ValueConverter<int64_t>{
        static int64_t fromIValue(const Value& a_val);
        static Value toIvalue(const int64_t& a_val);

    };


}
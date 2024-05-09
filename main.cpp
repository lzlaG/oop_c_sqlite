#include <iostream>
#include <string>
#include "sqlite3.h"
#include "main.h"

using namespace std;


int DBMutantContainerIterator::GetCount()
{
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(DB, "SELECT COUNT(*) FROM Mutants", -1, &stmt, 0);
    result = sqlite3_step(stmt);
    int count = sqlite3_column_int(stmt, 0);
    Count = count;
    return count;
};


void DBMutantContainerIterator::First()
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM Mutants ORDER BY id ASC LIMIT 1;";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    rc = sqlite3_step(stmt);
    int id = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
};


int main()
{
    DBMutantContainerIterator it("mydb.db");
}

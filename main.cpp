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
    sqlite3_finalize(stmt);
    return count;
};


string DBMutantContainerIterator::GetHandPower()
{
    sqlite3_stmt* stmt;
    const char *sql = "SELECT StrengthOfHands FROM Mutants WHERE ID = ?;";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, CurrentId);
    rc = sqlite3_step(stmt);
    string HandPower = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);
    return HandPower;
};


string DBMutantContainerIterator::GetLegPower()
{
    sqlite3_stmt* stmt;
    const char *sql = "SELECT StrengthOfLegs FROM Mutants WHERE ID = ?;";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, CurrentId);
    rc = sqlite3_step(stmt);
    string LegPower = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);
    return LegPower;
};


string DBMutantContainerIterator::GetAge()
{
    sqlite3_stmt* stmt;
    const char *sql = "SELECT Age FROM Mutants WHERE ID = ?;";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, CurrentId);
    rc = sqlite3_step(stmt);
    string Age = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);
    return Age;
};


string DBMutantContainerIterator::GetType()
{
    sqlite3_stmt* stmt;
    const char *sql = "SELECT MutantType FROM Mutants WHERE ID = ?;";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, CurrentId);
    rc = sqlite3_step(stmt);
    string TypeOfMutant = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);
    return TypeOfMutant;
};


void DBMutantContainerIterator::First()
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM Mutants ORDER BY id ASC LIMIT 1;";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    rc = sqlite3_step(stmt);
    CurrentId = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
};

int main()
{
    DBMutantContainerIterator it("mydb.db");
    it.First();
    cout << it.GetType() << "\n";
}

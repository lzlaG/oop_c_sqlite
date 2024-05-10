#include <iostream>
#include <string>
#include "sqlite3.h"
#include "main.h"

using namespace std;

//-------------- classes -----------------
class Gargoyle : public Scum
{
    public:
        MutantType GetType() const {return MutantType::Gargoyle;};
        void Fly(int x) const {cout << "Горгулья пролетела " << x << " км" << endl;}
        void Summon()   const { cout << " Призвали горгулью " << endl;};
        void Kill() const { cout << " убили горгулью " << endl;};
};

class Wolfman : public Scum
{
    public:
        MutantType GetType() const {return MutantType::Wolfman;};
        void Run(int x) { cout << "Оборотень пробежал " << x << " км " << endl;}
        void Summon() const { cout << " Призвали оборотня " << "\n";};
        void Kill() const { cout << " убили оборотня " << "\n";};
};

class Vampire : public Scum
{
    public:
        MutantType GetType() const {return MutantType::Vampire;}
        void Blood(int x) {cout << "Вампир выпил " << x << " литров крови " << endl;}
        void Summon() const { cout << " Призвали вампира " << "\n";}
        void Kill() const { cout << " убили вампира" << "\n";}
};

//---------- function for add string in sql------------------
string quotesql( const string& s ) {
    return string("'") + s + string("'");
};

//------pirnts-------------
string PrintMutantType(const MutantType type)
{
    switch(type)
	{
		case MutantType::Gargoyle: return  "ГОРГУЛЬЯ";
		case MutantType::Vampire: return  "ВАМПИР";
		case MutantType::Wolfman: return  "ОБОРОТЕНЬ";
		default: return "неизвестный";
	}
};

string PrintLegPower (const StregthOfLegs type)
{
    switch (type)
    {
        case StregthOfLegs::Low: return "Слабые ноги";
        case StregthOfLegs::Medium: return "Средние ноги";
        case StregthOfLegs::High: return "Сильные ноги";
    }
};

string PrintHandPower (const StregthOfHands type)
{
    switch (type)
    {
        case StregthOfHands::Low: return "Слабые руки";
        case StregthOfHands::Medium: return "Средние руки";
        case StregthOfHands::High: return "Мощные руки";
    }
};

string PrintAgeOfMutant(const Age type)
{
    switch(type)
    {
        case Age::Old: return "ПОЖИЛОЙ";
        case Age::Young: return "МОЛОДОЙ";
        case Age::Newborn: return "Новорожденный";
    }
};



//--------------db iterator----------------------
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

//----------- db iterator functions----------------
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

int DBMutantContainerIterator::GetCount()
{
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(DB, "SELECT COUNT(*) FROM Mutants", -1, &stmt, 0);
    result = sqlite3_step(stmt);
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return count;
};

void DBMutantContainerIterator::First()
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT ID FROM Mutants ORDER BY ID ASC LIMIT 1;";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    rc = sqlite3_step(stmt);
    CurrentId = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
};
//------------------- db container functions ----------------------
void DBMutantContainer::ClearDB()
{
    char *errmsg;
    sqlite3_exec(DB,"DELETE FROM Mutants", NULL, NULL, &errmsg);
};

void DBMutantContainer::AddMutant(ScumPointer newMutant)
{
    string mutanttype = PrintMutantType(newMutant->GetType());
    string handpower = PrintHandPower(newMutant->GetHandPower());
    string legpower = PrintLegPower(newMutant->GetLegPower());
    string ageofmutant = PrintAgeOfMutant(newMutant->GetAgeOfMutant());
    string query = "INSERT INTO Mutants (MutantType, StrengthOfHands, StrengthOfLegs, Age) VALUES ("
    +quotesql(mutanttype)+","
    +quotesql(handpower)+","
    +quotesql(legpower)+","
    +quotesql(ageofmutant)+");";
    char *errmsg;
    sqlite3_exec(DB, query.c_str(), NULL, NULL, &errmsg);
    cout << errmsg << "\n";
};

// ---------------------- factory method----------------------
Scum *MutantFactory(MutantType newMutant)
{
    switch(newMutant)
    {
        case MutantType::Gargoyle: return  new Gargoyle;
		case MutantType::Vampire: return  new Vampire;
		case MutantType::Wolfman: return  new Wolfman;
    }
};
//----------------- task ---------------------

void task(DBMutantContainerIterator it)
{
    it.First();
    int amount_of_mutants = it.GetCount();
    for (int i = 0; i<amount_of_mutants; i++)
    {
        DecoratorType dec(it.GetType());
        dec.FindType("ВАМПИР");
        if (dec.GetCorrect() == true)
        {
            cout << "~~~~~~~~~~~~~~~~~~~~" << "\n";
            cout << "Тип: " << it.GetType() << "\n";
            cout << "Сила ног: " << it.GetHandPower() << "\n";
            cout << "Сила рук: " << it.GetLegPower() << "\n";
            cout << "Возраст: " << it.GetAge() << "\n";
        };
        it.Next();
    };
}

int main()
{
    srand(time(NULL));
    
    sqlite3* DB;
    sqlite3_open("mydb.db", &DB);
    /*
    DBMutantContainer scumcell(DB);
    scumcell.ClearDB();
    int random_amount_of_mutant = random()%(100-10+1)+1;
    cout << "Генерируем " << random_amount_of_mutant << " мутантов" << "\n";
    for (int i=0; i<random_amount_of_mutant; i++)
    {
        scumcell.AddMutant(MutantFactory(MutantType(rand()%3)));
    };*/
    DBMutantContainerIterator IT(DB);
    task(IT);
};

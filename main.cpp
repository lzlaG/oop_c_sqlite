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
        case Age::Newborn: return "МЛАДЕНЕЦ";
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
    sqlite3_stmt* stmt;
    string insert_query = "INSERT INTO Mutants (MutantType,StrengthOfHands,StrengthOfLegs, Age)"
                            "VALUES (:mutanttype,:power_of_hands,:power_of_legs, :age);";
    sqlite3_prepare_v2(DB, insert_query.c_str(), -1, &stmt, NULL);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":mutanttype"), mutanttype.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":power_of_hands"), handpower.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":power_of_legs"), legpower.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":age"), ageofmutant.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
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

void task(DBMutantContainerIterator it, 
    int type_user_choise,
    int leg_power_user_choise,
    int hand_power_user_choise,
    int age_user_choise)
{
    it.First();
    int amount_of_mutants = it.GetCount();
    for (int i = 0; i<amount_of_mutants; i++)
    {
        Decorator dec_of_type(it.GetType());
        Decorator dec_of_leg(it.GetLegPower());
        Decorator dec_of_age(it.GetAge());
        Decorator dec_of_hands(it.GetHandPower());

        switch(type_user_choise)
        {
            case 1:
                dec_of_leg.Find("ВАМПИР");
                break;
            case 2:
                dec_of_type.Find("ОБОРОТЕНЬ");
                break;
            case 3:
                dec_of_type.Find("ГОРГУЛЬЯ");
                break;
            case 4:
                break;
        };

        switch(leg_power_user_choise)
        {
            case 1:
                dec_of_leg.Find("Сильные ноги");
                break;
            case 2:
                dec_of_leg.Find("Средние ноги");
                break;
            case 3:
                dec_of_leg.Find("Слабые ноги");
                break;
            case 4:
                break;
        };

        switch(hand_power_user_choise)
        {
            case 1:
                dec_of_hands.Find("Сильные руки");
                break;
            case 2:
                dec_of_hands.Find("Средние руки");
                break;
            case 3:
                dec_of_hands.Find("Слабые руки");
                break;
            case 4:
                break;
        };

        switch(age_user_choise)
        {
            case 1:
                dec_of_age.Find("ПОЖИЛОЙ");
                break;
            case 2:
                dec_of_age.Find("МОЛОДОЙ");
                break;
            case 3:
                dec_of_age.Find("МЛАДЕНЕЦ");
                break;
            case 4:
                break;
        };
        if (dec_of_type.GetCorrect() == true && 
            dec_of_age.GetCorrect() == true && 
            dec_of_leg.GetCorrect() == true && 
            dec_of_hands.GetCorrect() == true)
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
    
    DBMutantContainer scumcell(DB);
    scumcell.ClearDB();
    int random_amount_of_mutant = random()%(100-10+1)+1;
    cout << "Генерируем " << random_amount_of_mutant << " мутантов" << "\n";
    for (int i=0; i<random_amount_of_mutant; i++)
    {
        scumcell.AddMutant(MutantFactory(MutantType(rand()%3)));
    };
    DBMutantContainerIterator IT(DB);
    
    int choise_of_type;
    cout << "Выберите фильтр для типа мутанта: \n";
    cout << "1. ВАМПИР\n";
    cout << "2. ОБОРОТЕНЬ\n";
    cout << "3. ГОРГУЛЬЯ\n";
    cout << "4. Все\n";
    cout << "Ваш выбор: ";
    cin >> choise_of_type;
    system("clear");

    int choise_of_leg_power;
    cout << "Выберите фильтр для силы ног: \n";
    cout << "1. Сильные ноги\n";
    cout << "2. Средние ноги\n";
    cout << "3. Слабые ноги\n";
    cout << "4. Все\n";
    cout << "Ваш выбор: ";
    cin >> choise_of_leg_power;
    system("clear");

    int choise_of_hand_power;
    cout << "Выберите фильтр для силы рук: \n";
    cout << "1. Сильные руки\n";
    cout << "2. Средние руки\n";
    cout << "3. Слабые руки\n";
    cout << "4. Все\n";
    cout << "Ваш выбор: ";
    cin >> choise_of_hand_power;
    system("clear");

    int choise_of_age;
    cout << "Выберите возраст мутанта: \n";
    cout << "1. ПОЖИЛОЙ\n";
    cout << "2. МОЛОДОЙ\n";
    cout << "3. МЛАДЕНЕЦ\n";
    cout << "4. Все\n";
    cout << "Ваш выбор: ";
    cin >> choise_of_age;
    system("clear");

    task(IT, choise_of_type, choise_of_leg_power, choise_of_hand_power, choise_of_age);
};

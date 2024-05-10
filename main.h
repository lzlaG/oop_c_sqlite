#ifndef FC_sqlH

using namespace std;

enum class MutantType : int {Gargoyle, Wolfman, Vampire, Unknown};

enum class StregthOfHands : int {High, Medium, Low};

enum class StregthOfLegs : int {High, Medium, Low};

enum class Age : int {Old, Young, Newborn};


class Scum
{
    protected:
        StregthOfHands HandPower;
        StregthOfLegs LegPower;
        Age AgeOfMutant;
        Scum()
        {
            HandPower = StregthOfHands(rand()%3);
            LegPower = StregthOfLegs(rand()%3);
            AgeOfMutant = Age(rand()%3);
        };
    public:
        StregthOfHands GetHandPower() {return HandPower;};
        StregthOfLegs GetLegPower() {return LegPower;};
        Age GetAgeOfMutant() {return AgeOfMutant;};
        virtual void Summon() const = 0;
        virtual void Kill() const = 0;
        virtual MutantType GetType() const = 0;
};

typedef Scum * ScumPointer;

class DBMutantContainer
{
    private:
        sqlite3* DB;
    public:
        DBMutantContainer(sqlite3* db)
        {
            DB = db;
            string createtable = "CREATE TABLE IF NOT EXISTS Mutants ("
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "MutantType TEXT NOT NULL,"
                                "StrengthOfHands TEXT,"
                                "StrengthOfLegs TEXT,"
                                "Age TEXT"
                                ");";
            char *errMsg;
            sqlite3_exec(DB, createtable.c_str(), nullptr, nullptr, &errMsg);
            cout << errMsg << "\n";
        };
        void AddMutant(ScumPointer newMutant);
        void ClearDB();
        void CloseDB() {sqlite3_close(DB);}
};

class DBMutantContainerIterator
{
    private:
        int CurrentId;
        sqlite3* DB;
    public:
        DBMutantContainerIterator(sqlite3* db)
        {
            DB = db;
        };
        void First();
        int GetCount();
        string GetHandPower();
        string GetLegPower();
        string GetType();
        string GetAge();
        void Next() {CurrentId++;};
        //bool IsDone(int i) {return i>Count;};
};

class DecoratorType
{
    private:
        string TargetType;
        string CurrentType;
        bool IsCorrect;
    public:
        DecoratorType(string currenttype)
        {
            CurrentType = currenttype;
        };
        void FindType(string targettype)
        {
            TargetType = targettype;
            if (CurrentType == TargetType)
            {
                IsCorrect = true;
            }
            else
            {
                IsCorrect = false;
            }
        };
        bool GetCorrect() {return IsCorrect;}
};

#endif FC_sqlH

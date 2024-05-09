#ifndef FC_sqlH

using namespace std;

class DBMutantContainerIterator
{
private:
    int CurrentId;
    sqlite3 * DB;
    int Count;
public:
    DBMutantContainerIterator(const string& DBName)
    {
        int db = sqlite3_open(DBName.c_str(), &DB);
    };
    void First();
    int GetCount();
    string GetHandPower();
    string GetLegPower();
    string GetType();
    string GetAge();
    void Next() {CurrentId++;};
    bool IsDone(int i) {return i>Count;};
};

#endif FC_sqlH

#ifndef FC_sqlH

using namespace std;

class DBMutantContainerIterator
{
private:
    int CurrentId;
    sqlite3 * DB;
public:
    DBMutantContainerIterator(const string& DBName)
    {
        int db = sqlite3_open(DBName.c_str(), &DB);
    };
    void First();
    void GetCount();
    //void Next() {CurrentId++;};
    //bool IsDone() {return 1;};
    //ScumPointer GetCurrent() {return * ScumCell;};
};

#endif FC_sqlH

#include <vector>
#include <map>
#include <string>
using namespace std;

enum eMANBER {NONE, TEACHER, STUDENT};

class Manber
{
private:
    eMANBER type;
    string Name;
    int age;

protected:
    void setName(string name) { this->Name = name; }
    void setAge(int age) { this->age = age; }

public:
    Manber() { setType(eMANBER(NONE)); setData("", 0); }
    ~Manber();

    void setType(eMANBER);
    void setData(string, int);

    map<int, vector<string>> getData();
};
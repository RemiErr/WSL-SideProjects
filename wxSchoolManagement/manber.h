#include <string>
using namespace std;

enum eMANBER {NONE, ADMIN, TEACHER, STUDENT};
enum {};

class Manber
{
private:
    string joinTime;
    string Name;
    string ID;
    int age;

public:
    Manber() { init(); }
    ~Manber();
    
    void init();
    
    void setJoinTome(string time) { this->joinTime = time; }
    void setName(string name) { this->Name = name; }
    void setID(string ID) { this->ID = ID; }
    void setAge(int age) { this->age = age; }
};
#include "manber.h"
#include "score.h"

class Student:
    public Manber
{
private:
    string classroom;

public:
    Student();
    ~Student();

    void exam();
    void setClassRoom(string classroom) { this->classroom = classroom; }

    void getClassRoom();
};
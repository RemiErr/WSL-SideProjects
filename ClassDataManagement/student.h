#include <vector>
#include "manber.h"

enum eEXAM { FIRST, MIDTERM, FINAL };

struct stScores
{
    int CH_TW;
    int ENG;
    int MATH;
    int PRO_I;
    int PRO_II;
};

class Student:
    public Manber
{
private:
    string id;
    string classroom;
    stScores score;
    eEXAM exam;

public:
    Student() { setType(eMANBER(STUDENT)); this->exam = FIRST; };
    ~Student();

    void doExam();
    void setID(string);
    void setClassRoom(string);
    void setScores(stScores);

    string getClassRoom();
    vector<int> getScores();
};
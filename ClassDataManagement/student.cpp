#include <random>
#include "student.h"

// 產生 0~100 內的分數
int doRand()
{
    srand( time(NULL) );
    return rand() % 101;
}

void Student::doExam()
{
    srand( time(NULL) );
    if ( exam >= FINAL ) this->exam = FIRST;
    else
    {
        int t = int(this->exam);
        this->exam = eEXAM(++t);
    }

    setScores({
        doRand(), // CH_TW
        doRand(), // ENG
        doRand(), // MATH
        doRand(), // PRO_I
        doRand()  // PRO_II
    });
}

void Student::setClassRoom(string classroom)
{
    this->classroom = classroom;
}

void Student::setID(string id)
{
    this->id = id;
}

void Student::setScores(stScores score)
{
    this->score = score;
}

string Student::getClassRoom()
{
    return this->classroom;
}

vector<int> Student::getScores()
{
    return
    {
        this->score.CH_TW,
        this->score.ENG,
        this->score.MATH,
        this->score.PRO_I,
        this->score.PRO_II
    };
}
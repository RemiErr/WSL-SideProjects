#include "manber.h"

enum eSUBJECTS { CH_TW, ENG, MATH, PRO_I, PRO_II };

class Teacher:
    public Manber
{
private:
    int subj;
    int work_years; // 工作年數

public:
    Teacher();
    ~Teacher();

    void setSubj(eSUBJECTS subj) { this->subj = subj; }
    void setWorkYears(int years) { this->work_years = years; }
};
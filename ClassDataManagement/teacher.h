#include "manber.h"

enum eSUBJECTS { NONE, CH_TW, ENG, MATH, PRO_I, PRO_II };

class Teacher:
    public Manber
{
private:
    eSUBJECTS subj; // 主力科目
    int work_years; // 工作年數

public:
    Teacher() { setType(eMANBER(TEACHER)); this->subj = eSUBJECTS(NONE); };
    ~Teacher();

    void setSubj(int);
    void setWorkYears(int);

    string getSubj();
    int getWorkYears();
};
#include "teacher.h"

void Teacher::setSubj(int subj)
{
    this->subj = eSUBJECTS(subj);
}

void Teacher::setWorkYears(int years)
{
    this->work_years = years;
}

string Teacher::getSubj()
{
    switch (this->subj)
    {
    case CH_TW:
        return "國文";
    case ENG:
        return "英文";
    case MATH:
        return "數學";
    case PRO_I:
        return "專業(一)";
    case PRO_II:
        return "專業(二)";
    default:
        return "None";
    }
}

int Teacher::getWorkYears()
{
    return this->work_years;
}
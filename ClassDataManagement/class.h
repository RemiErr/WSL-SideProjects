#include "teacher.h"
#include "student.h"

class ClassRoom
{
private:
    Teacher tutor;        // 班級導師
    Teacher teacher[5];   // 任課老師
    Student students[30]; // 一個班級 最多30位學生

public:
    ClassRoom(){};
    ~ClassRoom(){};
};
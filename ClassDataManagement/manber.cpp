#include "manber.h"

void Manber::setType(eMANBER type)
{
    this->type = type;
}

void Manber::setData(string name, int age)
{
    setName(name);
    setAge(age);
}

map<int, vector<string>> Manber::getData()
{
    return
    {
        { int(this->type), { this->Name, to_string(this->age) } }
    };
}
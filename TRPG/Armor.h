#ifndef ARMOR_H
#define ARMOR_H

#include <iostream>
#include <string>
using namespace std;

// 基礎防具資料
class Armor
{
protected:
    string Name;
    int DEF;
    int Price;

public:
    Armor(string name = "", int def = 0, int price = 0)
    {
        Name = name;
        DEF = def;
        Price = price;
    }
    ~Armor(){}
    
};

// 各職業防具
class BerserkerArmor
{
public:
    BerserkerArmor(string name, int def, int price)
    { Armor(name, def, price); }

    ~BerserkerArmor(){}
};
class TankArmor:
    public Armor
{
public:
    TankArmor(string name, int atk, int price)
    { Armor(name, atk, price); }

    ~TankArmor(){}
};

class AssassinArmor:
    public Armor
{
public:
    AssassinArmor(string name, int atk, int price)
    { Armor(name, atk, price); }

    ~AssassinArmor(){}
};

class ArmorShop:
    public Armor
{
public:
    ArmorShop(){}
    ~ArmorShop(){}

    Armor *makeArmor(int choose);
    Armor *makeArmor(int mob, int choose);
};
#endif
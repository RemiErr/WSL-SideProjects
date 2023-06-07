#ifndef ARMOR_H
#define ARMOR_H

#include "FileManager.h"

// 基礎防具資料
class Armor
{
protected:
    string Name;
    int Health;
    int DEF;
    int Price;

public:
    Armor(string name = "", int def = 0, int health = 0, int price = 0)
    {
        Name = name;
        Health = health;
        DEF = def;
        Price = price;
    }
    ~Armor(){}

    string getName() { return Name; }
    int getHealth() { return Health; }
    int getDEF() { return DEF; }
    int getPrice() { return Price; }
};

// 各職業防具
class BerserkerArmor:
    public Armor
{
public:
    BerserkerArmor(string name, map<string, vector<int>> &arm)
    { 
        if (!arm[name].empty())
        {
            Name = name;
            Health = arm[name][0];
            DEF = arm[name][1];
            Price = arm[name][2];
            Armor(name, DEF, Health, Price);
        }
        else
            cout<<"Error: 未建立物件 - BerserkerArmor\n";
    }

    ~BerserkerArmor(){}
};

class TankArmor:
    public Armor
{
public:
    TankArmor(string name, map<string, vector<int>> &arm)
    { 
        if (!arm[name].empty())
        {
            Name = name;
            Health = arm[name][0] * 2;
            DEF = round(arm[name][1] * 1.5);
            Price = arm[name][2];
            Armor(name, DEF, Health, Price);
        }
        else
            cout<<"Error: 未建立物件 - TankArmor\n";
    }

    ~TankArmor(){}
};

class AssassinArmor:
    public Armor
{
public:
    AssassinArmor(string name, map<string, vector<int>> &arm)
    { 
        if (!arm[name].empty())
        {
            Name = name;
            Health = round(arm[name][0] * 0.5);
            DEF = round(arm[name][1] * 0.7);
            Price = arm[name][2];
            Armor(name, DEF, Health, Price);
        }
        else
            cout<<"Error: 未建立物件 - AssassinArmor\n";
    }

    ~AssassinArmor(){}
};

class ArmorManager:
    public Armor
{
protected:
    map<string, vector<int>> armors;

public:
    ArmorManager()
    {
        FileManager file("res/Armors.csv");
        armors = file.getData();
    }
    ArmorManager(string url)
    {
        FileManager file(url);
        armors = file.getData();
    }
    ~ArmorManager(){}

    map<string, vector<int>> getList()
    {
        return armors;
    }

    Armor *makeArmor(int option, string item_name)
    {
        switch (option)
        {
        case 1: // Berserker
            return new BerserkerArmor(item_name, armors);
            break;
        case 2: // Tank
            return new TankArmor(item_name, armors);
            break;
        case 3: // Assassin
            return new AssassinArmor(item_name, armors);
            break;
        default:
            return new Armor();
            break;
        }
    }
};
#endif
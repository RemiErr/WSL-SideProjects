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
    BerserkerArmor(map<string, vector<int>> &arm, string name)
    { 
        if (!arm[name].empty())
            Armor(name, arm[name][0], arm[name][1], arm[name][2]); // 防禦 血量 價格
        else
            cout<<"Error: 未建立物件 - BerserkerArmor\n";
    }

    ~BerserkerArmor(){}
};

class TankArmor:
    public Armor
{
public:
    TankArmor(map<string, vector<int>> &arm, string name)
    { 
        if (!arm[name].empty())
            Armor(name, arm[name][0]*2, arm[name][1], arm[name][2]);
        else
            cout<<"Error: 未建立物件 - TankArmor\n";
    }

    ~TankArmor(){}
};

class AssassinArmor:
    public Armor
{
public:
    AssassinArmor(map<string, vector<int>> &arm, string name)
    { 
        if (!arm[name].empty())
            Armor(name, arm[name][0]/2, arm[name][1], arm[name][2]);
        else
            cout<<"Error: 未建立物件 - AssassinArmor\n";
    }

    ~AssassinArmor(){}
};

class ArmorShop:
    public Armor
{
protected:
    map<string, vector<int>> armors;

public:
    ArmorShop()
    {
        FileManager file("res/Armors.csv");
        armors = file.getData();
    }
    ~ArmorShop(){}

    map<string, vector<int>> getList()
    {
        return armors;
    }

    Armor *makeArmor(int option, string item_name)
    {
        switch (option)
        {
        case 1: // Berserker
            return new BerserkerArmor(armors, item_name);
            break;
        case 2: // Tank
            return new TankArmor(armors, item_name);
            break;
        case 3: // Assassin
            return new AssassinArmor(armors, item_name);
            break;
        default:
            return NULL;
            break;
        }
    }
};
#endif
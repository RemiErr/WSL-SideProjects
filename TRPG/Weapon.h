#ifndef WEAPON_H
#define WEAPON_H

#include "FileManager.h"

// 基礎武器資料
class Weapon
{
protected:
    string Name;
    int ATK;
    int Price;

public:
    Weapon(string name = "", int atk = 0, int price = 0)
    {
        Name = name;
        ATK = atk;
        Price = price;
    }
    ~Weapon(){}

    string getName() { return Name; }
    int getATK() { return ATK; }
    int getPrice() { return Price; }
};


// 各職業武器
class BerserkerWeapon:
    public Weapon
{
public:
    BerserkerWeapon(map<string, vector<int>> &arm, string name)
    { 
        if (!arm[name].empty())
            Weapon(name, arm[name][0], arm[name][1]);
        else
            cout<<"Error: 未建立物件 - TankWeapon\n";
    }

    ~BerserkerWeapon(){}
};

class TankWeapon:
    public Weapon
{
public:
    TankWeapon(map<string, vector<int>> &arm, string name)
    { 
        if (!arm[name].empty())
            Weapon(name, arm[name][0]/2, arm[name][1]);
        else
            cout<<"Error: 未建立物件 - TankWeapon\n";
    }

    ~TankWeapon(){}
};

class AssassinWeapon:
    public Weapon
{
public:
    AssassinWeapon(map<string, vector<int>> &arm, string name)
    { 
        if (!arm[name].empty())
            Weapon(name, arm[name][0]*2, arm[name][1]);
        else
            cout<<"Error: 未建立物件 - TankWeapon\n";
    }

    ~AssassinWeapon(){}
};

class WeaponShop:
    public Weapon
{
protected:
    map<string, vector<int>> weapons;

public:
    WeaponShop()
    {
        FileManager file("res/Weapons.csv");
        weapons = file.getData();
    }
    ~WeaponShop(){}

    map<string, vector<int>> getList()
    {
        return weapons;
    }

    Weapon *makeWeapon(int option, string item_name)
    {
        switch (option)
        {
        case 1: // Berserker
            return new BerserkerWeapon(weapons, item_name);
            break;
        case 2: // Tank
            return new TankWeapon(weapons, item_name);
            break;
        case 3: // Assassin
            return new AssassinWeapon(weapons, item_name);
            break;
        default:
            return NULL;
            break;
        }
    }
};
#endif
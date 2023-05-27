#ifndef WEAPON_H
#define WEAPON_H

#include "FileManager.h"

#ifndef STD_H
using namespace std;
#define cout std::cout
#define cin std::cin
#endif

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
    BerserkerWeapon(string name, map<string, vector<int>> &wep)
    { 
        if (!wep[name].empty())
        {
            Name = name;
            ATK = wep[name][0];
            Price = wep[name][1];
            Weapon(name, ATK, Price);
        }
        else
            cout<<"Error: 未建立物件 - BerserkerWeapon\n";
    }

    ~BerserkerWeapon(){}
};

class TankWeapon:
    public Weapon
{
public:
    TankWeapon(string name, map<string, vector<int>> &wep)
    { 
        if (!wep[name].empty())
        {
            Name = name;
            ATK = wep[name][0] / 2;
            Price = wep[name][1];
            Weapon(name, ATK, Price);
        }
        else
            cout<<"Error: 未建立物件 - TankWeapon\n";
    }

    ~TankWeapon(){}
};

class AssassinWeapon:
    public Weapon
{
public:
    AssassinWeapon(string name, map<string, vector<int>> &wep)
    { 
        if (!wep[name].empty())
        {
            Name = name;
            ATK = wep[name][0] * 2;
            Price = wep[name][1];
            Weapon(name, ATK, Price);
        }
        else
            cout<<"Error: 未建立物件 - AssassinWeapon\n";
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
            return new BerserkerWeapon(item_name, weapons);
            break;
        case 2: // Tank
            return new TankWeapon(item_name, weapons);
            break;
        case 3: // Assassin
            return new AssassinWeapon(item_name, weapons);
            break;
        default:
            return NULL;
            break;
        }
    }
// private:
//     void updata(string name = "", int atk = NULL, int price = NULL)
//     {
//         if (!name.empty()) Name = name;
//         if (atk != NULL) ATK = atk;
//         if (price != NULL) Price = price;
//     }
};
#endif
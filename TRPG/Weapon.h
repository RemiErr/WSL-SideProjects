#ifndef WEAPON_H
#define WEAPON_H
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

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
        fstream file("res/Weapons.csv");
        if (!file) return;
    
        string line = "";
        while(getline(file, line) && line.length())
        {
            int prev_index = 0, index = 0;

            // 整行字串分割
            vector<string> temp{};
            for (char &c: line)
            {
                index = &c - &line[0]; // 透過位址差值取得 index
                if (c == ',' || &c == &line[line.length()-1]) // 若當前字元為 逗號 或 最後一字
                {
                    temp.push_back(line.substr(prev_index, index));
                    prev_index = c==','? index+1 : index;
                }
            }

            for (auto &t: temp)
            {
                if (&t == &temp[0])
                    weapons[t] = {};
                else
                    weapons[temp[0]].push_back(stoi(t));
            }
        }
        file.close();
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
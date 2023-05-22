#ifndef ARMOR_H
#define ARMOR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

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
        fstream file("res/Armors.csv");
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
                    armors[t] = {};
                else
                    armors[temp[0]].push_back(stoi(t));
            }
        }
        file.close();
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
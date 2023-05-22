#ifndef CHARACTER_H
#define CHARACTER_H

#include "Weapon.h"
#include "Armor.h"
#include <string>
#include <vector>
using namespace std;

// 基礎角色資料
class Character
{
protected:
    string Role; // 職業名稱
    int Health_Max;
    int Health;
    int ATK;
    int DEF;
    int Money;

    // 會以函式 new 物件出來，所以用指標抓取該位址
    Weapon *weapon;
    Armor *armor;
    // 解除裝備
    void delSuit()
    {
        if (!weapon)
            delete weapon;
        if (!armor)
            delete armor;
    }
    

public:
    Character(string role = "", int health = 0, int atk = 0, int def = 0, int money = 0)
    {
        setRole(role);
        setState(health, health, atk, def);
        setMoney(money);
        weapon = new Weapon();
        armor = new Armor();
    }
    ~Character() { delSuit(); }

    // 設定職業
    void setRole(string role)
    {
        Role = role;
    }

    void setState(int health_max, int health, int atk, int def)
    {
        Health_Max = health_max;
        Health = health;
        ATK = atk;
        DEF = def;
    }

    void setMoney(int money)
    {
        Money = money;
    }

    // 設定角色武器
    void setWeapon(Weapon *wep)
    {
        Money -= wep->getPrice();
        ATK += wep->getATK();
        weapon = wep;
    }

    // 設定角色防具
    void setArmor(Armor *arm)
    {
        Money -= arm->getPrice();
        Health_Max += arm->getHealth();
        DEF += arm->getDEF();
        armor = arm;
    }

    string getRole() { return Role; }
    vector<int> getState() { return { Health_Max, Health, ATK, DEF }; }    
    int getMoney() { return Money; }
    Weapon *getWeapon() { return weapon; }
    Armor *getArmor() { return armor; }

    void onHit(int dmg)
    {
        dmg = int((dmg * dmg) / (dmg/2 + DEF) + 0.5); // 四捨五入
        Health -= dmg;
        if (Health < 0) Health = 0;
    }

    void isPotion(int recovery)
    {
        Health += recovery;
        if (Health > Health_Max)
            Health = Health_Max;
    }
};

// 各職業差異設定
class Berserker:
    public Character
{
public:
    Berserker() { Character("狂戰士", 1000, 50, 20, 500); }
    ~Berserker() { delSuit(); }
};

class Tank:
    public Character
{
public:
    Tank() { Character("坦克", 1600, 15, 60, 500); }
    ~Tank() { delSuit(); }
};

class Assassin:
    public Character
{
public:
    Assassin() { Character("刺客", 850, 100, 10, 500); }
    ~Assassin() { delSuit(); }
};

class Monster:
    public Character
{
private:
    map<string, vector<int>> monsters;

    void loadData()
    {
        fstream file("res/Monsters.csv");
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
                    monsters[t] = {};
                else
                    monsters[temp[0]].push_back(stoi(t));
            }
        }
        file.close();
    }

public:
    Monster(){};
    Monster(string name)
    {
        loadData();
        Character(name, monsters[name][0], monsters[name][1],
                        monsters[name][2], monsters[name][3]);
    }
    ~Monster() { delSuit(); }
};

Character *makeRole(int option, string name = "")
{
    switch (option)
    {
    case 0:
        if (!name.empty())
            return new Monster(name);
        else return NULL;
        break;
    case 1:
        return new Berserker();
        break;
    case 2:
        return new Tank();
        break;
    case 3:
        return new Assassin();
        break;
    default:
        return NULL;
        break;
    }
}
#endif
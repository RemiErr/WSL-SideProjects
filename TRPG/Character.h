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
    string Name;
    int Health;
    int ATK;
    int DEF;
    int Money;

    // 會以函式 new 物件出來，所以用指標確保能抓到該位址
    Weapon *weapon;
    Armor *armor;

public:
    Character(int health = 500, int atk = 0, int def = 0, int money = 0)
    {
        Health = health;
        ATK = atk;
        DEF = def;
        Money = money;
        weapon = new Weapon();
        armor = new Armor();
    }
    ~Character(){}

    void setName(string name);
    void setState(int health, int atk, int def);
    void setMoney();
    void setWeapon(Weapon *wep)
    {
        Money -= wep->getPrice();
        ATK += wep->getATK();
        weapon = wep;
    }
    void setArmor(Armor *arm)
    {
        Money -= arm->getPrice();
        DEF += arm->getDEF();
        armor = arm;
    }

    void onHit(double dmg)
    {
        dmg = DEF / (dmg + DEF);
        Health -= dmg;
    }

    string getName() { return Name; }
    vector<int> getState() { return { Health, ATK, DEF }; }    
    int getMoney() { return Money; }
    Weapon *getWeapon() { return weapon; }
    Armor *getArmor() { return armor; }

};



#endif
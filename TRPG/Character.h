#ifndef CHARACTER_H
#define CHARACTER_H

#include "Weapon.h"
#include "Armor.h"
#include <string>
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
};



#endif
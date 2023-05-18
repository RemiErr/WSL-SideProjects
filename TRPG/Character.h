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
    string Role;
    int Health_Max;
    int Health;
    int ATK;
    int DEF;
    int Money;

    // 會以函式 new 物件出來，所以用指標確保能抓到該位址
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
    Character(){}
    Character(string role, int health, int atk, int def, int money)
    {
        Role = role;
        Health_Max = health;
        Health = health;
        ATK = atk;
        DEF = def;
        Money = money;
        weapon = new Weapon();
        armor = new Armor();
    }
    ~Character() { delSuit(); }

    void setRole(string role);
    void setState(int health_max, int health, int atk, int def);
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
        Health_Max += arm->getHealth();
        DEF += arm->getDEF();
        armor = arm;
    }

    string getRole() { return Role; }
    vector<int> getState() { return { Health_Max, Health, ATK, DEF }; }    
    int getMoney() { return Money; }
    Weapon *getWeapon() { return weapon; }
    Armor *getArmor() { return armor; }

    void onHit(double dmg)
    {
        dmg = DEF / (dmg + DEF);
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

Character *makeRole(int option)
{
    switch (option)
    {
    case 1:
        return new Berserker();
        break;
    case 2:
        return new Tank();
        break;
    case 3:
        return new Assassin();
        break;
    }
}

// class RoleMaker
// {
// public:
//     // 利用指標，將建立的角色記憶體位址，存放在 CharacterMaker 裡
//     Character *makeCharacter(int option)
//     {
//         switch (option)
//         {
//         case 1:
//             return new Berserker();
//             break;
//         case 2:
//             return new Tank();
//             break;
//         case 3:
//             return new Assassin();
//             break;
//         }
//     }
// };
#endif
#ifndef CHARACTER_H
#define CHARACTER_H

#include "FileManager.h"
#include "Weapon.h"
#include "Armor.h"

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
    // void delSuit()
    // {
    //     if (weapon != nullptr)
    //         delete weapon;
    //     if (armor != nullptr)
    //         delete armor;
    // }

public:
    Character(string role = "", int health = 0, int atk = 0, int def = 0, int money = 0)
    {
        setRole(role);
        setState(health, health, atk, def);
        setMoney(money);
        weapon = new Weapon();
        armor = new Armor();
    }
    ~Character() { }//delSuit(); }

    // 設定職業
    void setRole(string role)
    {
        Role = role;
    }

    void setState(int health_max = -1, int health = -1, int atk = -1, int def = -1)
    {
        if (health_max != -1) Health_Max = health_max;
        if (health != -1) Health = health;
        if (atk != -1) ATK = atk;
        if (def != -1) DEF = def;
    }

    void setMoney(int money)
    {
        Money = money;
    }

    // 設定角色武器
    void setWeapon(Weapon *wep, bool m_flag = true)
    {
        if (m_flag) Money -= wep->getPrice();
        ATK += wep->getATK();
        weapon = wep;
    }

    // 設定角色防具
    void setArmor(Armor *arm, bool m_flag = true)
    {
        if (m_flag) Money -= arm->getPrice();
        Health_Max += arm->getHealth();
        DEF += arm->getDEF();
        armor = arm;
    }

    // 檢查是否死亡
    bool checkDeath()
    {
        if (this != nullptr && this->getState()[1]>0)
            return false;
        return true;
    }

    void onHit(int dmg)
    {
        dmg = int((dmg * dmg) / (dmg/2 + DEF) + 0.5); // 四捨五入
        Health -= dmg;
        if (Health < 0) Health = 0;
    }

    void isRecovery(int recovery)
    {
        Health += recovery;
        if (Health > Health_Max)
            Health = Health_Max;
    }

    string getRole() { return Role; }
    vector<int> getState() { return { Health_Max, Health, ATK, DEF }; }    
    int getMoney() { return Money; }
    Weapon *getWeapon() { return weapon; }
    Armor *getArmor() { return armor; }
};

// 各職業差異設定
class Berserker:
    public Character
{
public:
    Berserker()
    {
        Role = "狂戰士";
        Health_Max = 1000;
        Health = Health_Max;
        ATK = 50;
        DEF = 20;
        Money = 500;
        Character(Role, Health, ATK, DEF, Money);
    }
    ~Berserker() { }//delSuit(); }
};

class Tank:
    public Character
{
public:
    Tank()
    {
        Role = "坦克";
        Health_Max = 1600;
        Health = Health_Max;
        ATK = 15;
        DEF = 60;
        Money = 500;
        Character(Role, Health, ATK, DEF, Money);
    }
    ~Tank() { }//delSuit(); }
};

class Assassin:
    public Character
{
public:
    Assassin()
    {
        Role = "刺客";
        Health_Max = 850;
        Health = Health_Max;
        ATK = 100;
        DEF = 10;
        Money = 500;
        Character(Role, Health, ATK, DEF, Money);
    }
    ~Assassin() { }//delSuit(); }
};

class Monster:
    public Character
{
private:
    map<string, vector<int>> monsters;

    void loadData()
    {
        FileManager file("res/Monsters.csv");
        monsters = file.getData();
    }

public:
    Monster(){};
    Monster(string name)
    {
        loadData();
        Role = name;                    // 用 職業名 存放 怪物名
        Health_Max = monsters[name][0]; // 血量
        Health = Health_Max;
        ATK = monsters[name][1];        // 攻擊力
        DEF = monsters[name][2];        // 防禦力
        Money = monsters[name][3];      // 掉落金幣
        Character(name, Health, ATK, DEF, Money);
    }
    ~Monster() { }//delSuit(); }
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
        return new Character();
        break;
    }
}
#endif
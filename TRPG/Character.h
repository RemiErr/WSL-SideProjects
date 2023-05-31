#ifndef CHARACTER_H
#define CHARACTER_H

#include "FileManager.h"
#include "Weapon.h"
#include "Armor.h"

#ifndef STD_H
using namespace std;
#define cout std::cout
#define cin std::cin
#endif

// 角色數值標籤
enum { eMAX_HP, eHP, eATK, eDEF, eSPD, eMONEY };

// 基礎角色資料
class Character
{
protected:
    string Role_Name; // 職業名稱
    int Role_Type; // 職業名稱
    int Health_Max;
    int Health;
    int ATK;
    int DEF;
    int SPD;
    int Money;

    // 會 new 物件出來，用指標抓取該位址
    Weapon *weapon;
    Armor *armor;


public:
    Character(string role = "", int health = 0, int atk = 0, int def = 0, int spd = 0, int money = 0)
    {
        setRole(role);
        setState(health, health, atk, def, spd);
        setMoney(money);
        weapon = new Weapon();
        armor = new Armor();
    }
    ~Character(){}

    // 設定職業
    void setRole(string role)
    {
        Role_Name = role;
        if      (role == "狂戰士") Role_Type = 1;
        else if (role == "坦克")   Role_Type = 2;
        else if (role == "刺客")   Role_Type = 3;
        else Role_Type = 0;
    }

    void setState(int health_max = -1, int health = -1, int atk = -1, int def = -1, int spd = -1)
    {
        if (health_max != -1) Health_Max = health_max;
        if (health != -1) Health = health;
        if (atk != -1) ATK = atk;
        if (def != -1) DEF = def;
        if (spd != -1) SPD = spd;

        if (Health_Max < 0) Health_Max = 0;
        if (Health < 0) Health = 0;
    }

    void setMoney(int money)
    {
        Money = money;
        if (Money < 0) Money = 0;
    }

    // 設定角色武器
    void setWeapon(Weapon *wep, bool m_flag = true)
    {
        if (m_flag) Money -= wep->getPrice();
        // ATK += wep->getATK();
        weapon = wep;
    }

    // 設定角色防具
    void setArmor(Armor *arm, bool m_flag = true)
    {
        if (m_flag) Money -= arm->getPrice();
        // Health_Max += arm->getHealth();
        // DEF += arm->getDEF();
        armor = arm;
    }

    // 檢查是否死亡
    // bool checkDeath()
    // {
    //     if (this != nullptr && this->getState()[1]>0)
    //         return false;
    //     return true;
    // }

    int onHit(double dmg, bool def_flag = false)
    {
        dmg -= DEF / (10 * dmg + DEF) + (def_flag? DEF : 0) + 0.5; // 四捨五入
        if (dmg <= 0) dmg = 1;

        Health -= dmg;
        if (Health < 0) Health = 0;

        return dmg;
    }

    void isRecovery(int recovery)
    {
        Health += recovery;
        if (Health > Health_Max + armor->getHealth())
            Health = Health_Max + armor->getHealth();
        if (Health < 0) Health = 0;
    }

    // 職業編號
    int getRoleType() { return Role_Type; }
    // 職業(怪物) 名稱
    string getRoleName() { return Role_Name; }
    // 最大血量 當前血量 攻擊力 防禦力 速度
    vector<int> getState()
    {
        return {
            Health_Max + armor->getHealth(),
            Health,
            ATK + weapon->getATK(),
            DEF + armor->getDEF(),
            SPD
        };
    }
    // 給 我 錢
    int getMoney() { return Money; }
    // 取得武器物件
    Weapon *getWeapon() { return weapon; }
    // 取得防具物件
    Armor *getArmor() { return armor; }
};

// 各職業差異設定
class Berserker:
    public Character
{
public:
    Berserker()
    {
        setRole("狂戰士");
        Health_Max = 1000;
        Health = Health_Max;
        ATK = 50;
        DEF = 20;
        SPD = 35;
        Money = 500;
        Character(Role_Name, Health, ATK, DEF, SPD, Money);
    }
    ~Berserker(){}
};

class Tank:
    public Character
{
public:
    Tank()
    {
        setRole("坦克");
        Health_Max = 1600;
        Health = Health_Max;
        ATK = 15;
        DEF = 60;
        SPD = 20;
        Money = 500;
        Character(Role_Name, Health, ATK, DEF, SPD, Money);
    }
    ~Tank(){}
};

class Assassin:
    public Character
{
public:
    Assassin()
    {
        setRole("刺客");
        Health_Max = 850;
        Health = Health_Max;
        ATK = 100;
        DEF = 10;
        SPD = 50;
        Money = 500;
        Character(Role_Name, Health, ATK, DEF, SPD, Money);
    }
    ~Assassin(){}
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
    Monster(){ loadData(); };
    Monster(string name)
    {
        loadData();
        // 職業名 & 怪物名
        setRole(name);
        // 最大血量
        Health_Max = monsters[name][0];
        // 當前血量
        Health = Health_Max;
        // 攻擊力
        ATK = monsters[name][1];
        // 防禦力
        DEF = monsters[name][2];
        // 速度
        SPD = monsters[name][3];
        // 掉落金幣
        Money = monsters[name][4];
        Character(name, Health, ATK, DEF, Money);
    }
    ~Monster(){}

    map<int, string> getNameDict()
    {
        map<int, string> dict;
        vector<string> names;
        for (auto mon: monsters)
            names.push_back( mon.first );

        for (int i=1; i <= names.size(); i++)
        {
            dict[i] = names[i-1];
        }

        return dict;
    }
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
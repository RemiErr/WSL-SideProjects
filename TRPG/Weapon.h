#ifndef WEAPON_H
#define WEAPON_H
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
    BerserkerWeapon(string name, int atk, int price)
    { Weapon(name, atk, price); }

    ~BerserkerWeapon(){}
};

class TankWeapon:
    public Weapon
{
public:
    TankWeapon(string name, int atk, int price)
    { Weapon(name, atk, price); }

    ~TankWeapon(){}
};

class AssassinWeapon:
    public Weapon
{
public:
    AssassinWeapon(string name, int atk, int price)
    { Weapon(name, atk, price); }

    ~AssassinWeapon(){}
};

class WeaponShop:
    public Weapon
{
public:
    WeaponShop(){}
    ~WeaponShop(){}

    Weapon *makeWeapon(int choose);
    Weapon *makeWeapon(int mob, int choose);
};
#endif
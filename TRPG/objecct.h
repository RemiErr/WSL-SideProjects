#include "Event.h"

/* 建立物件 */
WeaponManager wep_shop("res/WeaponsShop.csv");
WeaponManager wep_drop;

ArmorManager arm_shop("res/ArmorsShop.csv");
ArmorManager arm_drop;


/* 資料清單 */
map<int, string> mon_list = Monster().getNameDict();

map<string, vector<int>> wep_shop_list = wep_shop.getList();
vector<string> wep_drop_names = [](){
    vector<string> temp;
    for (auto wep: wep_drop.getList())
        temp.push_back( wep.first );
    return temp;
}();

map<string, vector<int>> arm_shop_list = arm_shop.getList();
vector<string> arm_drop_names = [](){
    vector<string> temp;
    for (auto arm: arm_drop.getList())
        temp.push_back( arm.first );
    return temp;
}();
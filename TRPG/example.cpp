#include "Character.h"
#include "Weapon.h"
#include "Armor.h"

int main ()
{
    // 若系統為 Windows ，自動下 CMD 編碼轉換指令
    #ifdef _WIN32
    system("chcp 65001 && cls");
    #endif

    // 會透過 makeRole 存取角色物件，指標指向該物件
    Character *player;
    Character *monster;

    // 用來分配裝備
    WeaponShop wep_shop;
    ArmorShop arm_shop;


    int opt = 0;
    cout << "選擇職業 (1.狂戰 2.坦克 3.刺客): ";
    cin >> opt;
    player = makeRole(opt!=0? opt : 1); // 這邊用三元運算子，處理使用者輸入 0 (生成怪物) 的情況

    player->setWeapon( wep_shop.makeWeapon(opt, "銳利刺匕"), false ); // 裝備物件, 是否扣錢
    player->setArmor( arm_shop.makeArmor(opt, "鐵製背心"), false );

    // 生成怪物
    monster = makeRole(0, "史萊姆");


    // 角色名稱 (職業)，存起來方便後面寫程式用
    string ply_role = player->getRole();
    string mon_name = monster->getRole(); 
    
    // 角色狀態 (最大血量 當前血量 攻擊力 防禦力)
    vector<int> ply_state = player->getState();
    vector<int> mon_state = monster->getState(); 
    int ply_atk = ply_state[2];
    int mon_atk = mon_state[2];

    // 輸出
    // 這邊記得注意是否有物件的數值更新，但上方變數忘記更新的
    char temp;
    cout << "[玩家狀態]\n";
    cout << "血量: " << ply_state[1] << " / " << ply_state[0] << endl;
    cout << "攻擊力: " << ply_state[2] << " 防禦力: " << ply_state[3] << endl;
    cout << "金幣: " << player->getMoney() << "\n====================" << endl;
    cout << "職業: " << player->getRole()
         << "\n武器: " << player->getWeapon()->getName()
         << "\n防具: " << player->getArmor()->getName() << endl;

    cout << "\n====================\n遭遇" << mon_name 
         << ", 怪物血量: " << mon_state[1] << " / " << mon_state[0] << endl;

    cout << "是否戰鬥? (y/n): ";
    cin >> temp;

    if (temp == 'y' || temp == 'Y')
    {
        player->onHit( mon_atk );
        monster->onHit( ply_atk );
        cout << "玩家血量: " << player->getState()[1] << " / " << ply_state[0] << endl;
        cout << mon_name << "血量: " << monster->getState()[1] << " / " << mon_state[0] << endl;
    } else {
        cout << "逃跑成功\n";
    }
}
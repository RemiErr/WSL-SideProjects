#include "Event.h"

#ifndef STD_H
using namespace std;
#define cout std::cout
#define cin std::cin
#endif


/* 建立物件 */
WeaponShop wep_shop;
ArmorShop arm_shop;

Character *p = makeRole(-1);
Character *m = makeRole(-1);

Event e;

/* 資料清單 */
map<int, string> mon_list
{
    {1, "史萊姆"},
    {2, "哥布林"},
    {3, "寶箱怪"}
};
map<string, vector<int>> wep_list = wep_shop.getList();
vector<string> wep_name_list = [](){
    vector<string> temp;
    for (auto wep: wep_list)
        temp.push_back( wep.first );
    return temp;
}();

map<string, vector<int>> arm_list = arm_shop.getList();
vector<string> arm_name_list = [](){
    vector<string> temp;
    for (auto arm: arm_list)
        temp.push_back( arm.first );
    return temp;
}();

/* 事件前置宣告 */
void runEvnG1();
void runEvnG2();
void runEvnG3();
void runEvnG4();
void runEvnS1();
void runEvnS2();
void runEvnS3();
void runEvnS4();
void runEvnR1();
void runEvnR1();
void runEvnR2();

void showPlayerState()
{
    cout << "\n==========< state >==========\n"
         << "血量: "   << p->getState()[1] << " / " << p->getState()[0] << endl
         << "攻擊力: " << p->getState()[2] << " 防禦力: " << p->getState()[3] << endl
         << "金幣: "   << p->getMoney()
         << "\n==========<---+--->=========="
         << "\n職業: "   << p->getRoleName()
         << "\n武器: " << p->getWeapon()->getName()
         << "\n防具: " << p->getArmor()->getName()
         << "\n==========<---o--->==========\n";
}


/* 程式主體 */
int main()
{
    CLS_M

    int role_opt = 0;
    e.displayText("請選擇職業:\n(1) 狂戰士\n(2) 坦克\n(3) 刺客\n\n");
    cin >> role_opt;

    p = makeRole(role_opt);

    e.displayText("你選擇了" + p->getRoleName(), 3);

    // 裝備物件, 是否扣錢
    p->setWeapon( wep_shop.makeWeapon(p->getRoleType(), "銳利刺匕"), false );
    p->setArmor( arm_shop.makeArmor(p->getRoleType(), "鐵製背心"), false );

    e.upPtr(p, m);



    e.showLoadingAnimation(30);
    sleep(20);
    cin.clear();
    cin.ignore(INT_MAX,'\n');
    CLS_M STOP_M
    while (e.Desert_alive() && e.Desert_menu())
    {
        e.upPtr(p, m);
        // runEvnG3();
        switch(e.whichEvent())
        {
            case G_1:
                runEvnG1();
                break;
            case G_2:
                runEvnG2();
                break;
            case G_3:
                runEvnG3();
                break;
            case G_4:
            case R_1:
                p->isRecovery(p->getState()[0] / 2);
                e.displayText( RE, 1, 3 );
                e.endEvent();
                break;
            case R_2:
                runEvnR2();
                break;
        }
        e.endEvent();
        sleep(2500);
    }
}



/* 事件包裝 */
void runEvnG1()
{
    int health = p->getState()[1];
    if (health > 0) p->setState(-1, (health / 10) * 8);
    // e.displayText("休息片刻後，仍然覺得疲勞的你，查看了自己的狀態。\n", 3);
    showPlayerState();
    e.endEvent();
    sleep(1000);
}


void runEvnG2()
{
    char opt;
    cin >> opt;

    if (opt == 'y' || opt == 'Y')
    {
        // 共有三種事件
        int rd = e.random(1, 10);

        if (rd < 7 && rd&1) // 1 3 5
        {
            m = makeRole(0, mon_list[e.random(1, 3)]);
            e.displayText("從冒險者的遺物中冒出了一個黑影，那是....\n");
            sleep(500);

            // 戰鬥
            do {
                CLS_M
                cout << "\n==========<⊙>==========\n"
                     << "怪物: " << m->getRoleName()
                     << ", 怪物血量: " << m->getState()[1] << " / " << m->getState()[0]
                     <<"\n==========<—>==========\n";
                showPlayerState();

                sleep(500);
                cout << "玩家攻擊怪物，造成 "
                     << m->onHit( p->getState()[2] )
                     << " 點傷害\n";
                cout << "\r玩家受到攻擊，總共 "
                     << p->onHit( m->getState()[2] )
                     << " 點傷害  \n";
                sleep(1000);
            } while(m->getState()[1] > 0);

        }
        else if (rd < 7 && !(rd&1)) // 2 4 6
        {
            char opt;
            string arm_name = arm_name_list[ e.random(0, arm_name_list.size()) ];
            Armor *arm = arm_shop.makeArmor(p->getRoleType(), arm_name);

            cout << "你從冒險者的遺物中，找到了 <" << arm->getName() << ">\n\n是否要更換裝備？ (y/n)\n";
            cin >> opt;

            CLS_M
            if (opt == 'y' || opt == 'Y')
            {
                p->setArmor(arm, false);
                cout << "你換上了 " << p->getArmor()->getName() << endl;
                showPlayerState();
                sleep(1000);
            } else {
                cout << "你看不上那把武器，就這樣離開了\n";
            }
        } else  cout << "什麼也沒找到\n";

    } else      cout << "你不想打擾死者的安寧，就這麼離開了\n";
}

void runEvnG3()
{
    m = makeRole(0, "史萊姆");

    char temp;
    while(e.isFight())
    {
        cout << "\n==========<⊙>==========\n遭遇" << m->getRoleName()
        << ", 怪物血量: " << m->getState()[1] << " / " << m->getState()[0];
        cout<<"\n==========<—>==========\n";
        cout << "是否戰鬥? (y/n): ";
        cin >> temp;

        if (temp == 'y' || temp == 'Y')
        {
            while(m->getState()[1] > 0)
            {
                CLS_M
                p->onHit( m->getState()[2] );
                m->onHit( p->getState()[2] );
                cout << "\n====================\n玩家血量: " << p->getState()[1] << " / " << p->getState()[0] << endl;
                cout << m->getRoleName() << "血量: " << m->getState()[1] << " / " << m->getState()[0] << endl;
                sleep(1000);
            }
            cout<<"\n====================\n玩家戰勝了"<<m->getRoleName()<<"\n====================\n";
        } else {
            cout<<"\n==========<⊙>==========\n";
            e.displayText("\t逃跑成功");
            cout<<"\n==========<—>==========\n";
        }
        e.endEvent();
        sleep(1550);
    }
}

// void runEvnG4();
// void runEvnS1();
// void runEvnS2();
// void runEvnS3();
// void runEvnS4();
// void runEvnR1()
// {

// }
void runEvnR2()
{
    // 隨機損失當前 10 ~ 30 % 金錢
    int temp = p->getMoney();
    int lost = e.random(temp / 10, (temp / 10) * 3);
    p->setMoney( temp - lost );
    e.displayText( RE, 2);
    cout<<"你付了 $"<<lost<<" 給旅人，他心滿意足的離開了\n";

    if (e.random(1, 5) > 3)
    {
        char opt;
        string wep_name = wep_name_list[ e.random(0, wep_name_list.size()) ];
        Weapon *wep = wep_shop.makeWeapon( p->getRoleType(), wep_name );
        cout << "旅人在離開前，留下了 <" << wep->getName() << ">\n\n是否要更換裝備？\n";
        cin >> opt;
        CLS_M
        if (opt == 'y' || opt == 'Y')
        {
            p->setWeapon(wep, false);
            cout << "你換上了 " << p->getWeapon() << endl;
            showPlayerState();
        } else {
            cout << "你留下了那把武器，離開了\n";
        }
    }
    e.endEvent();
    sleep(2000);
}

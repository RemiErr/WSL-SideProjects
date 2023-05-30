#include "Event.h"

#ifndef STD_H
using namespace std;
#define cout std::cout
#define cin std::cin
#endif


/* 建立物件 */
WeaponManager wep_shop("res/WeaponsShop.csv");
WeaponManager wep_drop;

ArmorManager arm_shop("res/ArmorsShop.csv");
ArmorManager arm_drop;

Character *p = makeRole(-1);
Character *m = makeRole(-1);

Event e;

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

/* 事件前置宣告 */
void runEvnG2();
void runEvnG3();
void runEvnS2();
void runEvnS4();
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
void showAttack()
{
    // 戰鬥
    do {
        int m_onhit = m->onHit( p->getState()[2] );
        int p_onhit = p->onHit( m->getState()[2] );

        CLS_M
        cout << "==========< ⊙ >==========\n"
             << "怪物: " << m->getRoleName()
             << ", 怪物血量: " << m->getState()[1] << " / " << m->getState()[0]
             << "\n==========< - >==========\n";
        showPlayerState();
        sleep(100);

        cout << "玩家攻擊怪物，造成 " << m_onhit << " 點傷害\n";
        cout << "玩家受到攻擊，總共 " << p_onhit << " 點傷害\n";
        sleep(600);
    } while(m->getState()[1] > 0 && p->getState()[1] > 0);

    if (p->getState()[1] > 0)
    {
        p->setMoney(p->getMoney() + m->getMoney());

        cout << "\n==========< - >==========\n"
             << "玩家戰勝了" << m->getRoleName()
             << "\n獲得 $" << m->getMoney()
             << "\n目前共有 $" << p->getMoney()
             << "\n==========<+##+>==========\n";
    } else {
        p->setMoney(p->getMoney() - m->getMoney());

        cout << "\n==========< - >==========\n"
             << "玩家戰敗了" << m->getRoleName()
             << "\n失去 $" << m->getMoney()
             << "\n目前剩下 $" << p->getMoney()
             << "\n==========<+##+>==========\n";
    }
    sleep(1000);
}


/* 程式主體 */
bool f_Game = true;
int main()
{
    #ifdef _WIN32
    system("chcp 65001");
    #endif

    CLS_M
    int debuff = 0;
    int role_opt = 0;

    e.displayText("請選擇職業:\n(1) 狂戰士\n(2) 坦克\n(3) 刺客\n\n");
    cin >> role_opt;
    e.reChoose(role_opt);
    p = makeRole(role_opt);

    e.displayText("你選擇了" + p->getRoleName() + "\n", 3);

    // 裝備物件, 是否扣錢
    p->setWeapon( wep_drop.makeWeapon(p->getRoleType(), "銳利刺匕"), false );
    p->setArmor( arm_drop.makeArmor(p->getRoleType(), "鐵製背心"), false );
    e.upPtr(p);


    e.showLoadingAnimation(30);
    sleep(20);
    cin.clear();
    cin.ignore(INT_MAX,'\n');
    CLS_M STOP_M

    /* 測試角色死亡事件 */
    // p->setState(-1,0);
    // p->setMoney(0);
    // e.Desert_alive();

    // 設定例外事件，跳過該回合
    e.setExcept( {R_1, R_2} );
    while (f_Game && e.Desert_menu())
    {
        e.upPtr(p, m);
        switch(e.whichEvent())
        {
            case G_1:
                debuff = -3;
                break;
            case G_2:
                runEvnG2();
                break;
            case G_3:
                runEvnG3();
                break;
            case S_2:
                runEvnS2();
                break;
            case S_4:
                sleep(1500);
                runEvnS4();
                break;
            case R_1:
                if (debuff >= 0)
                {
                    // 依最大血量 補60%血量
                    int health = p->getState()[1];
                    p->isRecovery( (health / 10) * 6 );
                    e.displayText( RE, 1, 3 );
                } else {
                    // 依 debuff 狀態 扣當前血量
                    int health = p->getState()[0];
                    p->isRecovery( (health / 10) * debuff );
                    debuff = 0;
                    showPlayerState();
                    sleep(1000);
                }
                break;
            case R_2:
                runEvnR2();
                break;
        }
        f_Game = e.Desert_alive();
        e.endEvent();
        sleep(1500);
    }
}



/* 事件包裝 */
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
            showAttack();
        }
        else if (rd < 7 && !(rd&1)) // 2 4 6
        {
            char opt;
            int chance = e.random(1, 10);

            string arm_name = arm_drop_names[ e.random(0, arm_drop_names.size()-1) ];
            string wep_name = wep_drop_names[ e.random(0, wep_drop_names.size()-1) ];

            Armor  *arm = arm_drop.makeArmor(p->getRoleType(), arm_name);
            Weapon *wep = wep_drop.makeWeapon(p->getRoleType(), wep_name);
            

            cout << "你從冒險者的遺物中，找到了 <";
            if (chance&1) cout << arm->getName();
            else          cout << wep->getName();
            cout << ">\n\n是否要更換裝備？ (y/n)\n";
            cin >> opt;

            CLS_M
            if (opt == 'y' || opt == 'Y')
            {
                cout << "你換上了 ";
                if (chance&1)
                {
                    p->setArmor(arm, false);
                    cout << p->getArmor()->getName() << endl;
                    delete wep;
                } else {
                    p->setWeapon(wep, false);
                    cout << p->getWeapon()->getName() << endl;
                    delete arm;
                }
                showPlayerState();
                sleep(1000);
            } else {
                cout << "你看不上那件裝備，就這樣離開了\n";
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
        cout << "\n==========< ⊙ >=========="
             << "\n遭遇 [" << m->getRoleName() << "]"
             << ", 怪物血量: " << m->getState()[1] << " / " << m->getState()[0]
             << "\n==========< - >==========\n";
        cout << "是否戰鬥? (y/n): ";
        cin  >> temp;

        if (temp == 'y' || temp == 'Y')
        {
            showAttack();
        } else {
            cout<<"\n==========< ⊕ >==========\n";
            e.displayText("\t逃跑成功");
            cout<<"\n==========<-##->==========\n";
        }
        e.endEvent();
        sleep(1550);
    }
}

void runEvnS2()
{
    map<int, string> items; // 商品清單
    int index = 1;
    int split[2] = {0};
    int num = 0;
    char opt;
    cin >> opt;
    if (opt == 'y' || opt == 'Y')
    {
        bool f_buy = false;
        do{
            CLS_M
            cout << "==========<  $  >==========\n"
                << "\t[ 武 器 ]\n";
                for (auto wep: wep_shop_list)
                {
                    split[0] = index;
                    items[index++] = wep.first;
                    cout << split[0] << ". " << wep.first << "\t:\t";
                    for (auto &val: wep.second)
                        cout << val << ((&val != &wep.second[wep.second.size()-1])? ", ":"\n");
                }
            cout << "==========<--+-->==========\n"
                << "\t[ 防 具 ]\n";
                for (auto arm: arm_shop_list)
                {
                    split[1] = index;
                    items[index++] = arm.first;
                    cout << split[1] << ". "  << arm.first << "\t:\t";
                    for (auto &val: arm.second)
                        cout << val << ((&val != &arm.second[arm.second.size()-1])? ", ":"\n");
                }
            cout << "==========<  $  >==========\n";
            cout << "請輸入武器編號: ";
            cin >> num;

            // 購買武器
            if (0 < num && num <= split[0])
            {
                Weapon *wep = wep_shop.makeWeapon( p->getRoleType(), items[num] );

                if (p->getMoney() >= wep->getPrice())
                {
                    f_buy = true;
                    p->setWeapon( wep );
                } else {
                    f_buy = false;
                    cout << "\n\t[ 餘額不足 請充值 ]\n";
                }
            }
            // 購買防具
            else if (split[0] < num && num <= split[1])
            {
                Armor *arm = arm_shop.makeArmor( p->getRoleType(), items[num] );

                if (p->getMoney() >= arm->getPrice())
                {
                    f_buy = true;
                    p->setArmor( arm );
                } else {
                    f_buy = false;
                    cout << "\n\t[ 餘額不足 請充值 ]\n";
                }
            }
            // 錯誤編號
            else cout << "\n\t[ 請輸入正確編號 ]\n";

            CLS_M
            cout << "\t[ 目前狀態 ]\n";
            showPlayerState();
            sleep(1000);
        } while (!f_buy);
    }
}

void runEvnS4()
{
    m = makeRole(0, "哥布林");
    string wep_name = wep_drop_names[ e.random(0, wep_drop_names.size()-1) ];
    Weapon *m_wep = new MonsterWeapon(wep_name, e.random(0, 2)); // 回傳特別武器

    showAttack();
    CLS_M
    cout << "你撿起了哥布林死前握在手上的武器 " << m_wep->getName() << "\n";
    p->setWeapon( m_wep, false );
    showPlayerState();
    sleep(1500);
}

void runEvnR2()
{
    // 隨機損失當前 10 ~ 30 % 金錢
    int temp = p->getMoney();
    if (temp <= 0)
    {
        e.displayText("臨死前的你，不斷地責怪疏忽大意的自己\n", 100);
        sleep(800);
        e.displayText("而你的意識也越來越模糊，直到...\n......\r.........\n", 150);
        sleep(900);
        CLS_M
        e.displayText("\t「 好像... ... ...\n\t\t有點...\n\t\t......", 200);
        sleep(1000);
        e.displayText("\r\t\t\t.........\n\n\t\t\t冷...... 」\n", 300);
        sleep(1500);
        f_Game = false;
        return;
    }

    int lost = e.random(temp / 10, (temp / 10) * 3);
    p->setMoney( temp - lost );
    e.displayText( RE, 2);
    cout<<"你付了 $"<<lost<<" 給旅人，他心滿意足的離開了\n";

    if (e.random(1, 5) > 3)
    {
        char opt;
        string wep_name = wep_drop_names[ e.random(0, wep_drop_names.size()-1) ];
        Weapon *wep = wep_drop.makeWeapon( p->getRoleType(), wep_name );
        cout << "旅人在離開前，留下了 <" << wep->getName() << ">\n\n是否要更換裝備？ (y/n)\n";
        cin >> opt;
        CLS_M
        if (opt == 'y' || opt == 'Y')
        {
            p->setWeapon(wep, false);
            cout << "你換上了 " << p->getWeapon()->getName() << endl;
            showPlayerState();
        } else {
            cout << "你留下了那把武器，離開了\n";
        }
    }
    sleep(1500);
}

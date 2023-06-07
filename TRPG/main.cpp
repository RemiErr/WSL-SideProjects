#include "process.h"
#ifndef STD_H
using namespace std;
#define cout std::cout
#define cin std::cin
#endif

/* 事件前置宣告 */
void runEvnG2();
void runEvnG3();
void runEvnS2();
void runEvnS4();
void runEvnR2();


/* 程式主體 */
int main()
{
    #ifdef _WIN32
    system("chcp 65001");
    #endif

    CLS_M
    int debuff = 0;
    starGame();
    creatCharacter();

    // 裝備物件, 是否扣錢
    p->setWeapon( wep_drop.makeWeapon(p->getRoleType(), "銳利刺匕"), false );
    p->setArmor( arm_drop.makeArmor(p->getRoleType(), "鐵製背心"), false );
    p->isRecovery( p->getArmor()->getHealth() );

    addPoint();

    if (f_Game) showLoadingAnimation(25);
    sleep(20);
    STOP_M
    CLS_M
    displayText(FOR, 1); // 開場白
    sleep(3000);


    // 設定例外事件，跳過該回合
    setExcept( {R_1, R_2} );
    while (f_Game && Desert_menu())
    {
        upPtr(p, m);
        switch(whichEvent())
        {
            // 練習
            case EX:
            {
                int opt;
                string mon_str[3] = {"練習用稻草人", "史萊姆王", "歷戰 獨眼巨人"};
                cout << "(0) 練習 (1) 普通王 (2) 歷戰王\n請選擇難度: ";
                cin >> opt;
                reChoose(opt, 0, 2);
                m = makeRole(0, mon_str[opt]);
                showAttack();
                break;
            }

            // 加點
            case PT:
                CLS_M
                addPoint();
                break;

            // 轉職
            case CHG:
                CLS_M
                changeCharacter(p->getWeapon(), p->getArmor());
                break;

            // 前進事件
            case G_1:
                debuff = -3;
                isLeave(true);
                break;
            case G_2:
                runEvnG2();
                break;
            case G_3:
                sleep(1000);
                runEvnG3();
                break;
            case G_4:
                isLeave(true);
                break;

            // 停在原地
            case S_2:
                runEvnS2();
                break;
            case S_4:
                sleep(1500);
                runEvnS4();
                break;

            // 倒下後，下一回合觸發
            case R_1:
                if (debuff >= 0)
                {
                    // 依最大血量 補20%血量
                    int health = p->getState()[eMAX_HP];
                    p->isRecovery( (health / 10) * 2 );
                    displayText( RE, 1, 3 );
                } else {
                    // 依 debuff 狀態 扣當前血量
                    int health = p->getState()[eHP];
                    p->isRecovery( (health / 10) * debuff );
                    debuff = 0;
                    if (p->getState()[eHP] > 0)
                        showPlayerState();
                    else endGame();
                    sleep(1000);
                }
                endEvent();
                break;
            case R_2:
                CLS_M
                runEvnR2();
                endEvent();
                break;
        }
        f_Game = Desert_alive();
        endEvent();
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
        int rd = random(1, 10);

        if (rd < 7 && rd&1) // 1 3 5
        {
            m = makeRole(0, mon_list[random(1, mon_list.size()-1)]);
            displayText("從冒險者的遺物中冒出了一個黑影，那是....\n");
            sleep(500);

            // 戰鬥
            showAttack();
        }
        else if (rd < 7 && !(rd&1)) // 2 4 6
        {
            char opt;
            int chance = random(1, 10);

            string arm_name = arm_drop_names[ random(0, arm_drop_names.size()-1) ];
            string wep_name = wep_drop_names[ random(0, wep_drop_names.size()-1) ];

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
    // m = makeRole(0, "歷戰 獨眼巨人");
    // 20% 機率出現史萊姆王
    m = makeRole(0, random(0, 100)<=20? "史萊姆王" : "史萊姆");


    cout << "\n==========< ⊙ >=========="
         << "\n遭遇 [" << m->getRoleName() << "]"
         << "\n==========< - >==========\n";
    showAttack();
    sleep(1550);
}

void runEvnS2()
{
    map<int, string> items; // 商品清單
    int num = 0;
    char opt;
    cin >> opt;
    if (opt == 'y' || opt == 'Y')
    {
        do{
            int index = 1;
            int split[2] = {0};
            CLS_M
            cout << "==========<   $   >==========\n"
                 << "\t[ 武 器 ]\n"
                 << "\t       ATK, Price\n";
                for (auto wep: wep_shop_list)
                {
                    split[0] = index;
                    items[index++] = wep.first;
                    cout << split[0] << ". " << wep.first << " :  "
                         << wep.second[0] << ", $" << wep.second[1] << "\n";
                }
            cout << "==========<---+--->==========\n"
                 << "\t[ 防 具 ]\n"
                 << "\t       HP, DEF, Price\n";
                for (auto arm: arm_shop_list)
                {
                    split[1] = index;
                    items[index++] = arm.first;
                    cout << split[1] << ". "  << arm.first << " :  "
                         << arm.second[0] << ", " << arm.second[1] << ", $" << arm.second[2] << "\n";
                }
            cout << "==========<   $   >==========\n"
                 << "剩餘金錢: $" << p->getMoney() << "\n"
                 << "(0) 離開商店\n\n請輸入武器編號: ";
            cin >> num;
            reChoose(num, 0, split[1]);

            if (num == 0) break;

            // 購買武器
            if (0 < num && num <= split[0])
            {
                Weapon *wep = wep_shop.makeWeapon( p->getRoleType(), items[num] );

                if (p->getMoney() < wep->getPrice())
                {
                    cout << "\n\t[ 餘額不足 請充值 ]\n";
                    STOP_M
                    break;
                }

                p->setWeapon(wep);
            }
            // 購買防具
            else if (split[0] < num && num <= split[1])
            {
                Armor *arm = arm_shop.makeArmor( p->getRoleType(), items[num] );

                if (p->getMoney() < arm->getPrice())
                {
                    cout << "\n\t[ 餘額不足 請充值 ]\n";
                    STOP_M
                    break;
                }

                p->setArmor(arm);
            }
            // 錯誤編號
            else cout << "\n\t[ 請輸入正確編號 ]\n";

            CLS_M
            cout << "\t[ 目前狀態 ]\n";
            showPlayerState();
            STOP_M
        } while (p->getMoney() >= 0);
    }
}
/* #20230606 S4 - 特別武器事件更新 */
void runEvnS4()
{
    string wep_name = wep_drop_names[ random(0, wep_drop_names.size()-1) ];
    Weapon *m_wep = new MonsterWeapon(wep_name, random(0, 2)); // 回傳特別武器
    m = makeRole(0, "哥布林");
    m->setRole("滿身疤痕的哥布林"); // 設定特殊名字
    m->setWeapon(m_wep);
    char opt;

    showAttack();
    CLS_M
    if (m->getState()[eHP] <= 0)
    {
        displayText("哥布林倒下後，你發現牠手上的武器，似乎有點不同\n");
        sleep(600);
        cout << "\n==========<  Wep  >=========="
             << "\n武器: " << m_wep->getName()
             << "\n攻擊力: " << m_wep->getATK()
             << "\n==========<---O--->==========\n";
        sleep(300);
        cout << "是否要更換裝備？ (y/n)\n";
        cin >> opt;
        if (opt == 'y' || opt == 'Y')
        {
            CLS_M
            displayText("你撿起了哥布林死前握在手上的武器 " + m_wep->getName() + "\n\n");
            p->setWeapon( m_wep, false );
            sleep(200);
            showPlayerState();
            STOP_M
        } else {
            cout << "那把武器看上去有點不祥，還是盡早離開這裡吧...\n";
            sleep(1000);
        }
    }
}

void runEvnR2()
{
    // 隨機損失當前 10 ~ 30 % + 200 金錢
    int p_money = p->getMoney();
    int lost = random(p_money / 10, (p_money / 10) * 3) + 200;
    if (p_money - lost > 0)
    {
        p->setMoney( p_money - lost );
    } else {
        endGame();
        return;
    }

    // 依最大血量 補80%血量
    int health = p->getState()[eMAX_HP];
    p->isRecovery( (health / 10) * 8 );

    displayText( RE, 2);
    cout<<"你付了 $"<<lost<<" 給旅人，他心滿意足的離開了\n";

    if (random(1, 5) > 3)
    {
        char opt;
        string arm_name = arm_drop_names[ random(0, arm_drop_names.size()-1) ];
        Armor *arm = arm_drop.makeArmor( p->getRoleType(), arm_name );
        cout << "旅人在離開前，留下了 <" << arm->getName() << ">\n\n是否要更換裝備？ (y/n)\n";
        cin >> opt;
        CLS_M
        if (opt == 'y' || opt == 'Y')
        {
            p->setArmor(arm, false);
            cout << "你換上了 " << p->getArmor()->getName() << endl;
            showPlayerState();
        } else {
            cout << "你留下了那件防具，離開了\n";
        }
    }
    sleep(1500);
}
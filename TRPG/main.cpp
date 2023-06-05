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


/* 動作函式 */
bool f_Game = true;
void showPlayerState()
{
    cout << "==========<  ※  >=========="
         << "\n血量: "   << p->getState()[eHP] << " / " << p->getState()[eMAX_HP]
         << "\n攻擊力: " << p->getState()[eATK] << " 防禦力: " << p->getState()[eDEF]
         << "\n速度: " << p->getState()[eSPD] << " 金幣: "   << p->getMoney()
         << "\n==========<--+-->=========="
         << "\n職業: "   << p->getRoleName()
         << "\n武器: " << p->getWeapon()->getName()
         << "\n防具: " << p->getArmor()->getName()
         << "\n==========<--o-->==========\n";
}
void showMonsterState()
{
    cout << "==========<  ⊙  >=========="
         << "\n怪物: " << m->getRoleName()
         << "\n血量: " << m->getState()[eHP] << " / " << m->getState()[eMAX_HP]
         << "\n攻擊力: " << m->getState()[eATK] << " 防禦力: " << m->getState()[eDEF]
         << "\n速度: " << m->getState()[eSPD]
         << "\n==========<--o-->==========\n";
}
void creatCharacter()
{   // 建立角色資料
    int role_opt  = 0;
    int role_flag = 0;
    Character *ex;
    do {
        CLS_M
        displayText("請選擇職業:\n\n(0) 職業數值\n(1) 狂戰士\n(2) 坦克\n(3) 刺客\n\n");
        cin >> role_opt;
        reChoose(role_opt, 0, 3);

        // 展示數值
        switch(role_opt)
        {
            case 0:
                CLS_M
                cout << "==========<  ※  >==========";
                for (int i=1; i <= 3; i++)
                {
                    ex = makeRole(i);
                    cout << "\n職業: "   << ex->getRoleName()
                        << "\n血量: "   << ex->getState()[eHP] << " / " << ex->getState()[eMAX_HP]
                        << "\n攻擊力: " << ex->getState()[eATK]
                        << "\n防禦力: " << ex->getState()[eDEF]
                        << "\n速度: " << ex->getState()[eSPD]
                        << "\n初始金幣: "   << ex->getMoney();
                    if (i < 3) cout << "\n==========<--+-->==========";
                    delete ex;
                }
                cout << "\n==========<--o-->==========\n";
                STOP_M
                break;
            case 1:
            case 2:
            case 3:
                ex = makeRole(role_opt);
                cout << "你選擇的職業是： "
                     << ex->getRoleName() << endl
                     << "確定要使用這個角色嗎？\n(0) 取消 (1) 確定\n";
                cin  >> role_flag;
                reChoose(role_flag, 0, 1);
                break;
        }
    } while ( !role_flag );
    p = ex;
    upPtr(p);
    CLS_M
    displayText("你選擇了" + p->getRoleName() + "\n", 3);
    sleep(500);
}
/* #20230605 轉職及加點 */
void changeCharacter(Weapon *wep, Armor *arm)
{
    // 取得之前總點數
    vector<int> recPt = p->getPointHistory();
    int allPoint = [&](){
        int sum = p->getPoint();
        for(auto pt: recPt)
            sum+=pt;
        return sum;
    }();
    // 取得之前的金錢
    int allMoney = p->getMoney();

    creatCharacter();

    // 依照記錄重新分配數值
    p->setMoney(allMoney);
    p->setPoint(allPoint);
    p->addState(recPt[eMAX_HP], recPt[eHP], recPt[eATK], recPt[eDEF], recPt[eSPD]);
    // 重新設定武器
    p->setWeapon(wep);
    p->setArmor(arm);
}
void addPoint(){
    int add;
    int choice;
    vector<int> state = p->getState();
    while (p->getPoint() > 0)
    {
        displayText("請輸入想分配的數值：\n", 2);
        sleep(200);
        cout << "0. 保留點數\n"
             << "1. 生命力\n"
             << "2. 攻擊力\n"
             << "3. 防禦力\n"
             << "4. 速度\n";
        cin >> choice;
        reChoose(choice, 0, 4);
        if (choice == 0) break;

        cout << "目前剩餘 [" << p->getPoint() << "點] 可以分配。\n"
             << "請輸入想分配的點數：\n";
        if (!(cin >> add) || (add < 0 || add > p->getPoint())) {
            cout << "你好像沒有你輸入的點數喔。\n" 
                 << "你還剩餘 [" << p->getPoint() << "點] 可以分配。\n";
            sleep(600);
            reChoose(add, 0, p->getPoint());
        }
        if (add == 0){
            cout << "返回分配點數介面。\n";
            sleep(1000);
        }
        if (add > 0){
            switch (choice)
            {
            case 1:
                p->addState(add, add);
                break;
            case 2:
                p->addState(-1,-1, add);
                break;

            case 3:
                p->addState(-1,-1,-1, add);
                break;

            case 4:
                p->addState(-1,-1,-1,-1, add);
                break;
            }

            cout<<"你目前的數值為："<<endl;
            showPlayerState();
            cout<<endl;
            STOP_M
        }
        CLS_M
    }

    if (choice != 0)
    {
        cout<<"加點完成，要進入遊戲了嗎？\n0.離開遊戲\n1.進入遊戲\n2.更改數值"<<endl;
        cin >> choice;
        reChoose(choice, 0, 2);

        switch(choice)
        {
        case 0:
            f_Game = false;
            break;
        case 1:
            cout<<"加點完成，進入遊戲。"<<endl;
            sleep(400);
            break;
        case 2:
            cout<<"更改數值，重新配點。"<<endl;
            sleep(400);
            addPoint();
            break;
        }
    }
}
void endGame()
{
    p->setState(0,0);
    displayText("臨死前的你，不斷地責怪疏忽大意的自己\n", 100);
    sleep(800);
    displayText("而你的意識也越來越模糊，直到...\n......\n...............\n", 150);
    sleep(1000);
    CLS_M
    displayText("\t「 好... 像... ...\n\t\t有點...\n\t.........", 200);
    sleep(1200);
    displayText("\t..........\n\n\t\t.....\t冷...... 」\n", 300);
    sleep(1500);
    f_Game = false;
}
bool isEscape()
{
    // 逃跑
    CLS_M
    cout << "\n你嘗試逃離" << m->getRoleName() << "的追擊\n";
    if (random(1, 50) - p->getState()[eSPD] < random(1, 50) - m->getState()[eSPD])
    {
        cout << "\n你成功逃離了 " << m->getRoleName() << " 的追擊\n";
        sleep(500);
        return true;
    }
    cout << "\n敵方的速度太快了，你沒有逃離 " << m->getRoleName() << " 的追擊\n"
         << "受到 " << p->onHit(m->getState()[eATK]) << " 點傷害\n";
    sleep(500);
    return false;
}
void showAttack()
{
    CLS_M
    isFight(true);
    // 怪物先手判斷
    if (m->getState()[eSPD] > p->getState()[eSPD])
    {
        int rd = random(1, 10);
        bool isDEF = (rd + random(0, 1))&1? true : false; // &1 以位元做 and，用來判斷是否奇數
        cout << m->getRoleName() << "偷襲成功，你受到 " << p->onHit( m->getState()[eATK], isDEF ) << " 點傷害\n\n";
        showPlayerState();
        sleep(2500);
    }

    // 戰鬥
    bool ft_flag = true;
    while(ft_flag && m->getState()[eHP] > 0 && p->getState()[eHP] > 0)
    {
        int opt, dmg;

        CLS_M
        showMonsterState();
        showPlayerState();
        cout<<"1.攻擊 2.防禦 3.祈禱 4.逃跑"<<endl;
        cin>>opt;
        reChoose(opt, 1, 4);
        switch (opt)
        {
        case 1:
            {
                CLS_M
                bool m_def = random(0, 100) <= 30;
                dmg = m->onHit( p->getState()[eATK], m_def ); // 怪物 30% 機會防禦成功
                showMonsterState();
                showPlayerState();
                if (!m_def)
                {
                    cout << "你對"<< m->getRoleName() << "發動了攻擊，"
                        << "造成 " << dmg << " 點傷害\n";
                } else {
                    cout << m->getRoleName() << "架起防禦姿勢，"
                        << "你造成 " << dmg << " 點傷害\n";
                }
                    
                cout << m->getRoleName() << "向你發動了攻擊，";
                dmg = p->onHit( m->getState()[eATK] );
                cout << "受到 "<< dmg << " 點傷害     \n";
                sleep(1200);
                break;
            }
        case 2:
            {
                CLS_M
                dmg = p->onHit( m->getState()[eATK], true );
                showMonsterState();
                showPlayerState();
                cout << "你架好了防禦姿勢。\n" << m->getRoleName() << "向你發動了攻擊，";
                cout << "受到 "<< dmg << " 點傷害     \n";
                sleep(1200);
                break;
            }
        case 3:
            {
                CLS_M
                p->isRecovery( random(p->getState()[eHP] * 0.1, p->getState()[eHP] * 0.3) );
                displayText("聖光灑落在你身上，恢復了些微血量。\n\n", 2);
                sleep(500);
                if (random(1, 100) <= 35)
                    displayText( m->getRoleName() +
                                "趁機偷襲，你受到 " +
                                to_string(p->onHit(m->getState()[eATK])) +
                                " 點傷害\n\n" );
                sleep(200);
                cout<<"\t[ 玩家狀態 ]"<<endl;
                showPlayerState();

                STOP_M
                break;
            }
        case 4:
            CLS_M
            if (isEscape()) ft_flag = false;
            break;
        }
        sleep(600);
    }

    if (!ft_flag) return;
    if (p->getState()[eHP] > 0)
    {
        p->setMoney(p->getMoney() + m->getMoney());

        cout << "\n==========< - >==========\n"
             << "玩家戰勝了" << m->getRoleName()
             << "\n獲得 $" << m->getMoney()
             << "\n目前共有 $" << p->getMoney()
             << "\n==========<+##+>==========\n";
    } else {
        cout << "\n==========< - >==========\n"
             << "玩家敗給了" << m->getRoleName();
        if (p->getArmor()->getName() != "")
            cout << "\n裝備毀損 " << p->getArmor()->getName();
        cout << "\n==========<+##+>==========\n";

        p->setArmor( arm_drop.makeArmor(0, "") );
    }
    sleep(1000);
}


/* 程式主體 */
int main()
{
    #ifdef _WIN32
    system("chcp 65001");
    #endif

    CLS_M
    int debuff = 0;

    creatCharacter();

    // 裝備物件, 是否扣錢
    p->setWeapon( wep_drop.makeWeapon(p->getRoleType(), "銳利刺匕"), false );
    p->setArmor( arm_drop.makeArmor(p->getRoleType(), "鐵製背心"), false );
    p->isRecovery( p->getArmor()->getHealth() );

    addPoint();

    if (f_Game) showLoadingAnimation(25);
    sleep(20);
    STOP_M


    // 設定例外事件，跳過該回合
    setExcept( {R_1, R_2} );
    while (f_Game && Desert_menu())
    {
        upPtr(p, m);
        switch(whichEvent())
        {
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
            case PT:
                CLS_M
                addPoint();
                break;
            case CHG:
                CLS_M
                changeCharacter(p->getWeapon(), p->getArmor());
                break;
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
    string wep_name = wep_drop_names[ random(0, wep_drop_names.size()-1) ];
    Weapon *m_wep = new MonsterWeapon(wep_name, random(0, 2)); // 回傳特別武器

    showAttack();
    CLS_M
    if (m->getState()[eHP] <= 0)
    {
        cout << "你撿起了哥布林死前握在手上的武器 " << m_wep->getName() << "\n";
        p->setWeapon( m_wep, false );
        showPlayerState();
        sleep(1000);
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
#include "objecct.h"

void starGame(){
    int start;//開始遊戲確認
    cout << "普通的文字RPG \n 開始遊戲(1) \n 退出遊戲(2) \n";
    cin >> start;
    while (start){
        if (start==1){
            displayText("歡迎來到普通的文字RPG\n",3);
            sleep(500);
            CLS_M
            break;
        }
        else if (start == 2){
            cout << "退出遊戲";
            exit(0); // 直接終止程式運行
        }else{
            cin.clear();
            cin.ignore();
            cout << "輸入無效!\n 開始遊戲(1)\n退出遊戲(2)\n";
            cin>>start;
        }
    }
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
                cout << "==========< state >==========";
                for (int i=1; i <= 3; i++)
                {
                    ex = makeRole(i);
                    displayText("\n職業: ", 3);
                    cout << ex->getRoleName();
                    displayText("\n血量: ", 3);
                    cout << ex->getState()[eHP];
                    displayText(" / ", 3);
                    cout << ex->getState()[eMAX_HP];
                    displayText("\n攻擊力: ", 3);
                    cout << ex->getState()[eATK];
                    displayText("\n防禦力: ", 3);
                    cout << ex->getState()[eDEF];
                    displayText("\n速度: " , 3);
                    cout << ex->getState()[eSPD];
                    displayText("\n初始金幣: ",3);
                    cout << ex->getMoney();
                    if (i < 3) cout << "\n==========<---+--->==========";
                    delete ex;
                }
                cout << "\n==========<---o--->==========\n";
                STOP_M
                break;
            case 1:
            case 2:
            case 3:
                ex = makeRole(role_opt);
                displayText("\n你選擇的職業是： ",3);
                cout << ex->getRoleName() << endl;
                displayText("確定要使用這個角色嗎？\n(0) 取消 (1) 確定\n",3);
                cin  >> role_flag;
                reChoose(role_flag, 0, 1);
                break;
        }
    } while ( !role_flag );//直到玩家選擇1才跳出迴圈

    p = ex;
    upPtr(p);
    CLS_M
    displayText("你選擇了" + p->getRoleName() + "\n", 3);
    sleep(500);
}


/* 轉職及加點 */
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
    p->addState(recPt[eHP], recPt[eATK], recPt[eDEF], recPt[eSPD]);
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
                p->addState(add);
                break;
            case 2:
                p->addState(-1, add);
                break;

            case 3:
                p->addState(-1,-1, add);
                break;

            case 4:
                p->addState(-1,-1,-1, add);
                break;
            }

            cout<<"你目前的數值為："<<endl;
            showPlayerState();
            cout<<endl;
            STOP_M
        }
        CLS_M
    }

    // 初次加點才執行
    int sum_pt = 0;
    for (auto pt: p->getPointHistory()) sum_pt += pt;

    if (sum_pt == 0 && choice != 0)
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
            p->resetPoint(); // [新功能] 重置點數 :D
            cout<<"更改數值，重新配點。"<<endl;
            sleep(400);
            CLS_M
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
    // 先手判斷
    if (m->getState()[eSPD] > p->getState()[eSPD])
    {
        int rd = random(1, 10);
        bool isDEF = (rd + random(0, 1))&1? true : false;
        cout << m->getRoleName() << "偷襲成功，你受到 " << p->onHit( m->getState()[eATK], isDEF ) << " 點傷害\n\n";
        showPlayerState();
        STOP_M
        CLS_M
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
        p->setPoint(p->getPoint() + m->getPoint());

        cout << "\n==========< - >==========\n"
             << "玩家戰勝了" << m->getRoleName()
             << "\n獲得 $" << m->getMoney()
             << "\n目前共有 $" << p->getMoney()
             << "\n獲得 " << m->getPoint() << "pt"
             << "\n目前共有 " << p->getPoint() << "pt"
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

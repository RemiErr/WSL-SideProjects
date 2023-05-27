#include "Event.h"

#ifndef STD_H
using namespace std;
#define cout std::cout
#define cin std::cin
#endif


/* 事件前置宣告 */
void runEvnG3( Event *e, Character *p, Character *m );


int main()
{
    CLS_M
    WeaponShop wep_shop;
    ArmorShop arm_shop;
    Event e;

    Character *p = makeRole(-1);
    Character *m = makeRole(-1);

    map<int, string> mlist
    {
        {1, "史萊姆"},
        {2, "哥布林"},
        {3, "寶箱怪"}
    };

    p = makeRole(1);

    p->setWeapon( wep_shop.makeWeapon(1, "銳利刺匕"), false ); // 裝備物件, 是否扣錢
    p->setArmor( arm_shop.makeArmor(1, "鐵製背心"), false );

    e.showLoadingAnimation(50);
    sleep(20);
    CLS_M STOP_M
    while (e.upPtr(p, m) && p->getState()[1]>0)
    {
        char temp;
        e.desert_event();
        switch(e.whichEvent())
        {
            case G_3:
                runEvnG3(&e, p, m);
                break;
        }
        sleep(2500);
    }
}



/* 事件包裝 */
#define PTRS_M Event *e, Character *p, Character *m

void runEvnG3( PTRS_M )
{
    m = makeRole(0, "史萊姆");


    char temp;
    while(e->isFight())
    {
        cout << "\n====================\n遭遇" << m->getRole() 
        << ", 怪物血量: " << m->getState()[1] << " / " << m->getState()[0] << endl;

        cout << "是否戰鬥? (y/n): ";
        cin >> temp;

        if (temp == 'y' || temp == 'Y')
        {
            while(m->getState()[1] > 0)
            {
                p->onHit( m->getState()[2] );
                m->onHit( p->getState()[2] );
                cout << "\n====================\n玩家血量: " << p->getState()[1] << " / " << p->getState()[0] << endl;
                cout << m->getRole() << "血量: " << m->getState()[1] << " / " << m->getState()[0] << endl;
            }
            cout<<"\n====================\n玩家戰勝了"<<m->getRole()<<"\n====================\n";
        } else {
            cout<<"\n==========<⊙>==========\n";
            cout<<"|       "; e->displayText("逃跑成功");cout<<"       |\n";
            cout<<"==========<—>==========\n";
        }
        e->endFight();
        sleep(1550);
    }
}
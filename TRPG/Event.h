#ifndef EVENT_H
#define EVENT_H

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(x)
#define Sleep(x) Sleep(x)
#elif __linux__
#include <unistd.h>
#define sleep(x) sleep(x/1000)
#define Sleep(x) sleep(x/1000)
#endif

#include <random>
#include <cstdlib>
#include "Character.h"
using namespace std;

class Event
{
private:
    Event *e;
    int leave;
    int count;
    string msg;

public:
    Event() { return; }
    Event(Event &e) { linkEvent(e); }
    ~Event(){}

    void linkEvent(Event &e) { this->e = &e; }

    //加載動畫
    void showLoadingAnimation()
    {
        int width = 50;
        cout << "Loading... ";

        for (int i = 0; i <= width; ++i) {
            cout << ".";
            cout.flush();
            sleep(10);
            cout << "\b \b";
            cout.flush();
            sleep(10);

            cout << "\rLoading: ";
            cout << "[";
            for (int j = 0; j < i; ++j) {
                cout << "=";
            }
            cout << ">";
            for (int j = i + 1; j <= width; ++j) {
                cout << " ";
            }
            cout << "] " << (i * 100) / width << "%";
            cout.flush();
            sleep(10);
        }
        cout << endl;
    }

    // 逐字逐句顯示文本，延遲 0 ~ 4 級 (15 ~ 75 ms)
    void displayText(string text, int t = 1)
    {
        for (char& c : text) {
            cout << c;
            sleep([](int t){
                switch (t)
                {
                default:
                case 0:
                    return 15;
                    break;
                case 1:
                    return 30;
                    break;
                case 2:
                    return 45;
                    break;
                case 3:
                    return 60;
                    break;
                case 4:
                    return 75;
                    break;
                }
            }(t));
        }
    }

    //隨機 利用<random>2^19937散度
    int random(int min, int max) {
        static random_device rd;
        static mt19937 rng(rd());
        int ran;
        uniform_int_distribution<int> dist(1, 100000);
        ran=dist(rng)%5;
        return ran;
    }

    void foreword(){
        displayText("你從睡夢中醒來，發現自己躺在一片荒野\n你不知道自己為何在這裡，你試著回想\n但......腦中卻一片空白。\n", 1);
    }

    void Desert_go(){
        string msg;

            switch (random(1,4))
            {
            case 1:
                msg="沙漠遼闊無邊，你走了一圈又回到原點。\n\n";
                displayText(msg);
                break;
            case 2:
                msg="你走走停停，感覺自己口乾舌燥。\n前面出現了一具冒險者的骨架，\n看上去被禿鷲啃食得十分乾淨。\n你想上前去翻找遺物嗎？\n";
                displayText(msg);
                break;
            case 3:
                msg="你不小心踩到了一坨黏呼呼的液體，不到片刻那陀液體逐漸凝聚在一起，看來你打擾到了某隻沙漠史萊姆的休眠。\n\n";
                displayText(msg);
                break;
            case 4:
                msg="你在這片沙漠走了好一陣子，經過漫無目的的遊走後你不僅喪失了水份，還有時間概念。\n烈日當空，無情的陽光不斷蒸發著你的活力，你現在又餓又累又渴。\n不知過了多久，你倒下了。\n......\n............\n你感受到有一群人正用著你聽不懂的語言在你周遭對話。\n他們抱起了你，並往你的口中倒了一點水。\n你的舌頭猶如久旱逢甘霖般舒暢，那點水不僅僅滋潤了你的口腔，彷彿也滲透到你的渾身上下，你活過來了。\n\n";
                displayText(msg, 0);
                leave=1;
                break;
        }
    }

    void Desert_stay(){
        int rd;
        string msg;
            
            rd=random(1,4);
            
        switch (rd)
            {
            case 1:
                msg ="你待在原地，但甚麼都沒有發生。烈日看上去沒有絲毫動彈的跡象，彷彿要將你烤乾。\n這麼待著可能不是一個好選擇。\n";
                displayText(msg);
                break;

            case 2:
                msg = "早安\n";
                displayText(msg);
                break;

            case 3:
                msg="在原地待著不是一個好選擇 \n 即便目光可及之處沒有任何遮蔽物，但你還是覺得有生物......或是東西在盯著自己看。\n";
                displayText(msg);
                break;

            case 4:
                msg="你待在原地甚麼也不做，隱隱約約看到有一隻哥布林朝著你衝了過來，拿起武器應戰吧孩子。(進入戰鬥)\n";
                displayText(msg);
                break;

        }
        if(count>4){
                msg="顯然待在原地並不是個明智的選擇，不知為何，你感受到一雙犀利的目光正朝著自己看，你似乎成為了獵物。\n你無意間朝著某個方向看過去，於是你開始後悔，後悔不該在這裡逗留如此久。\n恭喜你，那目光的主人是一頭%*@a$d@s，祝你好運了。(進入戰鬥)\n\n死亡\n你不曉得你面對的是什麼，你只感覺自己的身體被無情的撕裂、扭曲，直至虛無。\n你倒下了。\n......\n............\n你感受到有一群人正用著你聽不懂的語言在你周遭對話。\n他們抱起了你，並往你的口中倒了一點水。\n你的舌頭猶如久旱逢甘霖般舒暢，那點水不僅僅滋潤了你的口腔，彷彿也滲透到你的渾身上下，你活過來了。\n\n";
                displayText(msg, 0);
                leave++;
            }
    }

    void desert_event(){
        string msg;
        int option;
        while (leave!=1)
        {
            msg = "你現在想做什麼？\n1往前進\n2待在原地\n";
            displayText(msg, 2);
            cin>>option;
            while (option!=1&&option!=2)
            {
                msg="請重新輸入1或2！\n1往前進\n2待在原地\n";
                displayText(msg, 2);
                cin>>option;
            }

            
            switch (option)
            {
            case 1:
                msg="你選擇了前進\n";
                displayText(msg, 2);
                Desert_go();
                break;
            case 2:
                msg="你選擇了待在原地\n";
                displayText(msg, 2);
                Desert_stay();
                count++;
                break;
            }
        }
    }
};

#endif
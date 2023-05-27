#ifndef EVENT_H
#define EVENT_H

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(x)
#define Sleep(x) Sleep(x)
#define CLS_M system("cls");
#define STOP_M system("pause");
#elif __linux__
#include <unistd.h>
#define sleep(x) usleep(x*1000.0)
#define Sleep(x) usleep(x*1000.0)
#define CLS_M system("clear");
#define STOP_M printf("Press Enter key to continue..."); fgetc(stdin);
#define INT_MAX __INT_MAX__
#endif

#include <random>
#include <cstdlib>
#include "Character.h"
#include "FileManager.h"

#ifndef STD_H
using namespace std;
#define cout std::cout
#define cin std::cin
#endif

enum eState {FOR, GO, STAY, RE};
enum eID
{
    F_1 = 1,
    G_1 = 11,
    G_2 = 12,
    G_3 = 13,
    G_4 = 14,
    S_1 = 21,
    S_2 = 22,
    S_3 = 23,
    S_4 = 24,
    S_5 = 25,
    R_1 = 31
};

class Event
{
private:
    int Leave;
    int Count;
    int Evn;
    bool Fight;

    Character *player;
    Character *monster;

    map<string, vector<string>> Msg;
    map<eState, string> State
    {
        {FOR, "foreword"},
        {GO, "go"},
        {STAY, "stay"},
        {RE, "relive"}
    };


    void reChoose(int &opt)
    {
        while (opt!=1 && opt!=2)
        {
            CLS_M
            if (!cin)
            {
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            } else {
                displayText("請重新輸入 1 或 2！\n1.往前進\n2.待在原地\n", 2);
                cin>>opt;
            }
        }
    }

public:
    Event()
    {
        Fight = false;
        FileManager file("event/Events.csv");
        Msg = file.getText();    
    }
    ~Event(){}

    bool isFight() { return Fight; }
    void endFight() { Fight = false; Evn = 0; }
    int whichEvent() { return Evn; }
    int getLeave() { return Leave; }

    bool upPtr(Character *p = nullptr, Character *m = nullptr)
    {
        if (p == nullptr && m == nullptr) return false;
        if (p != nullptr) player = p;
        if (m != nullptr) monster = m;
        return true;
    }

    //隨機 利用<random>2^19937散度
    int random(int min, int max) {
        static random_device rd;
        static mt19937 rng(rd());
        int ran;
        uniform_int_distribution<int> dist(1, 100000);
        ran = dist(rng) % (max-min+1) + min;
        return ran;
    }

    //加載動畫
    void showLoadingAnimation(double ms = 10)
    {
        int width = 50;
        cout << "\n\n\n\n\n\n\n\n\n\n";
        // cout << "Loading... ";

        for (int i = 0; i <= width; ++i) {
            cout << "<->" << flush;
            sleep(ms);
            cout << "\b" << flush;
            sleep(ms);

            cout << "\rLoading: ";
            cout << "[";
            for (int j = 0; j < i; ++j) {
                cout << "=" << flush;
            }
            cout << ">";
            for (int j = i + 1; j <= width; ++j) {
                cout << " " << flush;
            }
            cout << "] " << (i * 100) / width << "% " << flush;
            sleep(ms);
        }
        cout << endl;
    }

    // 字串轉字元，判斷是否為英數字
    // string 存取中文字時為 8進制，所以需要三組字組 (2^3)
    bool checkChar(char &c, bool eng = true, bool num = true)
    {
        for (int i='a'; i<='z'; i++)
        {
            if (eng && (c == i || c == i-20)) return true;
            if (num && (int(c) == i-'a')) return true;
        }
        return false;
    }
    // 逐字逐句顯示文本，延遲 0 ~ 4 級 (15 ~ 75 ms)
    void displayText(string msg, int t = 1)
    {
        int count = 0;
        for (char c: msg) {
            cout << c << flush;
            if (!checkChar(c) && count < 3)
            {
                count++;
                continue;
            }
            count = 0;
            sleep([&](){
                switch (t)
                {
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
                case 5:
                    return 500;
                    break;
                default:
                case -1:
                    return 0;
                    break;
                }
            }());
        }
    }
    void displayText(eState e, int opt = 1, int t = 1)
    {
        string temp = Msg[ State[e] + "_" + to_string(opt) ][0];
        displayText(temp, t);
    }

    // void foreword(){
    //     displayText(FOR);
    // }

    void Desert_go(){
        // int rd = random(1,4);
        // displayText(GO, rd, rd!=4?1:0);
        int rd = 13;
        displayText(GO, rd-10, rd!=4?1:0);
        switch ( rd )
        {
        // case 1:
        case G_3:
            Fight = true;
            Evn = rd;
            break;
        case 4:
            Leave = 1;
            break;
        default:
            break;
        }
    }

    void Desert_stay(){
        if(Count > 3){
            string msg="顯然待在原地並不是個明智的選擇，不知為何，你感受到一雙犀利的目光正朝著自己看，你似乎成為了獵物。\n"\
            "你無意間朝著某個方向看過去，於是你開始後悔，後悔不該在這裡逗留如此久。\n"\
            "還沒來得及看清那目光的主人是什麼樣子，身上卻已烙上了血爪印，你身體開始感覺到寒冷...\n\n"\
            "你不曉得你面對的是什麼，你只感覺自己的身體被無情的撕裂、扭曲，直至虛無。\n\n";
            displayText(msg, 0);
            displayText("\t\t 你 死 了 (新細明體)\n", 3);
            player->setState(-1,0);
            Leave = 1;
        } else {
            int rd = random(1,4);
            displayText(STAY, rd, rd!=4?1:0);
        }
    }

    void desert_event(){
        CLS_M
        string msg = "";
        int option = 0;

        msg = "你現在想做什麼？\n\n1.往前進\n2.待在原地\n";
        displayText(msg, 2);
        cin>>option;
        reChoose(option);

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
            Count++;
            break;
        default:
            reChoose(option);
            break;
        }
    }
};
#endif
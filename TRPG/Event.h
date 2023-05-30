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

#include <set>
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
    R_1 = 31,
    R_2 = 32
};


// 隨機 利用<random>2^19937散度
int random(int min, int max) {
    static random_device rd;
    static mt19937 rng(rd());
    int ran;
    uniform_int_distribution<int> dist(1, 100000);
    ran = dist(rng) % (max-min+1) + min;
    return ran;
}

void reChoose(int &opt, int min = 1, int max = 3)
{
    while (opt < min || max < opt)
    {
        if (!cin) // 等於 !cin.fail()
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        } else {
            cout << "請重新輸入數字: ";
            cin >> opt;
        }
    }
}

// 加載動畫
void showLoadingAnimation(double ms = 10)
{
    int width = 50;
    cout << "\n\n\n";

    for (int i = 0; i <= width; ++i) {
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

        cout << "<->" << flush;
        sleep(ms);
        cout << "\b" << flush;
        sleep(ms);
    }
    cout << endl;
}

/* 字串轉字元，判斷是否為英數字 */
// string、char 以 8 bit (1 Bytes) 存取字元，中文字為 2 Bytes
bool checkChar(char &c, bool eng = true, bool num = true)
{
    for (int i='a'; i<='z'; i++)
    {
        if (eng && (c == i || c == i-20)) return true;
        if (num && (int(c) == i-'a')) return true;
    }
    return false;
}


class Event
{
private:
    // 事件旗標
    int Count;
    int EvnSignal;
    bool Leave;
    bool Fight;

    Character *p;
    Character *m;

    // 例外事件 ID
    vector<eID> Except;

    // 事件排程
    vector<int> Ds_go;
    vector<int> Ds_stay;

    map<string, vector<string>> Msg;
    map<eState, string> State
    {
        {FOR, "foreword"},
        {GO, "go"},
        {STAY, "stay"},
        {RE, "relive"}
    };


protected:
    // 確保亂數不重複
    void Desert_rand(vector<int> &ds,int min, int max){
        for(int i=0; i < max-min+1; i++)
        {
            ds.push_back(random(min, max));
            for (int j=0; j < i; j++)
                if (ds[i] == ds[j])
                {
                    ds.pop_back();
                    i--;
                    break;
                }
        }
    }

    void Desert_go(){
        if (Ds_go.empty()) Desert_rand(Ds_go, G_1, G_4);
        int rd = Ds_go.at(0);
        Ds_go.erase( Ds_go.begin() );

        displayText(GO, rd-10, rd != G_1 || rd != G_4 ? 2:3);

        // 訊號旗標觸發設定
        switch ( rd )
        {
        case G_1:
            Leave = true;
            break;
        case G_3:
            Fight = true;
            break;
        case G_4:
            Leave = true;
            break;
        }
        EvnSignal = rd;
    }

    void Desert_stay(){
        if(Count >= 3){
            string msg="顯然待在原地並不是個明智的選擇，不知為何，你感受到一雙犀利的目光正朝著自己看，你似乎成為了獵物。\n"\
            "你無意間朝著某個方向看過去，於是你開始後悔，後悔不該在這裡逗留如此久。\n"\
            "還沒來得及看清那目光的主人是什麼樣子，身上卻已烙上了血爪印，你身體開始感覺到寒冷...\n\n"\
            "你不曉得你面對的是什麼，你只感覺自己的身體被無情的撕裂、扭曲，直至虛無。\n\n";
            displayText(msg, 4);
            displayText("\t\t 你 死 了 (新細明體)\n", 5);

            // 設定角色歸零
            p->setRole("");
            p->setState(0,0,0,0);
            Leave = false;
            Fight = false;
            EvnSignal = 0;
            Count = 0;
        } else {
            if (Ds_stay.empty()) Desert_rand(Ds_stay, S_1, S_4);
            int rd = Ds_stay.at(0);
            Ds_stay.erase( Ds_stay.begin() );

            displayText(STAY, rd-20);
            if (rd == S_4) Fight = true;
            EvnSignal = rd;
        }
    }


public:
    Event()
    {
        Count = 0;
        EvnSignal = 0;
        Leave = false;
        Fight = false;
        FileManager file("event/Events.csv");
        Msg = file.getText();
    }
    Event(Character *p, Character *m)
    {
        Count = 0;
        EvnSignal = 0;
        Leave = false;
        Fight = false;
        FileManager file("event/Events.csv");
        Msg = file.getText();
        upPtr(p, m);
    }
    ~Event(){}

    // 設定例外事件
    void setExcept(vector<eID> exc) { Except = exc; }
    // 是否進入戰鬥
    bool isFight() { return Fight; }
    // 是否倒下
    bool isLeave() { return Leave; }
    // 回傳事件編號
    int whichEvent() { return EvnSignal; }
    // 結束事件
    void endEvent()
    {
        Fight = false;
        Leave = false;
    }

    // 更新玩家及怪物的指標
    bool upPtr(Character *p = nullptr, Character *m = nullptr)
    {
        if (!p && !m)
            return false;

        if (p) this->p = p;
        if (m) this->m = m;
        return true;
    }

    // 逐字逐句顯示文本，延遲 0 ~ 5 級 (15 ~ 90 ms)
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
                    return 90;
                    break;
                case -1:
                    return 0;
                    break;
                default:
                    return t;
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

    // 檢查玩家是否存活，並更新訊號
    bool Desert_alive(){
        if (p->getRoleName().empty()) return false; // 即死事件

        if (Leave)
        {   // 倒下事件
             EvnSignal = R_1;
        } else if (Fight && p->getState()[1] <= 0)
             EvnSignal = R_2; // 被打倒
        else EvnSignal = 0;
        return true;
    }

    // 事件選項
    bool Desert_menu(){
        // 若碰到例外事件，跳過該次選擇
        for (auto ex: Except)
            if(EvnSignal == ex) return true;

        CLS_M
        string msg = "";
        int option = 0;

        msg = "你現在想做什麼？\n\n(1) 往前進\n(2) 待在原地\n(3) 離開遊戲\n";
        displayText(msg, 2);
        cin >> option;
        reChoose(option);

        switch (option)
        {
        case 1:
            msg = "你選擇了前進\n";
            displayText(msg, 2);
            Desert_go();
            Count = 0;
            break;
        case 2:
            msg="你選擇了待在原地\n";
            displayText(msg, 2);
            Desert_stay();
            Count++;
            break;
        case 3:
            return false;
            break;
        default:
            reChoose(option);
            break;
        }
        // Desert_alive();
        return true;
    }
};
#endif
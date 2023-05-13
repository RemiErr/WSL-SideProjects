#include "teacher.h"
#include "student.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

/*
教師用 Terminal 進行檔案控制
學生用 Node-Red GUI 查看資料
*/


map<int, vector<string>> readFile(string url)
{
    map<int, vector<string>> datas;
    vector<string> data_line;
    ifstream file(url);
    string res = "";

    int count = 0;
    getline(file, res); // 吃掉第一行
    while (getline(file, res))
    {
        // string temp = "";
        istringstream line(res); // 輸出輸入的類別，需引用 sstream
        while (getline(line, res, ','))
            data_line.push_back(res);

        datas[count] = data_line;
        data_line.clear();
        count++;
    }
    file.close();
    return datas;
}

int class_1(Teacher &t, Student &s)
{


}
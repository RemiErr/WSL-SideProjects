#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#ifndef STD_H
using namespace std;
#define cout std::cout
#define cin std::cin
#endif

class FileManager
{
private:
    string URL;

public:
    FileManager(string url)
    {
        // 自動轉換路徑格式
        #ifdef _WIN32
        for (auto &u: url)
            if (u == '/') u = '\\';
        #elif __linux__
        for (auto &u: url)
            if (u == '\\') u = '/';
        #endif

        URL = url;
    }
    ~FileManager(){};

    map<string, vector<int>> getData(char split = ',')
    {
        map<string, vector<int>> data;
        ifstream file(URL);
        if (!file) return {};

        string line = "";
        while(getline(file, line) && line.length())
        {
            int prev_index = 0, index = 0;

            // 整行字串分割
            vector<string> temp{};
            for (char &c: line)
            {
                index = &c - &line[0]; // 透過位址差值取得 index
                if (c == split || &c == &line[line.length()-1]) // 若當前字元為 符號 或 最後一字
                {
                    temp.push_back(line.substr(prev_index, index));
                    prev_index = (c == split? index+1 : index);
                }
            }

            for (auto &t: temp)
            {
                if (&t == &temp[0])
                    data[t] = {};
                else
                    data[temp[0]].push_back(stoi(t));
            }
        }
        file.close();

        return data;
    };

    map<string, vector<string>> getText(char split = ',')
    {
        map<string, vector<string>> data;
        ifstream file(URL);
        if (!file) return {};

        string line = "";
        while(getline(file, line) && line.length())
        {
            int prev_index = 0, index = 0;

            // 整行字串分割
            vector<string> temp{};
            for (char &c: line)
            {
                index = &c - &line[0]; // 透過位址差值取得 index
                if (c == '\\') c = '\n';
                if (c == split || &c == &line[line.length()-1]) // 若當前字元為 符號 或 最後一字
                {
                    temp.push_back(line.substr(prev_index, index));
                    prev_index = (c == split? index+1 : index);
                }
            }

            for (auto &t: temp)
            {
                if (&t == &temp[0])
                    data[t] = {};
                else
                    data[temp[0]].push_back(t);
            }
        }
        file.close();

        return data;
    };
};

#endif
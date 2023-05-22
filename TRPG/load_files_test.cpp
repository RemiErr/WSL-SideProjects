#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

enum URL {ARM, WEP, MON};
map<URL, string> url
{
    {ARM, "Armors"},
    {WEP, "Weapons"},
    {MON, "Monsters"},
};

map<string, vector<int>> armors;
int main()
{
    fstream file("res/" + url[MON] + ".csv");
    if (!file) return 0;

    string line = "";
    while(getline(file, line) && line.length())
    {
        int prev_index = 0, index = 0;
        vector<string> temp{};

        for (char &c: line)
        {
            index = &c - &line[0];
            if (c == ',' || &c == &line[line.length()-1])
            {
                temp.push_back(line.substr(prev_index, index));
                prev_index = c==','? index+1 : index;
            }
        }

        for (auto &t: temp)
        {
            if (&t == &temp[0])
                armors[t] = {};
            else
                armors[temp[0]].push_back(stoi(t));
        }
    }
    file.close();

    for (auto t:armors)
    {
        cout<<t.first<<", ";
        for (auto &tt:t.second)
            cout<<tt<<(&tt != &t.second.back()? ", ":"\n");
    }
}
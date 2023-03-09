// https://mikethreeacer.medium.com/pybind11-4f538dbbc946
// $ g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` <your_c++_filename>.cpp -o <your_module_name>`python3-config --extension-suffix` -I </path/to/python3>
// g++ -shared -std=c++17 -fPIC `python3 -m pybind11 --includes` ./res/lib.cpp -o ./res/lib`python3-config --extension-suffix` -I/usr/include/python3.8

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <bits/stdc++.h>
using namespace std;
namespace py = pybind11;

class Test
{
private:
    string txt = "Array :";

public:
    int sumC(int x, int y)
    {
        cout << "C++ type: <int>" << '\n';
        return x + y;
    }
    double sumC(double x, double y)
    {
        cout << "C++ type: <double>" << '\n';
        return x + y;
    }
    string sumC(string x, string y)
    {       
        cout << "C++ type: <string>" << '\n';
        return x + y;
    }
    // char sumC(char x, char y)
    // {
    //     cout << "C++ type: <char>" << '\n';
    //     return x + y;
    // }
    int subC(int x, int y)
    {
        cout << "C++ type: <int>" << '\n';
        return x - y;
    }
    double subC(double x, double y)
    {
        cout << "C++ type: <double>" << '\n';
        return x - y;
    }
    string subC(string x, string y)
    {       
        cout << "C++ type: <string>" << '\n';
        return x +'\n'+ y;
    }
    string ctos(char c[])
    {
        cout << "C++ type: <char> to <string>" << '\n';
        string s = "";
        for (int i=0; i<strlen(c); i++) s += c[i];
        return s;
    }
    void vecTest(vector<int> vec)
    {
        int sum = 0;
        for (int i = 0; i < vec.size(); i++)
            sum += vec[i];
        cout << txt << sum << '\n';
        // return sum;
    }
};



PYBIND11_MODULE(lib, m)
{
    py::class_<Test>(m, "Test")
    .def(py::init())
    .def("pySum", py::overload_cast<int,int>(&Test::sumC))
    .def("pySum", py::overload_cast<double,double>(&Test::sumC))
    .def("pySum", py::overload_cast<string,string>(&Test::sumC))
    // .def("pySum", py::overload_cast<char,char>(&Test::sumC))
    .def("pySub", py::overload_cast<int,int>(&Test::subC))
    .def("pySub", py::overload_cast<double,double>(&Test::subC))
    .def("pySub", py::overload_cast<string,string>(&Test::subC))
    .def("c2s", &Test::ctos)
    .def("pyVec", &Test::vecTest);
}
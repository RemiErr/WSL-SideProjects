// #include "libtk.h"
// #include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;


// 一般副程式
extern "C" void hello()
{
    printf("Hello!!");
}

extern "C" bool isPrime(int n)
{
    for (int i=2; n==2 || i<n; i++)
    {
        return n!=2? (n%i==0? false:true):true;
    }
    return false;
}

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

// 讀取陣列
extern "C" int sumArray(int *arr, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

// 輸出陣列
extern "C" int* getPrimeSheet(int a, int b)
{
    if (a > b) swap(a,b);
    int *arr = (int*) (malloc((b-a+1) * sizeof(int)));

    for (int n=0; n<=(b-a+1); n++) arr[n] = 1;
    arr[0] = arr[1] = 0;

    for (int i=a; i<=(b-a+1); i++)
    {
        if (arr[i] && isPrime(i))
        {
            for (int j=i+i; j<=(b-a+1); j+=i)
                arr[j] = 0;
        }
        else if (arr[i])
        {
            arr[i] = 0;
        }
    }
    return arr;
}

// extern "C" int* getPrimeSheet(int a)
// {
//     int *arr = (int*) malloc((a+1) * sizeof(int));

//     // memset(arr, 1, sizeof(arr));
//     for (int n=0; n<=a+1; n++) arr[n] = 1;
//     arr[0] = arr[1] = 0;

//     for (int i=2; i<=a; i++)
//     {
//         for (int j=i+i; j<=a+1; j+=i)
//             arr[j] = 0;
//     }
//     return arr;
// }
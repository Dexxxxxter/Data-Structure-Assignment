/*
建立随机顺序的int数组
*/

#include <iostream>
#include <cstdlib>
#include <random>
#include <algorithm>
#include "datamgr.h"

#define MAX_SIZE 10000000 //10,000,000

using namespace std;

//升序数组
void ascending_order(int *A, int len)
{
    for (int i = 0; i < len; i++)
    {
        A[i] = i;
    }
    return;
}

//降序数组
void descending_order(int *A, int len)
{
    for (int i = 0; i < len; i++)
    {
        A[i] = len - 1 - i;
    }
    return;
}

//将数组随机打乱
void random_order(int *A, int len, int seed)
{
    srand(seed);
    shuffle(&A[0], &A[len - 1], default_random_engine(seed));
    // for (int i = len - 1; i >= 0; i++)
    // {
    //     uniform_int_distribution<int> U(0, i);
    //     swap(A[i], A[U(seed)]);
    // }
    return;
}

//存入文件
void save_as_file(string file_path, int *A, int len)
{
    FILE *fp = fopen(file_path.c_str(), "w+");
    if (fp == NULL)
    {
        cout << "file save error";
        system("pause");
        exit(-1);
    }
    for (int i = 0; i < len; i++)
    {
        fprintf(fp, "%d\n", A[i]);
    }
    fclose(fp);
    return;
}

//读入文件内容，将其存入数组
void load_file(string file_path, int *A, int &len)
{
    FILE *fp = fopen(file_path.c_str(), "r");
    if (fp == NULL)
    {
        cout << "file save error";
        system("pause");
        exit(-1);
    }
    len = 0;
    int flag = 1;
    while (flag != EOF)
    {
        flag = fscanf(fp, "%d\n", &A[len++]);
    }
    len--; //读EOF的时候多加了一次
    fclose(fp);
    return;
}

//生成新数据
string make_new_data()
{
    string ascending_order_filepath = ".\\asc.txt";
    string descending_order_filepath = ".\\des.txt";
    string random_order_filepath = ".\\rand.txt";
    string res_path;

    int *A = new int[MAX_SIZE];
    //default value
    int seed = 419;
    int default_len = 100000;
    int len = 0;
    int type = 1;

    cout << "input length:\n(enter 0 to use default value)" << endl;
    cin >> len;
    while (len < 0 || len >= MAX_SIZE)
    {
        cout << "invalid length, please input again" << endl;
        cin >> len;
    }
    if (len == 0)
    {
        len = default_len;
    }

    cout << "select data type:\n[ 1:ascending_order / 2: descending_order / 3: random_order ]" << endl;
    cin >> type;
    while (type != 1 && type != 2 && type != 3)
    {
        cout << "invalid type, please input again" << endl;
        cin >> type;
    }
    switch (type)
    {
    case 1:
        ascending_order(A, len);
        save_as_file(ascending_order_filepath, A, len);
        res_path = ascending_order_filepath;
        break;
    case 2:
        descending_order(A, len);
        save_as_file(descending_order_filepath, A, len);
        res_path = descending_order_filepath;
        break;
    case 3:
        ascending_order(A, len);
        random_order(A, len, seed);
        save_as_file(random_order_filepath, A, len);
        res_path = random_order_filepath;
        break;
    default:
        cout << "type case error?" << endl;
        system("pause");
        exit(-2);
    }

    delete[] A;
    return res_path;
}
/*
作业题目：哈夫曼编码与译码方法

哈夫曼编码是一种以哈夫曼树（最优二叉树，带权路径长度最小的二叉树）为基础变长编码方式。
其基本思想是：将使用次数多的代码转换成长度较短的编码，而使用次数少的采用较长的编码，并且保持编码的唯一可解性。
在计算机信息处理中，经常应用于数据压缩。
是一种一致性编码法（又称"熵编码法"），用于数据的无损耗压缩。

要求实现一个完整的哈夫曼编码与译码系统。 

作业要求： 
    从文件中读入任意一篇英文文本文件，分别统计英文文本文件中各字符（包括标点符号和空格）使用频率；
    根据已统计的字符使用频率构造哈夫曼编码树，并给出每个字符的哈夫曼编码（字符集的哈夫曼编码表）；
    将文本文件利用哈夫曼树进行编码，存储成压缩文件（哈夫曼编码文件）；
    计算哈夫曼编码文件的压缩率；
    将哈夫曼编码文件译码为文本文件，并与原文件进行比较。

以下可以不做，供思考，做了可以适当加分:
    能否利用堆结构，优化的哈夫曼编码算法。 
    上述1-5的编码和译码是基于字符的压缩，考虑基于单词的压缩，完成上述工作，讨论并比较压缩效果。
    上述1-5的编码是二进制的编码，可以采用K叉的哈夫曼树完成上述工作，实现“K进制”的编码和译码，并与二进制的编码和译码进行比较。
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include "myheap.h"

using namespace std;

#define MAXSIZE 130
//ascii一共128个字符的编码

typedef struct huffman_tree
{
    char ch;
    int weight; //字符权重（出现次数）
    int parent; //父节点在数组中的编号
    int lchild; //左孩子
    int rchild; //右孩子
} * Htree, Node;

typedef struct huffman_chart
{
    char ch;            //字符
    char bstr[MAXSIZE]; //字符对应编码（01字符串）
} * Hchart, Chart;

//计算每个字符对应的权重（出现次数）
void Count(string file_path, int *count)
{
    //c++ style
    char buffer;
    fstream infile;
    infile.open(file_path);
    cout << "reading..." << endl;
    while (infile.get(buffer))
    {
        if (buffer >= 0 && buffer <= 127)
        {
            count[buffer]++;
        }
        else
        {
            cout << "file contains char out of ASCII!" << endl;
            // infile << "!!!!!";
            infile.close();
            system("pause");
            exit(-1);
        }
    }
    infile.close();
    return;

    // //c style
    // char ch;
    // FILE *infile = fopen(file_path.c_str(), "rb");
    // if (infile == NULL)
    // {
    //     cout << "read file error" << endl;
    //     system("pause");
    //     exit(0);
    // }
    // cout << "reading..." << endl;
    // while (!feof(infile))
    // {
    //     ch = fgetc(infile);
    //     if (ch >= 0 && ch <= 127)
    //     {
    //         count[ch]++;
    //     }
    //     else
    //     {
    //         // TODO 或者可以用<unk>代替未知字符?
    //         cout << "file contains char out of ASCII!" << endl;
    //         system("pause");
    //         exit(0);
    //     }
    // }
    // return;
}

//初始化哈夫曼树的节点
Htree Init_Node(int *char_weight, int size)
{
    Htree newTree = new Node[2 * size - 1]; //多出size-1个节点
    for (int i = 0; i < size; i++)
    {
        newTree[i].ch = i; //字符为数组编号在ascii表中所对应的字符
        newTree[i].weight = char_weight[i];
        newTree[i].parent = newTree[i].lchild = newTree[i].rchild = -1; //父节点和左右子孩子都赋值为-1
    }
    return newTree;
}

//找到最小的两个节点的位置并返回
void Select_Min(Htree T, int i, int *p1, int *p2)
{
    // TODO 只对部分字符建立树?
    int min1 = -1, min2 = -1;
    for (int j = 0; j < i; j++)
    {
        if (T[j].parent < 0)
        {
            if (min1 < 0 || T[j].weight < T[min1].weight)
            {
                min2 = min1;
                min1 = j;
            }
            else if (min2 < 0 || T[j].weight < T[min2].weight)
            {
                min2 = j;
            }
        }
    }
    *p1 = min1;
    *p2 = min2;
    return;
}

//建立由树的字符节点（前size个）构成的堆
Heap *Build_Heap(Htree T, int size)
{
    int k = 1;
    Heap *start = new Heap;
    start->n = 0;
    HeapNode newnode;
    for (int i = 0; i < size; i++)
    {
        newnode.ch = T[i].ch;
        newnode.weight = T[i].weight;
        newnode.pos = i;
        Heap_Insert(start, newnode);
    }
    return start;
}

//根据各个字符的权重建立哈夫曼树
Htree BuildHtree(int *char_weight, int size)
{
    //n个字符 -> 2n-1个节点
    int i, p1, p2;
    Htree T = Init_Node(char_weight, size);
    Heap *heap = Build_Heap(T, size); //将树中每个节点建立一个堆
    for (int i = size; i < 2 * size - 1; i++)
    {
        //Select_Min(T, i, &p1, &p2);
        Select_Min_using_Heap(heap, &p1, &p2);
        T[p1].parent = T[p2].parent = i;
        T[i].lchild = p1;
        T[i].rchild = p2;
        T[i].weight = T[p1].weight + T[p2].weight;

        HeapNode newnode;
        newnode.ch = T[i].ch;
        newnode.pos = i;
        newnode.weight = T[i].weight;
        Heap_Insert(heap, newnode);
    }
    return T;
    //返回结构体数组
    //根节点为T[2*size-2]
}

//将01字符串转换为二进制数据(1 byte)
unsigned char Str2Bin(char *bstr)
{
    int a = 0;
    for (int i = 0; i < 8; i++)
    {
        if (bstr[i] == '1')
        {
            a = a | (0x1 << (7 - i));
        }
    }
    return (unsigned char)a;
}

//将一个字节的二进制数据转换为01字符串
void Bin2Str(unsigned char a, char *buf)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        buf[i] = (((a >> (7 - i)) & 0x1) != 0) ? '1' : '0';
    }
    buf[i] = '\0';
    return;
}

//将哈夫曼树编码成字符串形式的01编码
void Htree_Encode(Htree T, Hchart chart)
{
    int c, p, start; // c: child, p: parent
    char buffer[MAXSIZE + 1];
    buffer[MAXSIZE] = '\0';
    for (int i = 0; i < MAXSIZE; i++)
    {
        chart[i].ch = i;
        start = MAXSIZE;
        c = i;
        while (T[c].parent >= 0) //到根节点为止
        {
            p = T[c].parent;
            buffer[--start] = (T[p].lchild == c) ? '0' : '1';
            c = p;
        }
        strcpy(chart[i].bstr, buffer + start);
    }
    return;
}

//将01字符串编码通过哈夫曼树转换为字符
char Htree_Decode(Htree T, char *c)
{
    int p = 0; //指示当前节点位置
    int i = 0;
    while (T[p].rchild != -1 || T[p].rchild != -1)
    {
        if ((T[p].lchild == -1 && c[i] == 0) || (T[p].rchild == -1 && c[i] == 1))
        {
            cout << "tree decode error" << endl;
        }
        p = (c[i++] == 0) ? T[p].lchild : T[p].rchild;
    }
    return T[p].ch;
}

//将buf中的数据前移len个单位
void Realign(char *buf, int len)
{
    int p = 0;
    while (buf[p] != '\0')
    {
        buf[p] = buf[p + len];
        p++;
    }
    buf[p] = '\0';
    return;
}

//将文件压缩
void File_Compress(string file_path, string dest_path, Hchart chart)
{
    clock_t start_t, end_t;
    start_t = clock(); //计时

    FILE *fp = fopen(file_path.c_str(), "r");
    FILE *fnew = fopen(dest_path.c_str(), "wb+");
    if (fp == NULL || fnew == NULL)
    {
        cout << "无法打开文件" << endl;
        exit(0);
    }
    char buffer[2 * MAXSIZE];
    int end = 0;
    int end_len = 0;
    fwrite(&end_len, sizeof(int), 1, fnew); //开头留下int大小的空间
    while (!feof(fp))
    {
        char ch = fgetc(fp);
        strcpy(buffer + end, chart[(int)ch].bstr); //向缓冲区加入ch对应编码
        end += strlen(chart[(int)ch].bstr);
        while (end >= 8) //如果缓冲区长度大于8则转换为bin写入新文件
        {
            unsigned char uch = Str2Bin(buffer);
            fwrite(&uch, sizeof(unsigned char), 1, fnew);
            Realign(buffer, 8);
            end -= 8;
        }
    }
    end_len = end; //记录末尾长度
    //最后一段长度如果不足8，则补满0
    for (; end < 8; end++)
    {
        buffer[end] = 0;
    }
    unsigned char uch = Str2Bin(buffer);
    fwrite(&uch, sizeof(unsigned char), 1, fnew); //写入最后一个字节
    rewind(fnew);                                 //指针指向开头
    fwrite(&end_len, sizeof(int), 1, fnew);       //在开头记录末尾长度
    fclose(fp);
    fclose(fnew);

    end_t = clock();
    cout << "compression completed, "
         << "using " << (end_t - start_t) << " ms" << endl;

    return;
}

//解压文件
void File_Decompress(string file_path, string dest_path, Htree T)
{
    clock_t start_t, end_t;
    start_t = clock(); //计时

    FILE *fp = fopen(file_path.c_str(), "rb");
    FILE *fdecomp = fopen(dest_path.c_str(), "w+");
    FILE *next = fopen(file_path.c_str(), "rb"); //next标记fp后一位置
    if (fp == NULL || fdecomp == NULL)
    {
        cout << "无法打开文件" << endl;
        exit(0);
    }
    int end_len; //末尾字节长度
    fread(&end_len, sizeof(int), 1, next);
    fread(&end_len, sizeof(int), 1, fp);
    fgetc(next); //next指针指向下一个位置
    int end = 8;
    char buffer[8 + 1]; //存01字符串
    unsigned char uch;  //储存当前从压缩文件中读出的char
    int root = 2 * MAXSIZE - 2;
    int p = root; //指示当前树中节点位置
    int i = 0;    //指示buffer位置
    while (!feof(fp))
    {
        i = 0;
        fread(&uch, sizeof(unsigned char), 1, next);
        fread(&uch, sizeof(unsigned char), 1, fp);
        Bin2Str(uch, buffer);
        if (feof(next)) //判断是否为最后一段
        {
            end = end_len;
        }
        while (i < end)
        {
            if (T[p].rchild != -1 || T[p].lchild != -1)
            {
                if ((T[p].lchild == -1 && buffer[i] == 0) || (T[p].rchild == -1 && buffer[i] == 1))
                {
                    cout << "tree decode error" << endl;
                }
                p = (buffer[i++] == '0') ? T[p].lchild : T[p].rchild; //向左/右走
            }
            else //如果到达叶子节点，则向文件中输出对应字符
            {
                //fwrite(&T[p].ch, sizeof(char), 1, fdecomp);
                fputc(T[p].ch, fdecomp);
                p = root;
            }
        }
    }
    fclose(fp);
    fclose(fdecomp);

    end_t = clock();
    cout << "decompression completed, "
         << " using " << (end_t - start_t) << " ms" << endl;

    return;
}

//获取文件大小
size_t GetFileSize(const string &file_name)
{
    ifstream in(file_name.c_str());
    in.seekg(0, ios::end);
    size_t size = in.tellg();
    in.close();
    return size; //单位是byte
}

//计算压缩率
void Calculate_CompRatio(const string file_path, const string dest_path)
{
    size_t original = GetFileSize(file_path);
    size_t compressed = GetFileSize(dest_path);
    double rate;
    rate = (double)compressed / (double)original;
    cout << "The compression ratio is " << setprecision(4) << rate * 100 << "%" << endl;
    return;
}

int main()
{
    // TODO 加入输入文件选择
    string file_path = ".\\test_text3.txt";
    //string file_path = "C://assignment/assignment_3_2/test_text3.txt";
    string dest_path = ".\\test_text3.txt.huf";
    string decomp_path = ".\\test_text3_decomp.txt";

    int char_weight[MAXSIZE] = {0};
    Chart htree_chart[MAXSIZE];
    Count(file_path, char_weight);
    Htree T = BuildHtree(char_weight, MAXSIZE);
    Htree_Encode(T, htree_chart);
    File_Compress(file_path, dest_path, htree_chart);
    File_Decompress(dest_path, decomp_path, T);
    Calculate_CompRatio(file_path, dest_path);
    delete[] T; //释放内存

    system("pause");
    return 0;
}
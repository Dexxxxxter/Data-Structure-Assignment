/*
��ҵ��Ŀ�����������������뷽��

������������һ���Թ������������Ŷ���������Ȩ·��������С�Ķ�������Ϊ�����䳤���뷽ʽ��
�����˼���ǣ���ʹ�ô�����Ĵ���ת���ɳ��Ƚ϶̵ı��룬��ʹ�ô����ٵĲ��ýϳ��ı��룬���ұ��ֱ����Ψһ�ɽ��ԡ�
�ڼ������Ϣ�����У�����Ӧ��������ѹ����
��һ��һ���Ա��뷨���ֳ�"�ر��뷨"�����������ݵ������ѹ����

Ҫ��ʵ��һ�������Ĺ���������������ϵͳ�� 

��ҵҪ�� 
    ���ļ��ж�������һƪӢ���ı��ļ����ֱ�ͳ��Ӣ���ı��ļ��и��ַ������������źͿո�ʹ��Ƶ�ʣ�
    ������ͳ�Ƶ��ַ�ʹ��Ƶ�ʹ����������������������ÿ���ַ��Ĺ��������루�ַ����Ĺ������������
    ���ı��ļ����ù����������б��룬�洢��ѹ���ļ��������������ļ�����
    ��������������ļ���ѹ���ʣ�
    �������������ļ�����Ϊ�ı��ļ�������ԭ�ļ����бȽϡ�

���¿��Բ�������˼�������˿����ʵ��ӷ�:
    �ܷ����öѽṹ���Ż��Ĺ����������㷨�� 
    ����1-5�ı���������ǻ����ַ���ѹ�������ǻ��ڵ��ʵ�ѹ��������������������۲��Ƚ�ѹ��Ч����
    ����1-5�ı����Ƕ����Ƶı��룬���Բ���K��Ĺ��������������������ʵ�֡�K���ơ��ı�������룬��������Ƶı����������бȽϡ�
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
//asciiһ��128���ַ��ı���

typedef struct huffman_tree
{
    char ch;
    int weight; //�ַ�Ȩ�أ����ִ�����
    int parent; //���ڵ��������еı��
    int lchild; //����
    int rchild; //�Һ���
} * Htree, Node;

typedef struct huffman_chart
{
    char ch;            //�ַ�
    char bstr[MAXSIZE]; //�ַ���Ӧ���루01�ַ�����
} * Hchart, Chart;

//����ÿ���ַ���Ӧ��Ȩ�أ����ִ�����
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
    //         // TODO ���߿�����<unk>����δ֪�ַ�?
    //         cout << "file contains char out of ASCII!" << endl;
    //         system("pause");
    //         exit(0);
    //     }
    // }
    // return;
}

//��ʼ�����������Ľڵ�
Htree Init_Node(int *char_weight, int size)
{
    Htree newTree = new Node[2 * size - 1]; //���size-1���ڵ�
    for (int i = 0; i < size; i++)
    {
        newTree[i].ch = i; //�ַ�Ϊ��������ascii��������Ӧ���ַ�
        newTree[i].weight = char_weight[i];
        newTree[i].parent = newTree[i].lchild = newTree[i].rchild = -1; //���ڵ�������Ӻ��Ӷ���ֵΪ-1
    }
    return newTree;
}

//�ҵ���С�������ڵ��λ�ò�����
void Select_Min(Htree T, int i, int *p1, int *p2)
{
    // TODO ֻ�Բ����ַ�������?
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

//�����������ַ��ڵ㣨ǰsize�������ɵĶ�
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

//���ݸ����ַ���Ȩ�ؽ�����������
Htree BuildHtree(int *char_weight, int size)
{
    //n���ַ� -> 2n-1���ڵ�
    int i, p1, p2;
    Htree T = Init_Node(char_weight, size);
    Heap *heap = Build_Heap(T, size); //������ÿ���ڵ㽨��һ����
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
    //���ؽṹ������
    //���ڵ�ΪT[2*size-2]
}

//��01�ַ���ת��Ϊ����������(1 byte)
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

//��һ���ֽڵĶ���������ת��Ϊ01�ַ���
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

//����������������ַ�����ʽ��01����
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
        while (T[c].parent >= 0) //�����ڵ�Ϊֹ
        {
            p = T[c].parent;
            buffer[--start] = (T[p].lchild == c) ? '0' : '1';
            c = p;
        }
        strcpy(chart[i].bstr, buffer + start);
    }
    return;
}

//��01�ַ�������ͨ����������ת��Ϊ�ַ�
char Htree_Decode(Htree T, char *c)
{
    int p = 0; //ָʾ��ǰ�ڵ�λ��
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

//��buf�е�����ǰ��len����λ
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

//���ļ�ѹ��
void File_Compress(string file_path, string dest_path, Hchart chart)
{
    clock_t start_t, end_t;
    start_t = clock(); //��ʱ

    FILE *fp = fopen(file_path.c_str(), "r");
    FILE *fnew = fopen(dest_path.c_str(), "wb+");
    if (fp == NULL || fnew == NULL)
    {
        cout << "�޷����ļ�" << endl;
        exit(0);
    }
    char buffer[2 * MAXSIZE];
    int end = 0;
    int end_len = 0;
    fwrite(&end_len, sizeof(int), 1, fnew); //��ͷ����int��С�Ŀռ�
    while (!feof(fp))
    {
        char ch = fgetc(fp);
        strcpy(buffer + end, chart[(int)ch].bstr); //�򻺳�������ch��Ӧ����
        end += strlen(chart[(int)ch].bstr);
        while (end >= 8) //������������ȴ���8��ת��Ϊbinд�����ļ�
        {
            unsigned char uch = Str2Bin(buffer);
            fwrite(&uch, sizeof(unsigned char), 1, fnew);
            Realign(buffer, 8);
            end -= 8;
        }
    }
    end_len = end; //��¼ĩβ����
    //���һ�γ����������8������0
    for (; end < 8; end++)
    {
        buffer[end] = 0;
    }
    unsigned char uch = Str2Bin(buffer);
    fwrite(&uch, sizeof(unsigned char), 1, fnew); //д�����һ���ֽ�
    rewind(fnew);                                 //ָ��ָ��ͷ
    fwrite(&end_len, sizeof(int), 1, fnew);       //�ڿ�ͷ��¼ĩβ����
    fclose(fp);
    fclose(fnew);

    end_t = clock();
    cout << "compression completed, "
         << "using " << (end_t - start_t) << " ms" << endl;

    return;
}

//��ѹ�ļ�
void File_Decompress(string file_path, string dest_path, Htree T)
{
    clock_t start_t, end_t;
    start_t = clock(); //��ʱ

    FILE *fp = fopen(file_path.c_str(), "rb");
    FILE *fdecomp = fopen(dest_path.c_str(), "w+");
    FILE *next = fopen(file_path.c_str(), "rb"); //next���fp��һλ��
    if (fp == NULL || fdecomp == NULL)
    {
        cout << "�޷����ļ�" << endl;
        exit(0);
    }
    int end_len; //ĩβ�ֽڳ���
    fread(&end_len, sizeof(int), 1, next);
    fread(&end_len, sizeof(int), 1, fp);
    fgetc(next); //nextָ��ָ����һ��λ��
    int end = 8;
    char buffer[8 + 1]; //��01�ַ���
    unsigned char uch;  //���浱ǰ��ѹ���ļ��ж�����char
    int root = 2 * MAXSIZE - 2;
    int p = root; //ָʾ��ǰ���нڵ�λ��
    int i = 0;    //ָʾbufferλ��
    while (!feof(fp))
    {
        i = 0;
        fread(&uch, sizeof(unsigned char), 1, next);
        fread(&uch, sizeof(unsigned char), 1, fp);
        Bin2Str(uch, buffer);
        if (feof(next)) //�ж��Ƿ�Ϊ���һ��
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
                p = (buffer[i++] == '0') ? T[p].lchild : T[p].rchild; //����/����
            }
            else //�������Ҷ�ӽڵ㣬�����ļ��������Ӧ�ַ�
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

//��ȡ�ļ���С
size_t GetFileSize(const string &file_name)
{
    ifstream in(file_name.c_str());
    in.seekg(0, ios::end);
    size_t size = in.tellg();
    in.close();
    return size; //��λ��byte
}

//����ѹ����
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
    // TODO ���������ļ�ѡ��
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
    delete[] T; //�ͷ��ڴ�

    system("pause");
    return 0;
}
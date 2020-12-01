/*
˼���� 1:
��֪�����������ж��Ƿ��ཻ����BAT��˾�����⣩
*/

#include <iostream>
using namespace std;

typedef struct list
{
    struct list *next;
} LIST;

void build(LIST **p1, LIST **p2, int mode)
{
    LIST *p[4];
    for (int i = 0; i < 4; i++)
    {
        p[i] = (LIST *)malloc(sizeof(LIST));
    }
    if (mode == 0) //������������Ϊ3���ཻ����
    {
        p[0]->next = p[2];
        p[1]->next = p[2];
        p[2]->next = p[3];
        p[3]->next = NULL;
    }
    else if (mode == 1) //������������Ϊ2�Ĳ��ཻ����
    {
        p[0]->next = p[2];
        p[1]->next = p[3];
        p[2]->next = p[3]->next = NULL;
    }
    *p1 = p[0];
    *p2 = p[1];
    return;
}

void judge(LIST *p1, LIST *p2)
{
    while (p1->next != NULL)
    {
        p1 = p1->next;
    }
    while (p2->next != NULL)
    {
        p2 = p2->next;
    }
    if (p1==p2)
    {
        cout << "���������ཻ" << endl;
    }
    else
    {
        cout << "���������ཻ" << endl;
    }
    return;
}

int main()
{
    LIST *intersect1, *intersect2, *indie1, *indie2;
    build(&intersect1, &intersect2, 0);
    build(&indie1, &indie2, 1);
    judge(intersect1, intersect2);
    judge(indie1, indie2);
    system("pause");
    return 0;
}

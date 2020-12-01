/*
思考题 1:
已知两个单链表，判断是否相交？（BAT公司面试题）
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
    if (mode == 0) //创建两个长度为3的相交链表
    {
        p[0]->next = p[2];
        p[1]->next = p[2];
        p[2]->next = p[3];
        p[3]->next = NULL;
    }
    else if (mode == 1) //创建两个长度为2的不相交链表
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
        cout << "两个链表相交" << endl;
    }
    else
    {
        cout << "两个链表不相交" << endl;
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

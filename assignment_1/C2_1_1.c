/*
1190201520 李想

作业 1
1：已知一个单链表，求倒数第k个元素。求中点元素。
算法1： 1) 找出长度n   2) n-k+1
算法2：扫描一次 从左到右，设置两个变量
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list
{
    int value;
    struct list *next;
} LIST;

int v = 0;

void input(int *len, int *index)
{
    printf("Input length of the list:");
    while (!scanf("%d", len))
    {
        printf("invalid input, please input again:");
        while (getchar() != '\n')
            ;
    }
    printf("find index = ");
    while (!scanf("%d", index) || *index <=0 || *index > *len)
    {
        printf("invalid input, please input again:");
        while (getchar() != '\n')
            ;
    }
}

LIST *build(int len)
{
    LIST *newPt, *prePt, *head;
    newPt = (LIST *)malloc(sizeof(LIST));
    if (newPt != NULL)
    {
        newPt->value = v++;
        head = newPt;
        prePt = newPt;
        newPt->next = NULL;
    }
    else
    {
        printf("Failed");
        exit(0);
    }
    for (int i = 1; i < len; i++)
    {
        newPt = (LIST *)malloc(sizeof(LIST));
        if (newPt != NULL)
        {
            newPt->value = v++;
            prePt->next = newPt;
            prePt = newPt;
        }
        else
        {
            printf("Failed");
            exit(0);
        }
        newPt->next = NULL;
    }
    return head;
}

//算法一
void search1(LIST *head, int index)
{
    //假装不知道链表长度
    int len = 0;
    LIST *pt = head;
    while (pt->next != NULL)
    {
        pt = pt->next;
        len++;
    }
    pt = head;
    index = len - index + 1;
    while (index > 0)
    {
        pt = pt->next;
        index--;
    }
    printf("%d\n", pt->value);
}

//算法2
void search2(LIST *head, int index)
{
    LIST *p1 = head;
    LIST *p2 = head;
    for (int i = 0; i < index - 1; i++)
    {
        p2 = p2->next;
    }
    while (p2->next != NULL)
    {
        p1 = p1->next;
        p2 = p2->next;
    }
    printf("%d\n", p1->value);
}

//寻找中点元素
void findmid(LIST *head)
{
    int len = 1;
    LIST *pt = head;
    while (pt->next != NULL)
    {
        pt = pt->next;
        len++;
    }
    if (len % 2 == 0)
    {
        search2(head, len / 2 + 1);
        search2(head, len / 2);
    }
    else
    {
        search2(head, len / 2 + 1);
    }
}

int main()
{
    int len = 0, index = 0;
    input(&len, &index);
    LIST *head = build(len);
    printf("method 1: ");
    search1(head, index);
    printf("method 2: ");
    search2(head, index);
    printf("mid value: \n");
    findmid(head);
    system("pause");
    return 0;
}
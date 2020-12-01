/*
1190201520 李想

作业 1
2：单链表逆置问题
方法很简单，采用生成单链表算法中的头插法思想就可以实现！
1，首先将第一个结点和其余结点断开；
2、然后将剩下的结点依次取下来，始终插入到第一个结点之后，就行了
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct list
{
    int value;
    struct list *next;
} LIST;

int v = 0;

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

void input(int *len)
{
    printf("Input length of the list:");
    while (!scanf("%d", len) || *len <=0 )
    {
        printf("invalid input, please input again:");
        while (getchar() != '\n')
            ;
    }
}

void print_list(LIST *head)
{
    LIST *pt = head;
    do
    {
        printf("%d\t", pt->value);
        pt = pt->next;
    } while (pt != NULL);
    printf("\n");
    return;
}

LIST *reverse(LIST *head)
{
    if (head->next == NULL)  //若长度为1则直接返回
        return head;
    LIST *pt = head->next;
    LIST *nextpt = NULL;
    head->next = NULL;
    do
    {
        nextpt = pt->next;
        pt->next = head;
        head = pt;
        pt = nextpt;
    } while (pt != NULL);
    return head;
}

int main()
{
    int len = 0;
    input(&len);
    LIST *head = build(len);
    printf("original list:\n");
    print_list(head);
    head = reverse(head);
    printf("reversed list:\n");
    print_list(head);
    system("pause");
    return 0;
}

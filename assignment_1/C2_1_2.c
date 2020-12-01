/*
1190201520 ����

��ҵ 1
2����������������
�����ܼ򵥣��������ɵ������㷨�е�ͷ�巨˼��Ϳ���ʵ�֣�
1�����Ƚ���һ������������Ͽ���
2��Ȼ��ʣ�µĽ������ȡ������ʼ�ղ��뵽��һ�����֮�󣬾�����
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
    if (head->next == NULL)  //������Ϊ1��ֱ�ӷ���
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

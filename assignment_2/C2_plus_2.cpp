/*
�ж������Ƿ��л��� ���У��ҳ�����ڽ�㣿
*/

#include <iostream>
using namespace std;

typedef struct list
{
    struct list *next;
} LIST;

typedef struct ptlist
{
    int num;
    struct list *pt = NULL;
    struct ptlist *next;
} PTLIST;

LIST *build(int mode)
{
    LIST *p[5];
    for (int i = 0; i < 5; i++)
    {
        p[i] = (LIST *)malloc(sizeof(LIST));
        if (i > 0)
        {
            p[i - 1]->next = p[i];
        }
        if (i == 4)
            p[i]->next = NULL;
    }
    if (mode == 0)
        p[4]->next = p[1];
    return p[0];
}

void judge(LIST *head)
{
    if (head == NULL)
    {
        cout << "������Ϊ�գ�" << endl;
        return;
    }
    LIST *cur_node = head;
    PTLIST *pthead = (PTLIST *)malloc(sizeof(PTLIST));
    PTLIST *cur_pt = pthead;
    pthead->next = NULL;
    int n = 0;
    do
    {
        cur_pt->num = n++;
        cur_pt->pt = cur_node;

        PTLIST *history = pthead;
        for (int i = 0; i < n - 1; i++)
        {
            if (history->pt == cur_pt->pt)
            {
                cout << "�������л�" << endl;
                cout << "�������Ϊ��" << history->num << "���ڵ�" << endl;
                cout << "��ڵ�ַΪ" << history->pt << endl;
                return;
            }
            history = history->next;
        }
        //�ӳ�ptlist���� ���ƶ�����һ���ڵ�
        PTLIST *new_pt = (PTLIST *)malloc(sizeof(PTLIST));
        new_pt->next = NULL;
        cur_pt->next = new_pt;
        cur_pt = new_pt;
        cur_node = cur_node->next;
    } while (cur_node->next != NULL);
    cout << "������û�л�" << endl;
    return;
}

int main()
{
    LIST *loop = build(0);
    LIST *noloop = build(1);
    judge(loop);
    cout << endl;
    judge(noloop);
    cout << endl;
    system("pause");
    return 0;
}
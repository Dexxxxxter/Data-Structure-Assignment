/*
1190201520 ����
���ݴ洢��ͬĿ¼��Data.txt�ļ���


��ҵ2�������ά�����ļ���ʽ�ı��� 

Ҫ��
������ṹ��������ʾĳ�̳��ҵ�Ŀ��ģ�͡�
������������ʱ ��Ҫ�Ը��������ά����
ÿ�������ս���֮�󣬽��������е��������ļ���ʽ���棬ÿ�տ�ʼӪҵ֮ǰ���轫���ļ���ʽ��������ݻָ�������ṹ�������
�����������������ҵ����ơ�Ʒ�ơ����ۺ��������Ե��۵�������������������ԡ�

�����ܰ�����
������Ӫҵ��ʼ�������ļ��ָ� �������ݣ������������룩����������»�ɾ��������ѯ��Ϣ��������Ϣ��Ӫҵ�������������ݴ����ļ����ȡ�
*/


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define BUFSIZE 100

typedef struct record
{
    char name[100];
    char brand[10];
    float price;
    int num;
    struct record *next;
} RECORD;

int r_n = 0;  //��¼����

//���ƹ��λ��
void gotoxy(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ��׼����豸���
    SetConsoleCursorPosition(hOut, pos);
}

//����ڴ����
void Check_pt(void *p)
{
    if (p == NULL)
    {
        //��ӡ������ʾ���˳�
        system("cls");
        gotoxy(15, 15);
        printf("�ڴ������ִ���");
        gotoxy(15, 17);
        printf("��������˳�");
        _getch();
        exit(1);
    }
}

//�������������
void BubbleSort(RECORD *head)
{
    RECORD * p, * q, * tail;

    tail = NULL;

    while((head->next->next) != tail)
    {
        p = head;
        q = head->next;
        while(q->next != tail)
        {
            if((q->price) > (q->next->price))
            {
                p->next = q->next;
                q->next = q->next->next;
                p->next->next = q;
                q = p->next;
            }
            q = q->next;
            p = p->next;
        }
        tail = q;
    }
    return;
}

//�����ļ���ת���������ٽ�������
RECORD *Read()
{
    system("cls");
    char BUF[BUFSIZE], c;
    FILE *fp = fopen("Data.txt", "a+");
    Check_pt(fp);
    rewind(fp); //a+�򿪺�ָ�����ļ�ĩβ����rewind��λ
    while (!feof(fp))
    {
        c = fgetc(fp);
        if (c == '\n')
            r_n++;
    }
    rewind(fp);   //��ָ�븴λ���ٽ�����һ���Ķ���
    RECORD *head = (RECORD *)malloc(sizeof(RECORD)); //headΪ��
    RECORD *prept = head;
    for (int i = 0; i < r_n; i++)
    {
        RECORD *newNode = (RECORD *)malloc(sizeof(RECORD));
        Check_pt(newNode);
        fgets(BUF, BUFSIZE, fp); //���û����ַ������룬��ת�����ṹ��������
        sscanf(BUF, "%s %s %f %d", newNode->name, newNode->brand, &newNode->price, &newNode->num);
        newNode->next = NULL;
        prept->next = newNode;
        prept = newNode;
    }
    fclose(fp);
    BubbleSort(head);
    return head;
}

//�����¼�¼
void Insert(RECORD *head)
{
    system("cls");
    int flag = 0;
    RECORD *pt = head->next;
    RECORD *prept = head;
    RECORD *newNode = (RECORD *)malloc(sizeof(RECORD));
    Check_pt(newNode);
    printf("������������Ʒ���ơ�Ʒ�ơ��۸�Ϳ�������õ����ո�ָ���\n");
    do
    {
        flag = scanf("%s %s %f %d", newNode->name, newNode->brand, &newNode->price, &newNode->num);
        while (getchar() != '\n')
            ;
        if (flag != 4)
            printf("������������������\n");
    } while (flag != 4);
    //����Ƿ��ظ�
    while (pt!=NULL)
    {
        if (strcmp(pt->name, newNode->name)==0)
        {
            printf("������������Ŀ�ظ���\n");
            _getch();
            return;
        }
        pt = pt->next;
    }
    pt = head->next;
    while ((pt->price < newNode->price) && pt != NULL)
    {
        pt = pt->next;
        prept = prept->next;
    }
    prept->next = newNode;
    newNode->next = pt;
    printf("¼��ɹ�\n");
    _getch();
    return;
}

//ɾ��ĳ����¼
void Delete(RECORD *head)
{
    system("cls");
    int flag = 0;
    RECORD *pt = head->next;
    RECORD *prept = head;
    char name[100];
    printf("������Ҫɾ������Ʒ����\n");
    do
    {
        flag = scanf("%s", name);
        while (getchar() != '\n')
            ;
        if (!flag)
            printf("������������������\n");
    } while (!flag);
    while (strcmp(pt->name, name) != 0)
    {
        pt = pt->next;
        prept = prept->next;
        if (pt == NULL)
        {
            printf("û���ҵ���Ӧ��Ʒ��Ŀ\n");
            _getch();
            return;
        }
    }
    prept->next = pt->next;
    free(pt);
    printf("ɾ���ɹ�\n");
    _getch();
    return;
}

//�޸�ĳ����¼
void Modify(RECORD *head)
{
    system("cls");
    int flag = 0;
    RECORD *pt = head->next;
    RECORD *temp = (RECORD *)malloc(sizeof(RECORD));
    char name[100];
    printf("������Ҫ�޸ĵ���Ʒ����\n");
    do
    {
        flag = scanf("%s", name);
        while (getchar() != '\n')
            ;
        if (!flag)
            printf("������������������\n");
    } while (!flag);
    while (strcmp(pt->name, name) != 0)
    {
        pt = pt->next;
        if (pt == NULL)
        {
            printf("û���ҵ���Ӧ��Ʒ��Ŀ\n");
            _getch();
            return;
        }
    }
    printf("��Ʒ���� /  Ʒ��  /  �۸�  /  ���\n");
    printf("%s  %s     %.2f    %d\n", (*pt).name, (*pt).brand, pt->price, pt->num);
    printf("������������º����Ʒ���ơ�Ʒ�ơ��۸�Ϳ�������õ����ո�ָ���\n");
    do
    {
        flag = scanf("%s %s %f %d", temp->name, temp->brand, &temp->price, &temp->num);
        while (getchar() != '\n')
            ;
        if (flag != 4)
            printf("������������������\n");
    } while (flag != 4);
    temp->next = pt->next;
    memcpy(pt, temp, sizeof(RECORD));
    free(temp);
    BubbleSort(head);
    printf("�޸ĳɹ�\n");
    _getch();
    return;
}

//��ʾ���м�¼
void Print_all(RECORD *head)
{
    gotoxy(0, 0);
    system("cls");
    RECORD *pt = head->next;
    int l = 0;
    gotoxy(20, 8);
    printf("��Ʒ����                Ʒ��      �۸�      ���\n");
    while (pt != NULL)
    {
        gotoxy(10, 12 + l);
        printf("%s", (*pt).name);
        gotoxy(44, 12 + l);
        printf("%s", (*pt).brand);
        gotoxy(53, 12 + l);
        printf("%.2f", pt->price);
        gotoxy(65, 12 + l);
        printf("%d", pt->num);
        l += 2;
        pt = pt->next;
    }
    _getch();
    system("cls");
    return;
}

//���Ҽ�¼
void Search(RECORD *head)
{
    system("cls");
    int type = 0, flag = 0;
    char words[100];
    char *pos = NULL;
    RECORD *pt = head->next;
    printf("������Ҫ��ѯ�Ĺؼ������1-��Ʒ���ƣ�2-Ʒ��\n");
    do
    {
        flag = scanf("%d", &type);
        while (getchar() != '\n')
            ;
    } while (flag != 1 || (type != 1 && type != 2));
    printf("������Ҫ��ѯ�Ĺؼ��֣�\n");
    while (!scanf("%s", words))
    {
        while (getchar() != '\n')
            ;
        printf("������������������\n");
    }
    printf("\n��Ʒ����                  Ʒ��     �۸�     ���\n");
    if (type == 1)
    {
        while (pt != NULL)
        {
            //pos = issubstr(pt->name, words);
            pos = strstr(pt->name, words);
            if (pos != NULL)
                printf("%s     %s     %.2f    %d\n", (*pt).name, (*pt).brand, pt->price, pt->num);
            pt = pt->next;
        }
    }
    else
    {
        while (pt != NULL)
        {
            pos = strstr(pt->brand, words);
            if (pos != NULL)
                printf("%s  %s     %.2f    %d\n", (*pt).name, (*pt).brand, pt->price, pt->num);
            pt = pt->next;
        }
    }
    printf("\n�������\n");
    printf("\n-�����������-");
    _getch();
    return;
}

//��������Ϊ�ļ����˳�
void Save(RECORD *head)
{
    system("cls");
    FILE *fp = fopen("Data_tmp.txt", "a+");
    RECORD *pt = head->next;
    Check_pt(fp);
    while (pt != NULL)
    {
        fprintf(fp, "%s %s %.2f %d\n", (*pt).name, (*pt).brand, pt->price, pt->num);
        pt = pt->next;
    }
    fclose(fp);
    remove("Data.txt");
    rename("Data_tmp.txt", "Data.txt");
    gotoxy(9, 10);
    printf("����ɹ���");
    gotoxy(5, 12);
    printf("--��������˳�--");
    _getch();
    exit(0);
}

//��ҳ�棬ѡ����
void Menu_screen(RECORD *head)
{
    int selection = 0, flag = 0;
    while (1)
    {
        system("cls");
        gotoxy(20, 5);
        printf("|----------------------------------------------------------|");
        gotoxy(28, 7);
        printf(">> �ҵ������ϵͳ <<");
        gotoxy(27, 12);
        printf("1. �� �� �� ѯ");
        gotoxy(27, 14);
        printf("2. �� �� �� ¼");
        gotoxy(27, 16);
        printf("3. ɾ �� �� ¼");
        gotoxy(27, 18);
        printf("4. �� �� �� ¼");
        gotoxy(27, 20);
        printf("5. �� �� �� �� ��");
        gotoxy(27, 22);
        printf("6. �� �� �� �� �� ��");
        gotoxy(20, 32);
        printf("|----------------------------------------------------------|");
        gotoxy(25, 26);
        printf("ѡ �� �� ��");
        gotoxy(25, 27);
        printf("�� �� �� �� (1 / 2 / 3 / 4 / 5 / 6) : ");
        flag = 0;
        do
        {
            flag = scanf("%d", &selection);
            while (getchar() != '\n')
                ;
        } while (!flag || selection < 1 || selection > 6);

        switch (selection)
        {
        case 1:
            Search(head);
            break;
        case 2:
            Insert(head);
            break;
        case 3:
            Delete(head);
            break;
        case 4:
            Modify(head);
            break;
        case 5:
            Save(head);
            break;
        case 6:
            Print_all(head);
            break;
        }
        system("cls");
    }
    return;
}

//�������
int main()
{
    system("mode con cols=100 lines=35");
    RECORD *head = Read();
    Menu_screen(head);
    return 0;
}
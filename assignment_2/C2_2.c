/*
1190201520 李想
数据存储在同目录下Data.txt文件中


作业2：链表的维护与文件形式的保存 

要求：
用链表结构的有序表表示某商场家电的库存模型。
当有提货或进货时 需要对该链表进行维护。
每个工作日结束之后，将该链表中的数据以文件形式保存，每日开始营业之前，需将以文件形式保存的数据恢复成链表结构的有序表。
链表结点的数据域包括家电名称、品牌、单价和数量，以单价的升序体现链表的有序性。

程序功能包括：
创建表、营业开始（读入文件恢复 链表数据）、进货（插入）、提货（更新或删除）、查询信息、更新信息、营业结束（链表数据存入文件）等。
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

int r_n = 0;  //记录条数

//控制光标位置
void gotoxy(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); //获取标准输出设备句柄
    SetConsoleCursorPosition(hOut, pos);
}

//检查内存分配
void Check_pt(void *p)
{
    if (p == NULL)
    {
        //打印错误提示并退出
        system("cls");
        gotoxy(15, 15);
        printf("内存分配出现错误");
        gotoxy(15, 17);
        printf("按任意键退出");
        _getch();
        exit(1);
    }
}

//对链表进行排序
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

//读入文件并转换成链表，再进行排序
RECORD *Read()
{
    system("cls");
    char BUF[BUFSIZE], c;
    FILE *fp = fopen("Data.txt", "a+");
    Check_pt(fp);
    rewind(fp); //a+打开后指针在文件末尾，用rewind复位
    while (!feof(fp))
    {
        c = fgetc(fp);
        if (c == '\n')
            r_n++;
    }
    rewind(fp);   //将指针复位后再进行下一步的读入
    RECORD *head = (RECORD *)malloc(sizeof(RECORD)); //head为空
    RECORD *prept = head;
    for (int i = 0; i < r_n; i++)
    {
        RECORD *newNode = (RECORD *)malloc(sizeof(RECORD));
        Check_pt(newNode);
        fgets(BUF, BUFSIZE, fp); //先用缓冲字符串读入，再转移至结构体数组中
        sscanf(BUF, "%s %s %f %d", newNode->name, newNode->brand, &newNode->price, &newNode->num);
        newNode->next = NULL;
        prept->next = newNode;
        prept = newNode;
    }
    fclose(fp);
    BubbleSort(head);
    return head;
}

//插入新记录
void Insert(RECORD *head)
{
    system("cls");
    int flag = 0;
    RECORD *pt = head->next;
    RECORD *prept = head;
    RECORD *newNode = (RECORD *)malloc(sizeof(RECORD));
    Check_pt(newNode);
    printf("请依次输入商品名称、品牌、价格和库存量（用单个空格分隔）\n");
    do
    {
        flag = scanf("%s %s %f %d", newNode->name, newNode->brand, &newNode->price, &newNode->num);
        while (getchar() != '\n')
            ;
        if (flag != 4)
            printf("输入有误，请重新输入\n");
    } while (flag != 4);
    //检查是否重复
    while (pt!=NULL)
    {
        if (strcmp(pt->name, newNode->name)==0)
        {
            printf("名称与已有条目重复！\n");
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
    printf("录入成功\n");
    _getch();
    return;
}

//删除某条记录
void Delete(RECORD *head)
{
    system("cls");
    int flag = 0;
    RECORD *pt = head->next;
    RECORD *prept = head;
    char name[100];
    printf("请输入要删除的商品名称\n");
    do
    {
        flag = scanf("%s", name);
        while (getchar() != '\n')
            ;
        if (!flag)
            printf("输入有误，请重新输入\n");
    } while (!flag);
    while (strcmp(pt->name, name) != 0)
    {
        pt = pt->next;
        prept = prept->next;
        if (pt == NULL)
        {
            printf("没有找到对应商品条目\n");
            _getch();
            return;
        }
    }
    prept->next = pt->next;
    free(pt);
    printf("删除成功\n");
    _getch();
    return;
}

//修改某条记录
void Modify(RECORD *head)
{
    system("cls");
    int flag = 0;
    RECORD *pt = head->next;
    RECORD *temp = (RECORD *)malloc(sizeof(RECORD));
    char name[100];
    printf("请输入要修改的商品名称\n");
    do
    {
        flag = scanf("%s", name);
        while (getchar() != '\n')
            ;
        if (!flag)
            printf("输入有误，请重新输入\n");
    } while (!flag);
    while (strcmp(pt->name, name) != 0)
    {
        pt = pt->next;
        if (pt == NULL)
        {
            printf("没有找到对应商品条目\n");
            _getch();
            return;
        }
    }
    printf("商品名称 /  品牌  /  价格  /  库存\n");
    printf("%s  %s     %.2f    %d\n", (*pt).name, (*pt).brand, pt->price, pt->num);
    printf("请依次输入更新后的商品名称、品牌、价格和库存量（用单个空格分隔）\n");
    do
    {
        flag = scanf("%s %s %f %d", temp->name, temp->brand, &temp->price, &temp->num);
        while (getchar() != '\n')
            ;
        if (flag != 4)
            printf("输入有误，请重新输入\n");
    } while (flag != 4);
    temp->next = pt->next;
    memcpy(pt, temp, sizeof(RECORD));
    free(temp);
    BubbleSort(head);
    printf("修改成功\n");
    _getch();
    return;
}

//显示所有记录
void Print_all(RECORD *head)
{
    gotoxy(0, 0);
    system("cls");
    RECORD *pt = head->next;
    int l = 0;
    gotoxy(20, 8);
    printf("商品名称                品牌      价格      库存\n");
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

//查找记录
void Search(RECORD *head)
{
    system("cls");
    int type = 0, flag = 0;
    char words[100];
    char *pos = NULL;
    RECORD *pt = head->next;
    printf("请输入要查询的关键字类别：1-商品名称，2-品牌\n");
    do
    {
        flag = scanf("%d", &type);
        while (getchar() != '\n')
            ;
    } while (flag != 1 || (type != 1 && type != 2));
    printf("请输入要查询的关键字：\n");
    while (!scanf("%s", words))
    {
        while (getchar() != '\n')
            ;
        printf("输入有误，请重新输入\n");
    }
    printf("\n商品名称                  品牌     价格     库存\n");
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
    printf("\n搜索完毕\n");
    printf("\n-按任意键返回-");
    _getch();
    return;
}

//将链表保存为文件并退出
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
    printf("保存成功！");
    gotoxy(5, 12);
    printf("--按任意键退出--");
    _getch();
    exit(0);
}

//主页面，选择功能
void Menu_screen(RECORD *head)
{
    int selection = 0, flag = 0;
    while (1)
    {
        system("cls");
        gotoxy(20, 5);
        printf("|----------------------------------------------------------|");
        gotoxy(28, 7);
        printf(">> 家电库存管理系统 <<");
        gotoxy(27, 12);
        printf("1. 库 存 查 询");
        gotoxy(27, 14);
        printf("2. 插 入 记 录");
        gotoxy(27, 16);
        printf("3. 删 除 记 录");
        gotoxy(27, 18);
        printf("4. 修 改 记 录");
        gotoxy(27, 20);
        printf("5. 保 存 并 退 出");
        gotoxy(27, 22);
        printf("6. 查 看 所 有 库 存");
        gotoxy(20, 32);
        printf("|----------------------------------------------------------|");
        gotoxy(25, 26);
        printf("选 择 操 作");
        gotoxy(25, 27);
        printf("输 入 数 字 (1 / 2 / 3 / 4 / 5 / 6) : ");
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

//程序入口
int main()
{
    system("mode con cols=100 lines=35");
    RECORD *head = Read();
    Menu_screen(head);
    return 0;
}
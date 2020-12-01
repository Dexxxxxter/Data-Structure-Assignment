/*
使用单向链表实现栈
链表头为栈顶
*/

#ifndef mystack_h
#define mystack_h

#include <iostream>
#include <cstdlib>

typedef struct path_stack
{
    int i; //x pos
    int j; //y pos
    int v; //direction
    struct path_stack *next;
} STACK;

void check_stack(STACK *head)
{
    if (head == NULL)
    {
        std::cout << "NULL STACK ERROR" << std::endl;
        system("pause");
        exit(-1);
    }
}

// 初始化一个栈，栈顶为空
STACK *init_path_stack()
{
    STACK *head = (STACK *)malloc(sizeof(STACK));
    check_stack(head);
    head->next = NULL;
    return head;
}

// 压栈
void push(STACK p, STACK *head)
{
    STACK *temp = (STACK *)malloc(sizeof(STACK));
    *temp = p;
    check_stack(head); 
    temp->next = head->next;
    head->next = temp;
    return;
}

// 弹栈
void pop(STACK *head)
{
    check_stack(head);
    STACK *pt = head->next;
    head->next = pt->next;
    free(pt);
    return;
}

// 返回栈顶元素
STACK top(STACK *head)
{
    check_stack(head);
    return *(head->next);
}

// 判断栈是否为空，若空返回true
int isempty(STACK *head)
{
    check_stack(head);
    if (head->next == NULL)
        return 1;
    else
        return 0;
}

// 将栈正向输出
void stack_output(STACK *head)
{
    check_stack(head);
    STACK *p = head->next;
    while (p != NULL)
    {
        std::cout << p->i << "  " << p->j << std::endl;
        p = p->next;
    }
    return;
}

// 将栈反向输出
void reversed_output(STACK *head)
{
    STACK *p, *pend = NULL;
    check_stack(head);
    if (head->next == NULL)
        std::cout << "the stack is empty!" << std::endl;
    while (pend != head->next)
    {
        for (p = head->next; p->next != pend; p = p->next)
            ;
        std::cout << p->i << "  " << p->j << std::endl;
        pend = p;
    }
    return;
}

// void reversed_output2(STACK *head)
// {
//     check_stack(head);
//     STACK *pt = head->next;
//     STACK *rev = init_path_stack();
//     while(pt!=NULL)
//     {
//         push(*pt, rev);
//         pt = pt->next;
//     }
//     stack_output(rev);
// }

#endif
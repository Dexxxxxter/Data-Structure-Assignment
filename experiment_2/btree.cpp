/*
ʵ��� ���ͽṹ��Ӧ��

1����д����������(����10�����) �Ķ�������洢�ṹ����������ʾ���ĳ��򣬲����ʵ�����ʽ��ʾ�ͱ����������
2�����ö������Ķ�������洢�ṹ��
  ��д����ʵ�ֶ���������������ͺ�������ĵݹ�ͷǵݹ��㷨�Լ���������㷨��
  �����ʵ�����ʽ��ʾ�ͱ��������������Ӧ�ı������У� 
3������һ���������� ��д�㷨�������Ӧ��: 
    ��1���ж����Ƿ�Ϊ��ȫ��������
    ��2����������������������Ĺ������ȡ�
*/

#include <iostream>
#include <stack>
#include <string>
#include <queue>

using namespace std;

typedef struct tree_node
{
    int data;
    struct tree_node *parent;
    struct tree_node *lchild;
    struct tree_node *rchild;
} * Btree, Node;

//�ǵݹ�������ר�ýṹ��
typedef struct postorder_tree_node
{
    Btree node;
    int flag;
} * Btree_post, Node_post;

Btree CreateBT(Btree parent)
{
    Btree T;
    char ch;
    cin >> ch;
    if (ch == '#')
        T = NULL;
    else
    {
        T = new Node;
        T->parent = parent;
        T->data = ch - '0';
        T->lchild = CreateBT(T);
        T->rchild = CreateBT(T);
    }
    return (T);
}

//������Ƿ�Ϊ��
bool Check_Btree(Btree root)
{
    if (root == NULL)
    {
        cout << "��Ϊ�գ�";
        return true;
    }
    else
    {
        return false;
    }
}

//�ֶ���������
Btree NewTree()
{
    Btree node[9];
    for (int i = 0; i < 9; i++)
    {
        node[i] = (Btree)malloc(sizeof(Node));
        node[i]->data = i;
        node[i]->parent = node[i]->lchild = node[i]->rchild = NULL;
    }
    node[0]->lchild = node[1];
    node[0]->rchild = node[2];
    node[1]->parent = node[2]->parent = node[0];
    node[1]->lchild = node[3];
    node[1]->rchild = node[4];
    node[3]->parent = node[4]->parent = node[1];
    node[4]->lchild = node[7];
    node[7]->parent = node[4];
    node[2]->lchild = node[5];
    node[2]->rchild = node[6];
    node[5]->parent = node[6]->parent = node[2];
    node[5]->rchild = node[8];
    node[8]->parent = node[5];
    return node[0];
}

//�ֶ�������ȫ������
Btree NewTree_complete()
{
    Btree node[7];
    for (int i = 0; i < 7; i++)
    {
        node[i] = (Btree)malloc(sizeof(Node));
        node[i]->data = i;
        node[i]->lchild = node[i]->rchild = NULL;
    }
    node[0]->lchild = node[1];
    node[0]->rchild = node[2];
    node[1]->parent = node[2]->parent = node[0];
    node[1]->lchild = node[3];
    node[1]->rchild = node[4];
    node[3]->parent = node[4]->parent = node[1];
    node[2]->lchild = node[5];
    node[2]->rchild = node[6];
    node[5]->parent = node[6]->parent = node[2];
    return node[0];
}

//�������-�ݹ����
string PreOrder_recursive_core(Btree root)
{
    string s;
    if (root != NULL)
    {
        //cout << root->data;
        s.append(to_string(root->data) + "  ");
        s.append(PreOrder_recursive_core(root->lchild));
        s.append(PreOrder_recursive_core(root->rchild));
    }
    return s;
}

//�������-�ݹ�
void PreOrder_recursive(Btree root)
{
    if (Check_Btree(root))
        return;
    cout << endl
         << "�������-�ݹ�: " << endl;
    cout << PreOrder_recursive_core(root) << endl;
    return;
}

//�������-�ݹ����
string InOrder_recursive_core(Btree root)
{
    string s;
    if (root != NULL)
    {
        s.append(InOrder_recursive_core(root->lchild));
        s.append(to_string(root->data) + "  ");
        s.append(InOrder_recursive_core(root->rchild));
    }
    return s;
}

//�������-�ݹ�
void InOrder_recursive(Btree root)
{
    if (Check_Btree(root))
        return;
    cout << endl
         << "�������-�ݹ�: " << endl;
    cout << InOrder_recursive_core(root) << endl;
    return;
}

//�������-�ݹ����
string PostOrder_recursive_core(Btree root)
{
    string s;
    if (root != NULL)
    {
        s.append(PostOrder_recursive_core(root->lchild));
        s.append(PostOrder_recursive_core(root->rchild));
        s.append(to_string(root->data) + "  ");
    }
    return s;
}

//�������-�ݹ�
void PostOrder_recursive(Btree root)
{
    if (Check_Btree(root))
        return;
    cout << endl
         << "�������-�ݹ�: " << endl;
    cout << PostOrder_recursive_core(root) << endl;
    return;
}

//����������ǵݹ�
void PreOrder_nonrecursive(Btree root)
{
    stack<Btree> S;
    if (Check_Btree(root))
        return;
    cout << endl
         << "����������ǵݹ�: " << endl;
    while (root != NULL || !S.empty()) //ѭ��ֱ��rootΪ����ջsΪ��
    {
        while (root != NULL) //��root����ʱѭ��
        {
            cout << root->data << "  "; //���root->data
            S.push(root);               ////��ָ��root��ֵ���浽ջ��
            root = root->lchild;        //��������root��������
        }
        if (!S.empty()) //���ջs����
        {
            root = S.top(); //��ջ��Ԫ�ص�����root
            S.pop();
            root = root->rchild; //׼������root��������
        }
    }
    cout << endl;
    return;
}

//�������-�ǵݹ�
void InOrder_nonrecursive(Btree root)
{
    stack<Btree> S;
    if (Check_Btree(root))
        return;
    cout << endl
         << "�������-�ǵݹ�: " << endl;
    while (root != NULL || !S.empty()) //ѭ��ֱ��rootΪ����ջsΪ��
    {
        while (root != NULL) //��root����ʱѭ��
        {
            S.push(root);        //��ָ��root��ֵ���浽ջ��
            root = root->lchild; //��������root��������
        }
        if (!S.empty()) //���ջs����
        {
            root = S.top(); //��ջ��Ԫ�ص�����root
            S.pop();
            cout << root->data << "  "; //���root->data
            root = root->rchild;        //׼������root��������
        }
    }
    cout << endl;
    return;
}

//�������-�ǵݹ�
void PostOrder_nonrecursive(Btree root)
{
    stack<Btree_post> S;
    if (Check_Btree(root))
        return;
    cout << endl
         << "�������-�ǵݹ�: " << endl;
    while (root != NULL || !S.empty())
    {
        while (root != NULL)
        {
            Btree_post node_post = new Node_post;
            node_post->node = root;
            node_post->flag = 0;
            S.push(node_post);
            root = root->lchild;
        }
        if (!S.empty())
        {
            if (S.top()->flag == 1)
            {
                Btree_post tmp = S.top();
                cout << tmp->node->data << "  ";
                S.pop();
                delete tmp;
            }
            else
            {
                S.top()->flag = 1;
                root = S.top()->node->rchild;
            }
        }
    }
    cout << endl;
    return;
}

//�������
void LayerOrder(Btree root)
{
    queue<Btree> Q;
    if (Check_Btree(root))
        return;
    cout << endl
         << "�������: " << endl;
    Q.push(root);
    while (!Q.empty())
    {
        root = Q.front();
        Q.pop();
        cout << root->data << "  ";
        if (root->lchild != NULL)
            Q.push(root->lchild);
        if (root->rchild != NULL)
            Q.push(root->rchild);
    }
    cout << endl;
    return;
}

//��ӡ������
void Print_Btree(Btree root, string prefix, string childrenprefix)
{
    //prefixȡ���ڸ��ڵ�����
    if (root == NULL)
    {
        return;
    }
    cout << prefix << root->data << endl;
    if (root->rchild != NULL)
    {
        if (root->lchild != NULL)
        {
            prefix = "�� R�� ";
        }
        else
        {
            prefix = "�� R�� ";
        }
        Print_Btree(root->rchild, childrenprefix + prefix, childrenprefix + "��    ");
    }
    if (root->lchild != NULL)
    {
        prefix = "�� L�� ";
        Print_Btree(root->lchild, childrenprefix + prefix, childrenprefix + "     ");
    }
    return;
}

//�ж��Ƿ�Ϊ��ȫ������
bool Is_Complete_Btree(Btree root)
{
    queue<Btree> Q;
    int flag = 0;
    Q.push(root);
    while (!Q.empty())
    {
        root = Q.front();
        Q.pop();
        if (flag == 1)
        {
            if (root->lchild == NULL && root->rchild == NULL)
                continue;
            else
            {
                return false;
            }
        }
        if (root->lchild != NULL && root->rchild != NULL)
        {
            Q.push(root->lchild);
            Q.push(root->rchild);
        }
        else if (root->lchild == NULL && root->rchild != NULL)
        {
            return false;
        }
        else if (root->lchild != NULL && root->rchild == NULL)
        {
            flag = 1;
        }
    }
    return true;
}

//����Ƿ�Ϊ��ȫ������
void Judge_Complete_Btree(Btree root)
{
    string res;
    if (Check_Btree(root))
        return;
    if (Is_Complete_Btree(root))
        res = "��";
    else
        res = "����";
    cout << endl
         << "�˶�����" + res + "��ȫ������" << endl;
    return;
}

Btree Get_Last_Common_Ancestor(Btree root, int n1, int n2)
{
    if (root == NULL)
        return NULL; //����Ҷ�ӽڵ� - û���ҵ�
    if (root->data == n1 || root->data == n2)
        return root; //�ҵ� - ���ظ��ڵ�
    //��ǰ�ڵ��Ҳ��� - ������������
    Btree left = Get_Last_Common_Ancestor(root->lchild, n1, n2);
    Btree right = Get_Last_Common_Ancestor(root->rchild, n1, n2);
    if (left && right) //����ֱ��������������򷵻ص�ǰ���ڵ�
    {
        return root;
    }
    else if (left == NULL) //�������������
    {
        return right;
    }
    else if (right == NULL) //�������������
    {
        return left;
    }
}

//Ѱ�����������ڵ�����й�ͬ����
//������������ڵ㣿��ţ�
void Find_Common_Ancestors(Btree root, int n1, int n2)
{
    // stack<Btree> s1, s2
    // if (n1 == n2)
    // {
    //     return n1;
    // }
    // else
    // {
    //     Btree_DFS(root, n1, s1);
    //     Btree_DFS(root, n2, s2);

    // }
    Btree res = Get_Last_Common_Ancestor(root, n1, n2);
    cout << endl
         << n1 << " �� " << n2 << "�����й������Ƚڵ�Ϊ: ";
    while (res)
    {
        cout << res->data << "  ";
        res = res->parent;
    }
    cout << endl;
    return;
}

// bool Btree_DFS(Btree root, int n, stack<Btree> &s)
// {
//     if(root==NULL)
//     {
//         return false;
//     }
//     s.push(root);
//     if(root->data==n)
//     {
//         return true;
//     }
//     else
//     {
//         if(Btree_DFS(root->lchild, n, s))
//             return true;
//         else if(Btree_DFS(root->rchild, n, s))
//         {
//             return true;
//         }
//         else
//         {
//             s.pop();
//             return false;
//         }
//     }
// }

void full_service(Btree root)
{
    cout << endl;
    Print_Btree(root, "", "");
    Judge_Complete_Btree(root);
    LayerOrder(root);
    PreOrder_recursive(root);
    PreOrder_nonrecursive(root);
    InOrder_recursive(root);
    InOrder_nonrecursive(root);
    PostOrder_recursive(root);
    PostOrder_nonrecursive(root);
    return;
}

int main()
{
    Btree root = NewTree();
    full_service(root);

    Btree root_complete = NewTree_complete();
    full_service(root_complete);
    Find_Common_Ancestors(root, 3, 4);

    cout << endl
         << "input a new btree:" << endl;
    Btree newroot = CreateBT(NULL);
    full_service(newroot);
    //Find_Common_Ancestors(newroot, , );

    cout << endl;
    system("pause");
    return 0;
}
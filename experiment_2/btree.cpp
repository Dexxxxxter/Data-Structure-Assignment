/*
实验二 树型结构及应用

1．编写建立二叉树(大于10个结点) 的二叉链表存储结构（左右链表示）的程序，并以适当的形式显示和保存二叉树；
2．采用二叉树的二叉链表存储结构，
  编写程序实现二叉树的先序、中序和后序遍历的递归和非递归算法以及层序遍历算法，
  并以适当的形式显示和保存二叉树及其相应的遍历序列； 
3．给定一个二叉树， 编写算法完成下列应用: 
    （1）判断其是否为完全二叉树；
    （2）求二叉树中任意两个结点的公共祖先。
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

//非递归后序遍历专用结构体
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

//检查树是否为空
bool Check_Btree(Btree root)
{
    if (root == NULL)
    {
        cout << "树为空！";
        return true;
    }
    else
    {
        return false;
    }
}

//手动创建新树
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

//手动创建完全二叉树
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

//先序遍历-递归核心
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

//先序遍历-递归
void PreOrder_recursive(Btree root)
{
    if (Check_Btree(root))
        return;
    cout << endl
         << "先序遍历-递归: " << endl;
    cout << PreOrder_recursive_core(root) << endl;
    return;
}

//中序遍历-递归核心
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

//中序遍历-递归
void InOrder_recursive(Btree root)
{
    if (Check_Btree(root))
        return;
    cout << endl
         << "中序遍历-递归: " << endl;
    cout << InOrder_recursive_core(root) << endl;
    return;
}

//后序遍历-递归核心
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

//后序遍历-递归
void PostOrder_recursive(Btree root)
{
    if (Check_Btree(root))
        return;
    cout << endl
         << "后序遍历-递归: " << endl;
    cout << PostOrder_recursive_core(root) << endl;
    return;
}

//先序遍历—非递归
void PreOrder_nonrecursive(Btree root)
{
    stack<Btree> S;
    if (Check_Btree(root))
        return;
    cout << endl
         << "先序遍历—非递归: " << endl;
    while (root != NULL || !S.empty()) //循环直到root为空且栈s为空
    {
        while (root != NULL) //当root不空时循环
        {
            cout << root->data << "  "; //输出root->data
            S.push(root);               ////将指针root的值保存到栈中
            root = root->lchild;        //继续遍历root的左子树
        }
        if (!S.empty()) //如果栈s不空
        {
            root = S.top(); //将栈顶元素弹出至root
            S.pop();
            root = root->rchild; //准备遍历root的右子树
        }
    }
    cout << endl;
    return;
}

//中序遍历-非递归
void InOrder_nonrecursive(Btree root)
{
    stack<Btree> S;
    if (Check_Btree(root))
        return;
    cout << endl
         << "中序遍历-非递归: " << endl;
    while (root != NULL || !S.empty()) //循环直到root为空且栈s为空
    {
        while (root != NULL) //当root不空时循环
        {
            S.push(root);        //将指针root的值保存到栈中
            root = root->lchild; //继续遍历root的左子树
        }
        if (!S.empty()) //如果栈s不空
        {
            root = S.top(); //将栈顶元素弹出至root
            S.pop();
            cout << root->data << "  "; //输出root->data
            root = root->rchild;        //准备遍历root的右子树
        }
    }
    cout << endl;
    return;
}

//后序遍历-非递归
void PostOrder_nonrecursive(Btree root)
{
    stack<Btree_post> S;
    if (Check_Btree(root))
        return;
    cout << endl
         << "后序遍历-非递归: " << endl;
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

//层序遍历
void LayerOrder(Btree root)
{
    queue<Btree> Q;
    if (Check_Btree(root))
        return;
    cout << endl
         << "层序遍历: " << endl;
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

//打印二叉树
void Print_Btree(Btree root, string prefix, string childrenprefix)
{
    //prefix取决于根节点的情况
    if (root == NULL)
    {
        return;
    }
    cout << prefix << root->data << endl;
    if (root->rchild != NULL)
    {
        if (root->lchild != NULL)
        {
            prefix = "├ R─ ";
        }
        else
        {
            prefix = "└ R─ ";
        }
        Print_Btree(root->rchild, childrenprefix + prefix, childrenprefix + "│    ");
    }
    if (root->lchild != NULL)
    {
        prefix = "└ L─ ";
        Print_Btree(root->lchild, childrenprefix + prefix, childrenprefix + "     ");
    }
    return;
}

//判断是否为完全二叉树
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

//输出是否为完全二叉树
void Judge_Complete_Btree(Btree root)
{
    string res;
    if (Check_Btree(root))
        return;
    if (Is_Complete_Btree(root))
        res = "是";
    else
        res = "不是";
    cout << endl
         << "此二叉树" + res + "完全二叉树" << endl;
    return;
}

Btree Get_Last_Common_Ancestor(Btree root, int n1, int n2)
{
    if (root == NULL)
        return NULL; //到达叶子节点 - 没有找到
    if (root->data == n1 || root->data == n2)
        return root; //找到 - 返回根节点
    //当前节点找不到 - 继续向下搜索
    Btree left = Get_Last_Common_Ancestor(root->lchild, n1, n2);
    Btree right = Get_Last_Common_Ancestor(root->rchild, n1, n2);
    if (left && right) //如果分别在左右子树，则返回当前根节点
    {
        return root;
    }
    else if (left == NULL) //如果都在右子树
    {
        return right;
    }
    else if (right == NULL) //如果都在左子树
    {
        return left;
    }
}

//寻找任意两个节点的所有共同祖先
//如何输入两个节点？编号？
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
         << n1 << " 和 " << n2 << "的所有公共祖先节点为: ";
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
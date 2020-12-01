#include <iostream>
#include <stack>

using namespace std;

typedef struct tree_node
{
    int data;
    struct tree_node *lchild;
    struct tree_node *rchild;
} * Btree, Node;

Btree nonrecursive_copy(Btree root)
{
    stack<Btree> S1, S2;
    Btree newroot = (Btree)malloc(sizeof(Node));
    newroot->lchild = newroot->rchild = NULL;
    Btree res = newroot;
    while (root != NULL || !S1.empty())
    {
        while (root != NULL)
        {
            newroot->data = root->data;
            S1.push(root);
            S2.push(newroot);
            if (root->lchild != NULL)
            {
                Btree newlchild = (Btree)malloc(sizeof(Node));
                newlchild->lchild = newlchild->rchild = NULL;

                newroot->lchild = newlchild;
                newlchild->data = root->lchild->data;
                newroot = newroot->lchild;
            }
            root = root->lchild;
        }
        if (!S1.empty())
        {
            root = S1.top();
            S1.pop();
            newroot = S2.top();
            S2.pop();
            if (root->rchild != NULL)
            {
                Btree newrchild = (Btree)malloc(sizeof(Node));
                newrchild->lchild = newrchild->rchild = NULL;

                newroot->rchild = newrchild;
                newrchild->data = root->rchild->data;
                newroot = newroot->rchild;
            }
            root = root->rchild;
        }
    }
    return res;
}

void PreOrder(Btree root)
{
    stack<Btree> S;
    while (root != NULL || !S.empty())
    {
        while (root != NULL)
        {
            cout << root->data << endl;
            S.push(root);
            root = root->lchild;
        }
        if (!S.empty())
        {
            root = S.top();
            S.pop();
            root = root->rchild;
        }
    }
}

Btree NewTree()
{
    Btree node[9];
    for (int i = 0; i < 9; i++)
    {
        node[i] = (Btree)malloc(sizeof(Node));
        node[i]->data = i;
        node[i]->lchild = node[i]->rchild = NULL;
    }
    node[0]->lchild = node[1];
    node[0]->rchild = node[2];
    node[1]->lchild = node[3];
    node[1]->rchild = node[4];
    node[4]->lchild = node[7];
    node[2]->lchild = node[5];
    node[2]->rchild = node[6];
    node[5]->rchild = node[8];
    return node[0];
}

int main()
{
    Btree root = NewTree();
    PreOrder(root);
    Btree newroot = nonrecursive_copy(root);
    cout << endl;
    PreOrder(newroot);
    system("pause");
    return 0;
}
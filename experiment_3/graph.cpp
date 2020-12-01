/*
1190201520 ����

ʵ��3ͼ�ͽṹ����Ӧ��
ʵ����Ŀ:ͼ�Ĵ洢�ṹ�Ľ���������
ʵ������:
    ͼ������(����)�㷨��ͼ�ͽṹ����㷨�Ļ�������ʵ��Ҫ���д������ʾͼ���ֵ��ʹ洢�ṹ�Ľ���������(����)���̡�
ʵ��Ҫ��:
    1.�ֱ�ʵ��ͼ���ڽӾ����ڽӱ�洢�ṹ�Ľ����㷨�������ͱȽϸ������㷨��ʱ�临�Ӷ��Լ��洢�ṹ�Ŀռ�ռ�����;
    2.���������ִ洢�ṹ�ϣ��ֱ�ʵ��ͼ�������������(�ݹ�ͷǵݹ�)�͹�����������㷨�������ʵ��ķ�ʽ�洢����ʾ��Ӧ���������(������Ȼ�����������ɭ��(��������)��������Ȼ����������кͱ��);
    3.���������㷨��ʱ�临�Ӷ�;
    4.���ļ���ʽ����ͼ�Ķ���ͱߣ�����ʾ��Ӧ�Ľ���� !!Ҫ�󶥵㲻����10�����߲�����13��!!;
    5.������ܽṹ���ź��������Ѻã�����ʹ�á�
    6.ʵ����С�������������㷨: Prim�㷨��Kruskal�㷨
*/

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <queue>
#include <limits>
#include <algorithm>

#define MAXLEN 1000
#define MAX_VERTICES 100
#define INF INT32_MAX

using namespace std;

typedef struct graph_adjacent_matrix
{
    char vertex[MAX_VERTICES];            //�����
    int edge[MAX_VERTICES][MAX_VERTICES]; //�ڽӾ��󡪱߱�, ����Ϊ��֮��Ĺ�ϵ
    int v, e;                             //ͼ�Ķ����������
} Graph_AdjMtx;

typedef struct graph_adjlst_edgenode //�߱���
{
    int adjvex;                  //�ڽӵ����±꣩
    int cost;                    //���ϵ�Ȩֵ
    graph_adjlst_edgenode *next; //��һ������ָ��
} EdgeNode;

typedef struct graph_adjlst_vertexnode //�������
{
    char vertex;         //����������
    EdgeNode *firstedge; //������ͷָ��
} VertexNode;

typedef struct graph_adjacent_list //ͼ���ڽӱ�
{
    VertexNode vexlist[MAX_VERTICES];
    int v, e; //������������ ;
} Graph_AdjLst;

typedef struct kruskal_edge
{
    int bgn;
    int end;
    int weight;
} KEdgeNode;

string saved_tree_path = ".\\saved_tree.txt"; //�洢������

void File_Print(string head, string tree, string save)
{
    FILE *fp = fopen(save.c_str(), "a+");
    if (fp == NULL)
    {
        cout << "file open error";
        system("pause");
        exit(-1);
    }
    tree = tree + "\n";
    fprintf(fp, head.c_str());
    fprintf(fp, tree.c_str());
    return;
}

//�ڽӾ���-Ѱ�ҽڵ��Ӧ�±�
int Find_index_mtx(char ch, char *V)
{
    int i = 0;
    while (V[i] != '\0')
    {
        if (V[i] == ch)
            return i;
        i++;
    }
    return -1;
}

//�ڽӱ�-Ѱ�ҽڵ��Ӧ�±�
int Find_index_lst(char ch, VertexNode *V)
{
    int i = 0;
    while (V[i].vertex != '\0')
    {
        if (V[i].vertex == ch)
            return i;
        i++;
    }
    return -1;
}

//���ڽӾ���ʽ����ͼ
Graph_AdjMtx *Build_Graph_Adjmtx(char *V, char *E, int directed)
{
    int i = 0, j = 0;
    int p = 0;           //ָʾ��ǰ�����ǳ���/���/Ȩֵ
    int temp = 0, t = 0; //��λ��������
    int in = 0, out = 0;
    Graph_AdjMtx *g_mtx = new Graph_AdjMtx;
    //��¼�ڵ�
    g_mtx->v = 0;
    while (V[i] != '\n')
    {
        if (V[i] != ' ')
        {
            g_mtx->v++;
            g_mtx->vertex[j++] = V[i];
        }
        i++;
    }
    //�����ڽӱ�
    i = j = 0;
    // while(E[i]!='\n')
    while (!(E[i] == '\n' || E[i] == '\0'))
    {
        if (E[i] == ' ')
        {
            if (p == 2)
            {
                g_mtx->edge[out][in] = temp;
                g_mtx->edge[in][out] = (directed == 0) ? g_mtx->edge[out][in] : g_mtx->edge[in][out]; //��Ϊ����ͼ����Գ�λ�ø���ͬ��ֵ
                g_mtx->e++;
            }
            in = out = p = temp = 0;
            t = 1;
        }
        else if (E[i] == '-')
        {
            p++;
        }
        else
        {
            if (p == 0)
            {
                out = Find_index_mtx(E[i], g_mtx->vertex);
            }
            else if (p == 1)
            {
                in = Find_index_mtx(E[i], g_mtx->vertex);
            }
            else if (p == 2)
            {
                temp = temp * t + (E[i] - '0');
                t *= 10;
            }
        }
        i++;
    }
    return g_mtx;
}

//�ڽӱ�����ӱ�
void Append_Node(int in, int value, VertexNode *vnode)
{
    EdgeNode *newNode = new EdgeNode;
    newNode->adjvex = in;
    newNode->cost = value;
    newNode->next = NULL;
    if (vnode->firstedge == NULL)
        vnode->firstedge = newNode;
    else
    {
        EdgeNode *p = vnode->firstedge;
        while (p->next != NULL)
            p = p->next;
        p->next = newNode;
    }
    return;
}

//���ڽӱ�ʽ����ͼ
Graph_AdjLst *Build_Graph_Adjlst(char *V, char *E, int directed)
{
    int i = 0, j = 0;
    int p = 0;           //ָʾ��ǰ�����ǳ���/���/Ȩֵ
    int temp = 0, t = 0; //��ʱ����Ȩֵ��λ��
    int in = 0, out = 0;
    Graph_AdjLst *g_lst = new Graph_AdjLst;
    //��¼�ڵ�
    while (V[i] != '\n')
    {
        if (V[i] != ' ')
        {
            g_lst->vexlist[j].vertex = V[i];
            g_lst->vexlist[j].firstedge = NULL;
            g_lst->v++;
            j++;
        }
        i++;
    }
    g_lst->vexlist[i].vertex = '\0';
    //��¼��
    i = j = 0;
    while (E[i] != '\n')
    {
        if (E[i] == ' ')
        {
            if (p == 2)
            {
                Append_Node(in, temp, &(g_lst->vexlist[out]));
                g_lst->e++;
                if (directed == 0)
                {
                    Append_Node(out, temp, &(g_lst->vexlist[in]));
                    g_lst->e++;
                }
            }
            in = out = p = temp = 0;
            t = 1;
        }
        else if (E[i] == '-')
        {
            p++;
        }
        else
        {
            if (p == 0)
            {
                out = Find_index_lst(E[i], g_lst->vexlist);
            }
            else if (p == 1)
            {
                in = Find_index_lst(E[i], g_lst->vexlist);
            }
            else if (p == 2)
            {
                temp = temp * t + (E[i] - '0');
                t *= 10;
            }
        }
        i++;
    }
    return g_lst;
}

//�ڽӾ���-��������
void Mtx_DFS(Graph_AdjMtx *g, int i, bool *visited, int *DFS_num, int *cnt, string &tree)
{
    cout << g->vertex[i] << "  ";
    visited[i] = true;
    DFS_num[i] = *cnt++;
    for (int j = 0; j < g->v; j++)              //��������vi���ڽӵ�
        if ((g->edge[i][j] > 0) && !visited[j]) //��vj��δ����
        {
            char temp[10];
            sprintf(temp, "%c-%c-%d ", g->vertex[i], g->vertex[j], g->edge[i][j]);
            tree += temp;
            Mtx_DFS(g, j, visited, DFS_num, cnt, tree);
        }
    return;
}

//�ڽӾ���-������ȱ���-�ݹ�
void Graph_AdjMtx_DFSTraverse_Recursive(Graph_AdjMtx *g)
{
    bool visited[MAX_VERTICES];
    int DFS_num[MAX_VERTICES]; //����ı������
    string tree;
    int i, cnt = 1;
    cout << "\n DFSTraverse_Recursive(mtx):" << endl;
    for (int j = 0; j < g->v; j++) //��־�����ʼ��
        visited[j] = false;
    for (i = 0; i < g->v; i++)
        if (!visited[i])
            Mtx_DFS(g, i, visited, DFS_num, &cnt, tree); //�Ӷ��� i ������һ������
    cout << "\n spanning tree:" << endl;
    cout << " edges:  " << tree << endl;
    string head = "DFSTraverse_Recursive(mtx):\n";
    File_Print(head, tree, saved_tree_path);
    return;
}

//�ڽӾ���-������ȱ���-�ǵݹ�
void Graph_AdjMtx_DFSTraverse_NonRecursive(Graph_AdjMtx *g)
{
    bool visited[MAX_VERTICES];
    int DFS_num[MAX_VERTICES];
    int i, k, cnt = 1;
    string tree;
    stack<int> S;
    cout << "\n DFSTraverse_NonRecursive(mtx):" << endl;
    for (int k = 0; k < g->v; k++)
        visited[k] = false;

    for (i = 0; i < g->v; i++) //�п���������ɭ�֣������ڿ�ͷҪ��һ������
    {
        if (!visited[i])
        {
            S.push(i);
            cout << g->vertex[S.top()] << "  ";
            visited[i] = true;
        }
        while (!S.empty()) // ջΪ��ʱ����
        {
            int i = S.top(); // iΪ��ǰ�ڵ�
            int j = 0;       // jΪ��i�������������Ľڵ�
            for (j = 0; j < g->v; j++)
            {
                if (!visited[j] && g->edge[i][j] != 0) //�ڵ�δ���ʹ����뵱ǰ�ڵ�֮���б�����
                {
                    S.push(j); //��ǰ�ڵ���ջ
                    visited[j] = true;
                    DFS_num[cnt++] = j;
                    cout << g->vertex[j] << "  ";

                    char temp[10];
                    sprintf(temp, "%c-%c-%d ", g->vertex[i], g->vertex[j], g->edge[i][j]);
                    tree += temp;

                    break;
                }
                if (j == g->v - 1) //���������յ�
                    S.pop();       //������ǰ�ڵ�
            }
        }
    }
    cout << "\n spanning tree:" << endl;
    cout << " edges:  " << tree << endl;
    string head = "DFSTraverse_NonRecursive(mtx):\n";
    File_Print(head, tree, saved_tree_path);
    return;
}

//�ڽӱ�-��������
void Lst_DFS(Graph_AdjLst *g, int i, bool *visited, int *DFS_num, int *cnt, string &tree)
{
    EdgeNode *p;
    cout << g->vexlist[i].vertex << "  ";
    visited[i] = true;
    DFS_num[i] = *cnt++;
    p = g->vexlist[i].firstedge;
    while (p)
    {
        if (!visited[p->adjvex])
        {
            char temp[10];
            sprintf(temp, "%c-%c-%d ", g->vexlist[i].vertex, g->vexlist[p->adjvex].vertex, p->cost);
            tree += temp;
            Lst_DFS(g, p->adjvex, visited, DFS_num, cnt, tree);
        }
        p = p->next;
    }
    return;
}

//�ڽӱ�-������ȱ���-�ݹ�
void Graph_AdjLst_DFSTraverse_Recursive(Graph_AdjLst *g)
{
    bool visited[MAX_VERTICES];
    int DFS_num[MAX_VERTICES];
    int i, cnt = 1;
    string tree;
    cout << "\n DFSTraverse(lst):" << endl;
    for (int j = 0; j < g->v; j++)
        visited[i] = false;
    for (i = 0; i < g->v; i++)
        if (!visited[i])
            Lst_DFS(g, i, visited, DFS_num, &cnt, tree);
    cout << "\n spanning tree:" << endl;
    cout << " edges:  " << tree << endl;
    string head = "DFSTraverse(lst):\n";
    File_Print(head, tree, saved_tree_path);
    return;
}

//�ڽӱ�-������ȱ���-�ǵݹ�
void Graph_AdjLst_DFSTraverse_NonRecursive(Graph_AdjLst *g)
{
    bool visited[MAX_VERTICES];
    int DFS_num[MAX_VERTICES];
    int i, k, cnt = 1;
    string tree;
    stack<int> S;
    cout << "\n DFSTraverse_NonRecursive(lst):" << endl;
    for (int k = 0; k < g->v; k++)
        visited[k] = false;

    for (i = 0; i < g->v; i++)
    {
        if (!visited[i])
        {
            S.push(i);
            cout << g->vexlist[S.top()].vertex << "  ";
            visited[i] = true;
        }
        while (!S.empty()) // ջΪ��ʱ����
        {
            int i = S.top();                       // iΪ��ǰ�ڵ�
            EdgeNode *j = g->vexlist[i].firstedge; // jΪ��i�������������Ľڵ�
            while (j != NULL)
            {
                if (!visited[j->adjvex]) //�ڵ�δ���ʹ����뵱ǰ�ڵ�֮���б�����
                {
                    S.push(j->adjvex); //��ǰ�ڵ���ջ
                    visited[j->adjvex] = true;
                    DFS_num[cnt++] = j->adjvex;
                    cout << g->vexlist[j->adjvex].vertex << "  ";

                    char temp[10];
                    sprintf(temp, "%c-%c-%d ", g->vexlist[i].vertex, g->vexlist[j->adjvex].vertex, j->cost);
                    tree += temp;

                    break;
                }
                j = j->next;
            }
            if (j == NULL) //���������յ�
                S.pop();   //������ǰ�ڵ�
        }
    }
    cout << "\n spanning tree:" << endl;
    cout << " edges:  " << tree << endl;
    string head = "DFSTraverse_NonRecursive(lst):\n";
    File_Print(head, tree, saved_tree_path);
    return;
}

//�ڽӾ���-��������
void Mtx_BFS(Graph_AdjMtx *g, int i, bool *visited, int *BFS_num, int *cnt, string &tree)
{
    int j;
    queue<int> Q;
    cout << g->vertex[i] << "  ";
    visited[i] = true;
    BFS_num[i] = *cnt++;
    Q.push(i);
    while (!Q.empty())
    {
        j = Q.front();
        Q.pop();
        for (int k = 0; k < g->v; k++)
        {
            if (g->edge[j][k] != 0 && !visited[k])
            {
                cout << g->vertex[k] << "  ";
                visited[k] = true;
                BFS_num[k] = *cnt++;
                Q.push(k);

                char temp[10];
                sprintf(temp, "%c-%c-%d ", g->vertex[j], g->vertex[k], g->edge[j][k]);
                tree += temp;
            }
        }
    }
    return;
}

//�ڽӾ���-������ȱ���
void Graph_AdjMtx_BFSTraverse(Graph_AdjMtx *g)
{
    bool visited[MAX_VERTICES];
    int BFS_num[MAX_VERTICES];
    int i, cnt = 1;
    string tree;
    cout << "\n BFSTraverse(mtx):" << endl;
    for (int j = 0; j < g->v; j++)
        visited[j] = false;
    for (i = 0; i < g->v; i++)
        if (!visited[i])
            Mtx_BFS(g, i, visited, BFS_num, &cnt, tree);
    cout << "\n spanning tree:" << endl;
    cout << " edges:  " << tree << endl;
    string head = "BFSTraverse(mtx):\n";
    File_Print(head, tree, saved_tree_path);
    return;
}

//�ڽӱ�-��������
void Lst_BFS(Graph_AdjLst *g, int i, bool *visited, int *BFS_num, int *cnt, string &tree)
{
    int j;
    EdgeNode *p;
    queue<int> Q;
    cout << g->vexlist[i].vertex << "  ";
    visited[i] = true;
    BFS_num[i] = *cnt++;
    Q.push(i);
    while (!Q.empty())
    {
        j = Q.front();
        Q.pop();
        p = g->vexlist[j].firstedge;
        while (p)
        {
            if (!visited[p->adjvex])
            {
                cout << g->vexlist[p->adjvex].vertex << "  ";
                visited[p->adjvex] = true;
                BFS_num[p->adjvex] = *cnt++;
                Q.push(p->adjvex);
                char temp[10];
                sprintf(temp, "%c-%c-%d ", g->vexlist[j].vertex, g->vexlist[p->adjvex].vertex, p->cost);
                tree += temp;
            }
            p = p->next;
        }
    }
    return;
}

//�ڽӱ�-�����������
void Graph_AdjLst_BFSTraverse(Graph_AdjLst *g)
{
    bool visited[MAX_VERTICES];
    int BFS_num[MAX_VERTICES];
    int i, cnt = 1;
    string tree;
    cout << "\n BFSTraverse(lst):" << endl;
    for (int j = 0; j < g->v; j++)
        visited[j] = false;
    for (i = 0; i < g->v; i++)
        if (!visited[i])
            Lst_BFS(g, i, visited, BFS_num, &cnt, tree);

    cout << "\n spanning tree:" << endl;
    cout << " edges:  " << tree << endl;
    string head = "BFSTraverse(lst):\n";
    File_Print(head, tree, saved_tree_path);
    return;
}

//�ڽӾ���-����С������-Prim�㷨������ͼ��
void Prim_AdjMtx(Graph_AdjMtx *g)
{
    /*
    1. ��ʼ��������������LOWCOST��CLOSEST��
    2. �������v0��������v0���뼯��U�У�
    3. �ظ�ִ�����в���n-1�� 
        3.1 ��LOWCOST��ѡȡ��̱ߣ�ȡCLOSEST�ж�Ӧ�Ķ������k��
        3.2 �������k�Ͷ�Ӧ��Ȩֵ��
        3.3 ������k���뼯��U�У�
        3.4 ��������LOWCOST��CLOSEST��

    ʱ�临�Ӷ� O(v^2)
    */
    int LowCost[MAX_VERTICES];
    int Closest[MAX_VERTICES];
    bool visited[MAX_VERTICES] = {false};
    int j, min;
    cout << "\n Prim (mtx):" << endl;
    visited[0] = true;
    for (int k = 0; k < g->v; k++) // ��ʼ����������
    {
        // LowCost[k] = (g->edge[0][k] == 0) ? INF : g->edge[0][k];
        LowCost[k] = (g->edge[0][k] == 0) ? INF : g->edge[0][k];
        Closest[k] = 0;
    }
    for (int i = 1; i < g->v; i++)
    {
        // min = LowCost[i]; 
        // j = i;
        min = INF;//��ʼ
        for (int k = 1; k < g->v; k++) //�ҵ�Ȩֵ��С�Ľڵ㣬
        {
            if (LowCost[k] < min)
            {
                min = LowCost[k];
                j = k;
            }
        }
        cout << "(" << Closest[j] << ", " << j << ")"
             << "  " << LowCost[j] << endl;
        LowCost[j] = INF; //����U����
        visited[j] = true;
        for (int k = 1; k < g->v; k++) //����LowCost��Closest����Ϣ��ֻ��Ҫ����[U-V�и����ڵ�]��[�¼���U�Ľڵ�]�Ĺ�ϵ��
        {
            if (g->edge[j][k] != 0 && !visited[k] && g->edge[j][k] < LowCost[k])
            {
                LowCost[k] = g->edge[j][k];
                Closest[k] = j;
            }
        }
    }
    return;
}

//kruskal ��ʼ����
int Get_K_Edges(Graph_AdjMtx *g, KEdgeNode *e)
{
    int k = 0;
    // ����������ͼ��
    for (int i = 0; i < g->v; i++)
    {
        for (int j = i + 1; j < g->v; j++)
        {
            if (g->edge[i][j] != 0)
            {
                e[k].bgn = i;
                e[k].end = j;
                e[k].weight = g->edge[i][j];
                k++;
            }
        }
    }
    return k; //���رߵĸ���
}

//kruskal�ڵ����� �ȽϺ���
bool KEdgeNode_Compare(KEdgeNode a, KEdgeNode b)
{
    return a.weight < b.weight;
}

//���ظ��ڵ��±�
int Find(int father[], int v)
{
    int f = v;
    while (father[f] > 0)
        f = father[f];
    return f;
}

//�ڽӾ���-����С������-Kruskal�㷨������ͼ��
void Kruskal_AdjMtx(Graph_AdjMtx *g)
{
    KEdgeNode edges[MAX_VERTICES * (MAX_VERTICES - 1)];
    Get_K_Edges(g, edges);
    sort(edges, edges + g->e - 1, KEdgeNode_Compare);

    cout << "\n Kruskal (mtx):" << endl;
    int father[MAX_VERTICES], bnf, edf, i;
    for (int i = 0; i < g->e; i++)
        father[i] = 0; //��ʼ��
    for (int i = 0; i < g->e; i++)
    {
        bnf = Find(father, edges[i].bgn);
        edf = Find(father, edges[i].end);
        if (bnf != edf) //�ж��Ƿ�Ϊͬһ��ͨ����
        {
            father[bnf] = edf;
            cout << "(" << edges[i].bgn << ", " << edges[i].end << ")"
                 << "  " << edges[i].weight << endl;
        }
    }
    return;
}

//�����ļ��������ַ�ʽ����ͼ�����в���
void start(string digraph_path, string undigraph_path)
{
    // TODO ���һ����ѡ��: ����ͼ/����ͼ ���/��� �ݹ�/�ǵݹ� ��
    int directed = 0;
    cout << "����/����ͼ [0/1]:" << endl;
    cin >> directed;

    string file_path = (directed == 0) ? undigraph_path : digraph_path;
    FILE *fp = fopen(file_path.c_str(), "rb");
    if (fp == NULL)
    {
        cout << "file open error" << endl;
        system("pause");
        exit(-1);
    }
    char buffer[2][MAXLEN + 1];
    if (directed != fgetc(fp) - '0')
    {
        cout << "graph type error" << endl;
        system("pause");
        exit(-1);
    }
    fgetc(fp); //�����س�
    fgets(buffer[0], MAXLEN, fp);
    fgets(buffer[1], MAXLEN, fp);
    fclose(fp);

    Graph_AdjMtx *gm = Build_Graph_Adjmtx(buffer[0], buffer[1], directed);
    Graph_AdjLst *gl = Build_Graph_Adjlst(buffer[0], buffer[1], directed);

    string vertices = buffer[0];
    if (directed == 0)
    {
        string graph_type_head = "Undirected Graph:\n";
        File_Print(graph_type_head, vertices, saved_tree_path);
    }
    else if (directed == 1)
    {
        string graph_type_head = "Directed Graph:\n";
        File_Print(graph_type_head, vertices, saved_tree_path);
    }

    cout << "\n\n adjacency matrix:"
         << endl;
    Graph_AdjMtx_DFSTraverse_Recursive(gm);
    Graph_AdjMtx_DFSTraverse_NonRecursive(gm);
    Graph_AdjMtx_BFSTraverse(gm);
    if (directed == 0) //���������ͼ��ʹ��prim��kruskal
    {
        Prim_AdjMtx(gm);
        Kruskal_AdjMtx(gm);
    }

    cout << "\n\n  adjacency list:"
         << endl;
    Graph_AdjLst_DFSTraverse_Recursive(gl);
    Graph_AdjLst_DFSTraverse_NonRecursive(gl);
    Graph_AdjLst_BFSTraverse(gl);

    cout << endl;
    return;
}

int main()
{
    string digraph_path = ".\\directed.txt";
    string undigraph_path = ".\\undirected.txt";
    start(digraph_path, undigraph_path);
    system("pause");
    return 0;
}

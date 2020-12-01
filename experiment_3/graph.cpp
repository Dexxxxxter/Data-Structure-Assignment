/*
1190201520 李想

实验3图型结构及其应用
实验题目:图的存储结构的建立与搜索
实验内容:
    图的搜索(遍历)算法是图型结构相关算法的基础，本实验要求编写程序演示图两种典型存储结构的建立和搜索(遍历)过程。
实验要求:
    1.分别实现图的邻接矩阵、邻接表存储结构的建立算法，分析和比较各建立算法的时间复杂度以及存储结构的空间占用情况;
    2.在上述两种存储结构上，分别实现图的深度优先搜索(递归和非递归)和广度优先搜索算法。并以适当的方式存储和显示相应的搜索结果(深度优先或广度优先生成森林(或生成树)、深度优先或广度优先序列和编号);
    3.分析搜索算法的时间复杂度;
    4.以文件形式输入图的顶点和边，并显示相应的结果。 !!要求顶点不少于10个，边不少于13个!!;
    5.软件功能结构安排合理，界面友好，便于使用。
    6.实现最小生成树的两个算法: Prim算法和Kruskal算法
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
    char vertex[MAX_VERTICES];            //顶点表
    int edge[MAX_VERTICES][MAX_VERTICES]; //邻接矩阵—边表, 可视为边之间的关系
    int v, e;                             //图的顶点数与边数
} Graph_AdjMtx;

typedef struct graph_adjlst_edgenode //边表结点
{
    int adjvex;                  //邻接点域（下标）
    int cost;                    //边上的权值
    graph_adjlst_edgenode *next; //下一边链接指针
} EdgeNode;

typedef struct graph_adjlst_vertexnode //顶点表结点
{
    char vertex;         //顶点数据域
    EdgeNode *firstedge; //边链表头指针
} VertexNode;

typedef struct graph_adjacent_list //图的邻接表
{
    VertexNode vexlist[MAX_VERTICES];
    int v, e; //顶点个数与边数 ;
} Graph_AdjLst;

typedef struct kruskal_edge
{
    int bgn;
    int end;
    int weight;
} KEdgeNode;

string saved_tree_path = ".\\saved_tree.txt"; //存储生成树

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

//邻接矩阵-寻找节点对应下标
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

//邻接表-寻找节点对应下标
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

//以邻接矩阵方式建立图
Graph_AdjMtx *Build_Graph_Adjmtx(char *V, char *E, int directed)
{
    int i = 0, j = 0;
    int p = 0;           //指示当前读的是出点/入点/权值
    int temp = 0, t = 0; //多位数字输入
    int in = 0, out = 0;
    Graph_AdjMtx *g_mtx = new Graph_AdjMtx;
    //记录节点
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
    //创建邻接表
    i = j = 0;
    // while(E[i]!='\n')
    while (!(E[i] == '\n' || E[i] == '\0'))
    {
        if (E[i] == ' ')
        {
            if (p == 2)
            {
                g_mtx->edge[out][in] = temp;
                g_mtx->edge[in][out] = (directed == 0) ? g_mtx->edge[out][in] : g_mtx->edge[in][out]; //若为无向图，则对称位置赋相同的值
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

//邻接表中添加边
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

//以邻接表方式建立图
Graph_AdjLst *Build_Graph_Adjlst(char *V, char *E, int directed)
{
    int i = 0, j = 0;
    int p = 0;           //指示当前读的是出点/入点/权值
    int temp = 0, t = 0; //临时储存权值和位数
    int in = 0, out = 0;
    Graph_AdjLst *g_lst = new Graph_AdjLst;
    //记录节点
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
    //记录边
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

//邻接矩阵-深搜主体
void Mtx_DFS(Graph_AdjMtx *g, int i, bool *visited, int *DFS_num, int *cnt, string &tree)
{
    cout << g->vertex[i] << "  ";
    visited[i] = true;
    DFS_num[i] = *cnt++;
    for (int j = 0; j < g->v; j++)              //依次搜索vi的邻接点
        if ((g->edge[i][j] > 0) && !visited[j]) //若vj尚未访问
        {
            char temp[10];
            sprintf(temp, "%c-%c-%d ", g->vertex[i], g->vertex[j], g->edge[i][j]);
            tree += temp;
            Mtx_DFS(g, j, visited, DFS_num, cnt, tree);
        }
    return;
}

//邻接矩阵-深度优先遍历-递归
void Graph_AdjMtx_DFSTraverse_Recursive(Graph_AdjMtx *g)
{
    bool visited[MAX_VERTICES];
    int DFS_num[MAX_VERTICES]; //顶点的遍历编号
    string tree;
    int i, cnt = 1;
    cout << "\n DFSTraverse_Recursive(mtx):" << endl;
    for (int j = 0; j < g->v; j++) //标志数组初始化
        visited[j] = false;
    for (i = 0; i < g->v; i++)
        if (!visited[i])
            Mtx_DFS(g, i, visited, DFS_num, &cnt, tree); //从顶点 i 出发的一次搜索
    cout << "\n spanning tree:" << endl;
    cout << " edges:  " << tree << endl;
    string head = "DFSTraverse_Recursive(mtx):\n";
    File_Print(head, tree, saved_tree_path);
    return;
}

//邻接矩阵-深度优先遍历-非递归
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

    for (i = 0; i < g->v; i++) //有可能是生成森林，所以在开头要加一个遍历
    {
        if (!visited[i])
        {
            S.push(i);
            cout << g->vertex[S.top()] << "  ";
            visited[i] = true;
        }
        while (!S.empty()) // 栈为空时结束
        {
            int i = S.top(); // i为当前节点
            int j = 0;       // j为从i出发向下搜索的节点
            for (j = 0; j < g->v; j++)
            {
                if (!visited[j] && g->edge[i][j] != 0) //节点未访问过且与当前节点之间有边相连
                {
                    S.push(j); //当前节点入栈
                    visited[j] = true;
                    DFS_num[cnt++] = j;
                    cout << g->vertex[j] << "  ";

                    char temp[10];
                    sprintf(temp, "%c-%c-%d ", g->vertex[i], g->vertex[j], g->edge[i][j]);
                    tree += temp;

                    break;
                }
                if (j == g->v - 1) //到达深搜终点
                    S.pop();       //弹出当前节点
            }
        }
    }
    cout << "\n spanning tree:" << endl;
    cout << " edges:  " << tree << endl;
    string head = "DFSTraverse_NonRecursive(mtx):\n";
    File_Print(head, tree, saved_tree_path);
    return;
}

//邻接表-深搜主体
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

//邻接表-深度优先遍历-递归
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

//邻接表-深度优先遍历-非递归
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
        while (!S.empty()) // 栈为空时结束
        {
            int i = S.top();                       // i为当前节点
            EdgeNode *j = g->vexlist[i].firstedge; // j为从i出发向下搜索的节点
            while (j != NULL)
            {
                if (!visited[j->adjvex]) //节点未访问过且与当前节点之间有边相连
                {
                    S.push(j->adjvex); //当前节点入栈
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
            if (j == NULL) //到达深搜终点
                S.pop();   //弹出当前节点
        }
    }
    cout << "\n spanning tree:" << endl;
    cout << " edges:  " << tree << endl;
    string head = "DFSTraverse_NonRecursive(lst):\n";
    File_Print(head, tree, saved_tree_path);
    return;
}

//邻接矩阵-广搜主体
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

//邻接矩阵-广度优先遍历
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

//邻接表-广搜主体
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

//邻接表-广度优先搜索
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

//邻接矩阵-求最小生成树-Prim算法（无向图）
void Prim_AdjMtx(Graph_AdjMtx *g)
{
    /*
    1. 初始化两个辅助数组LOWCOST和CLOSEST；
    2. 输出顶点v0，将顶点v0加入集合U中；
    3. 重复执行下列操作n-1次 
        3.1 在LOWCOST中选取最短边，取CLOSEST中对应的顶点序号k；
        3.2 输出顶点k和对应的权值；
        3.3 将顶点k加入集合U中；
        3.4 调整数组LOWCOST和CLOSEST；

    时间复杂度 O(v^2)
    */
    int LowCost[MAX_VERTICES];
    int Closest[MAX_VERTICES];
    bool visited[MAX_VERTICES] = {false};
    int j, min;
    cout << "\n Prim (mtx):" << endl;
    visited[0] = true;
    for (int k = 0; k < g->v; k++) // 初始化两个数组
    {
        // LowCost[k] = (g->edge[0][k] == 0) ? INF : g->edge[0][k];
        LowCost[k] = (g->edge[0][k] == 0) ? INF : g->edge[0][k];
        Closest[k] = 0;
    }
    for (int i = 1; i < g->v; i++)
    {
        // min = LowCost[i]; 
        // j = i;
        min = INF;//初始
        for (int k = 1; k < g->v; k++) //找到权值最小的节点，
        {
            if (LowCost[k] < min)
            {
                min = LowCost[k];
                j = k;
            }
        }
        cout << "(" << Closest[j] << ", " << j << ")"
             << "  " << LowCost[j] << endl;
        LowCost[j] = INF; //归入U集合
        visited[j] = true;
        for (int k = 1; k < g->v; k++) //更新LowCost和Closest的信息（只需要考虑[U-V中各个节点]与[新加入U的节点]的关系）
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

//kruskal 初始化边
int Get_K_Edges(Graph_AdjMtx *g, KEdgeNode *e)
{
    int k = 0;
    // 仅用于无向图！
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
    return k; //返回边的个数
}

//kruskal节点排序 比较函数
bool KEdgeNode_Compare(KEdgeNode a, KEdgeNode b)
{
    return a.weight < b.weight;
}

//返回父节点下标
int Find(int father[], int v)
{
    int f = v;
    while (father[f] > 0)
        f = father[f];
    return f;
}

//邻接矩阵-求最小生成树-Kruskal算法（无向图）
void Kruskal_AdjMtx(Graph_AdjMtx *g)
{
    KEdgeNode edges[MAX_VERTICES * (MAX_VERTICES - 1)];
    Get_K_Edges(g, edges);
    sort(edges, edges + g->e - 1, KEdgeNode_Compare);

    cout << "\n Kruskal (mtx):" << endl;
    int father[MAX_VERTICES], bnf, edf, i;
    for (int i = 0; i < g->e; i++)
        father[i] = 0; //初始化
    for (int i = 0; i < g->e; i++)
    {
        bnf = Find(father, edges[i].bgn);
        edf = Find(father, edges[i].end);
        if (bnf != edf) //判断是否为同一连通分量
        {
            father[bnf] = edf;
            cout << "(" << edges[i].bgn << ", " << edges[i].end << ")"
                 << "  " << edges[i].weight << endl;
        }
    }
    return;
}

//读入文件并以两种方式建立图并进行操作
void start(string digraph_path, string undigraph_path)
{
    // TODO 添加一连串选择: 有向图/无向图 深度/广度 递归/非递归 …
    int directed = 0;
    cout << "无向/有向图 [0/1]:" << endl;
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
    fgetc(fp); //消除回车
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
    if (directed == 0) //如果是无向图，使用prim和kruskal
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

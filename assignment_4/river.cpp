/*
农夫需要把狼、羊、菜和自己运到河对岸去，只有农夫能够划船，而且船比较小，除农夫之外每次只能运一种东西，还有一个棘手问题，就是如果没有农夫看着，羊会偷吃菜，狼会吃羊。请考虑一种最佳方法编写算法，让农夫能够安全地安排这些东西和他自己以最短的次数过河。
（最短路径问题）
*/

#include <iostream>
#include <cstring>
#include <limits>
#include <iomanip>

#define INF INT32_MAX

using namespace std;

#define MAX_VERTICES 100

typedef struct graph_node
{
    // 0/1分别表示在河的两侧
    int farmer;
    int sheep;
    int veg;
    int wolf;
} Node;

typedef struct graph_adjacent_matrix
{
    Node vertex[MAX_VERTICES];            //顶点表
    int edge[MAX_VERTICES][MAX_VERTICES]; //邻接矩阵—边表, 可视为边之间的关系
    int v, e;                             //图的顶点数与边数
} Graph;

//初始化图的顶点
void Init_Vertices(Graph *g)
{
    g->v = 0;
    for (int a1 = 0; a1 <= 1; a1++)
    {
        for (int a2 = 0; a2 <= 1; a2++)
        {
            for (int a3 = 0; a3 <= 1; a3++)
            {
                for (int a4 = 0; a4 <= 1; a4++)
                {
                    g->vertex[g->v].farmer = a1;
                    g->vertex[g->v].sheep = a2;
                    g->vertex[g->v].veg = a3;
                    g->vertex[g->v].wolf = a4;
                    g->v++;
                }
            }
        }
    }
    return;
}

//判断两个状态能否相互转换
bool Transition(Node a, Node b)
{
    if (a.farmer == b.farmer) //农夫没有过河则状态无法转移
        return false;
    if (abs(a.sheep - b.sheep) + abs(a.veg - b.veg) + abs(a.wolf - b.wolf) > 1) //除农夫外，过河的东西超过一个
        return false;
    if ((a.sheep != b.sheep && a.farmer != a.sheep) ||
        (a.veg != b.veg && a.farmer != a.veg) ||
        (a.wolf != b.wolf && a.farmer != a.wolf)) //农夫和另一个过河的东西一开始必须在同一侧
        return false;
    return true;
}

//判断该状态是否能稳定存在
bool Possible(Node a)
{
    //狼和羊在同一侧且无农夫
    //羊和菜在同一侧且无农夫
    if ((a.wolf == a.sheep && a.farmer != a.sheep) ||
        (a.sheep == a.veg && a.farmer != a.sheep))
        return false;
    return true;
}

//初始化图的边
void Init_Edges(Graph *g)
{
    memset(g->edge, 0, sizeof(g->edge));
    for (int i = 0; i < g->v; i++)
    {
        for (int j = i + 1; j < g->v; j++)
        {
            if (Possible(g->vertex[i]) &&
                Possible(g->vertex[j]) &&
                Transition(g->vertex[i], g->vertex[j]))
                g->edge[i][j] = g->edge[j][i] = 1;
        }
    }
    return;
}

//建立状态转移图
Graph *Build_State_Graph()
{
    Graph *g = new Graph;
    Init_Vertices(g);
    Init_Edges(g);
    return g;
}

//寻找起始点和终点对应的编号
void FindNodes(Graph *g, int &bgn, int &end)
{
    for (int i = 0; i < g->v; i++)
    {
        if (g->vertex[i].farmer == 0 && g->vertex[i].sheep == 0 && g->vertex[i].veg == 0 && g->vertex[i].wolf == 0)
            bgn = i;
        else if (g->vertex[i].farmer == 1 && g->vertex[i].sheep == 1 && g->vertex[i].veg == 1 && g->vertex[i].wolf == 1)
            end = i;
    }
    return;
}

//打印表头
void Print_Head()
{
    cout << "\t"
         << "State"
         << "\t"
         << "farmer"
         << "\t"
         << "sheep"
         << "\t"
         << "veg"
         << "\t"
         << "wolf" << endl;
    return;
}

//dijkstra算法-打印结果
void Print_Path_Dijkstra(Graph *g, int *p, int bgn, int i)
{
    if (i != bgn)
    {
        Print_Path_Dijkstra(g, p, bgn, p[i]);
    }
    cout << "\t" << i << ": ";
    cout << "\t" << (int)(g->vertex[i].farmer)
         << "\t" << (int)(g->vertex[i].sheep)
         << "\t" << (int)(g->vertex[i].veg)
         << "\t" << (int)(g->vertex[i].wolf)
         << endl;
    return;
}

/*
dijkstra算法
时间复杂度O(n^2)
bgn: 路径起点
end：路径终点
*/
void FindMinPath_Dijkstra(Graph *g, int bgn, int end)
{
    int D[MAX_VERTICES];           //起点到第i个顶点的[当前]最短路径长度
    int P[MAX_VERTICES];           //起点到第1个顶点的[当前]最短路径上的最后一个顶点
    bool S[MAX_VERTICES];          //起点和已经生成的终点
    for (int i = 1; i < g->v; i++) //初始化
    {
        if (g->edge[bgn][i] != 0)
        {
            D[i] = g->edge[bgn][i];
            P[i] = bgn;
        }
        else
        {
            D[i] = INF;
            P[i] = -1;
        }
        S[i] = false;
    }
    S[bgn] = true;                 //从0号结点开始
    for (int i = 0; i < g->v; i++) //找到D中最短的路径
    {
        int min = INF;
        int j = 1; //存储最短路径的节点下标
        for (int k = 0; k < g->v; k++)
        {
            if (!S[k] && D[k] < min)
            {
                min = D[k];
                j = k;
            }
        }
        S[j] = true;                   //将j节点并入集合S
        for (int k = 0; k < g->v; k++) //对U-S中节点，更新最短路径
        {
            if (!S[k] && g->edge[j][k] != 0)
            {
                int tempsum = D[j] + g->edge[j][k];
                if (tempsum < D[k])
                {
                    D[k] = tempsum;
                    P[k] = j;
                }
            }
        }
    }
    cout << "Shortest Path (Dijkstra):" << endl;
    Print_Head();
    Print_Path_Dijkstra(g, P, bgn, end);
    return;
}

//floyd算法-打印结果
void Print_Path_Floyd(Graph *g, int p[MAX_VERTICES][MAX_VERTICES], int i, int j)
{
    if (i == j)
    {
        cout << "\t" << i << ": ";
        cout << "\t" << (int)(g->vertex[i].farmer)
             << "\t" << (int)(g->vertex[i].sheep)
             << "\t" << (int)(g->vertex[i].veg)
             << "\t" << (int)(g->vertex[i].wolf)
             << endl;
        return;
    }
    int k = p[i][j];
    if (k != -1)
    {
        Print_Path_Floyd(g, p, i, k);
        cout << "\t" << p[i][j] << ": ";
        cout << "\t" << (int)(g->vertex[k].farmer)
             << "\t" << (int)(g->vertex[k].sheep)
             << "\t" << (int)(g->vertex[k].veg)
             << "\t" << (int)(g->vertex[k].wolf)
             << endl;
        Print_Path_Floyd(g, p, k, j);
    }
    return;
}

/*
floyd算法
时间复杂度O(n^3)
bgn: 路径起点
end：路径终点
*/
void FindMinPath_Floyd(Graph *g, int bgn, int end)
{
    int D[MAX_VERTICES][MAX_VERTICES]; //D[i][j]: i和j之间的路径长度
    int P[MAX_VERTICES][MAX_VERTICES]; //P[i][j]: i和j之间路径所经过的某个点的下标
    for (int i = 0; i < g->v; i++)     //初始化
    {
        for (int j = 0; j < g->v; j++)
        {
            if (g->edge[i][j] == 0)
                D[i][j] = INF;
            else
                D[i][j] = g->edge[i][j];
            P[i][j] = -1;
        }
    }
    for (int k = 0; k < g->v; k++) //将 [第k个顶点] 作为插入[从i到j的路径] 的点
        for (int i = 0; i < g->v; i++)
            for (int j = 0; j < g->v; j++)
            {
                int tempsum;
                if (D[i][k] != INF && D[k][j] != INF) //如果ik，kj都连通，则可以插入第k个点
                    tempsum = D[i][k] + D[k][j];
                if (D[i][k] + D[k][j] < D[i][j]) //如果插入第k个点后路径变短，则更新D
                {
                    D[i][j] = tempsum;
                    P[i][j] = k;
                }
            }
    cout << "\nShortest Path (Floyd):" << endl;
    Print_Head();
    Print_Path_Floyd(g, P, bgn, bgn);
    Print_Path_Floyd(g, P, bgn, end);
    Print_Path_Floyd(g, P, end, end);
    return;
}

int main()
{
    Graph *g = Build_State_Graph();
    int bgn, end;
    FindNodes(g, bgn, end);
    FindMinPath_Dijkstra(g, bgn, end);
    FindMinPath_Floyd(g, bgn, end);
    system("pause");
    return 0;
}
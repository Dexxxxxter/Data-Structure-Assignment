/*
写stack相关的函数 （用结构体/数组表示每个元素？）
push, pop, empty(), top, ...
继续修改getmaze函数
path_output 函数
*/

#include <stdio.h>
#include <iostream>
#include <stack>
using namespace std;

typedef struct path
{
    int i; //x pos
    int j; //y pos
    int v; //direction
} PATH;

int maze[13][17] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1},
    {1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

// "move" will conflict with the reserved word？
int mymove[8][2] = {
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1},
    {-1, 0},
    {-1, 1}};

// output the stack in reversed order
void path_output(stack<PATH> S)
{
    stack<PATH> rev;
    while (!S.empty())
    {
        rev.push(S.top());
        S.pop();
    }
    while (!rev.empty()) //pop out of stack in normal order
    {
        cout << rev.top().i << "  " << rev.top().j << endl;
        rev.pop();
    }
    return;
}

// DFS
void GETMAZE(int maze[][17], int mark[][17], int move[][2], int m, int n)
{
    int i = 1, j = 1, v = 0; //(i,j) is the current position and v represents the direction
    int g, h;                //(g,h) is the next position
    mark[1][1] = 1;          //mark the path already taken
    stack<PATH> S;
    PATH cur;
    do
    {
        g = i + move[v][0];
        h = j + move[v][1];
        if ((maze[g][h] == 0) && mark[g][h] == 0)
        {
            mark[g][h] = 1; //mark the next position as taken
            cur.i = i;
            cur.j = j;
            cur.v = v;
            S.push(cur); //push the current pos and direction into stack
            //update pos
            i = g;
            j = h;
            v = 0;
        }
        else if (v < (8 - 1))
            v++; //change direction
        else
        {
            while ((S.top().v == (8 - 1)) && (!S.empty()))
            {
                S.pop(); //pop until the latest possible pos
            }
            if (S.size() > 0)
            {
                PATH temp = S.top();
                i = temp.i;
                j = temp.j;
                v = temp.v + 1; //change direction
                S.pop();
            }
        };
        if ((g == m) && (h == n) && (maze[m][n] == 0))
        {
            PATH dest;
            dest.i = g;
            dest.j = h;
            dest.v = v;
            S.push(dest); //push the destination into stack
            path_output(S);
            return;
        }
    } while ((!S.empty()) || (v != (8 - 1)));
    cout << "the path doesn't exist!" << endl;
}

int main()
{
    int mark[14][17] = {0};
    GETMAZE(maze, mark, mymove, 11, 15);
    system("pause");
    return 0;
}

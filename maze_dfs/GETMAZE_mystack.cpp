/*
使用mystack.h头文件
*/

#include <stdio.h>
#include <iostream>
#include "mystack.h"
using namespace std;

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

// DFS
void GETMAZE(int maze[][17], int mark[][17], int move[][2], int m, int n)
{
    int i = 1, j = 1, v = 0; //(i,j) is the current position and v represents the direction
    int g, h;                //(g,h) is the next position
    mark[1][1] = 1;          //mark the path already taken
    STACK *S = init_path_stack();
    STACK cur;
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
            push(cur, S); //push the current pos and direction into stack
            //update pos
            i = g;
            j = h;
            v = 0;
        }
        else if (v < (8 - 1))
            v++; //change direction
        else
        {
            while ((!isempty(S)) && (top(S).v == (8 - 1)))
            {
                pop(S); //pop until the latest possible pos
            }
            if (!isempty(S))
            {
                STACK temp = top(S);
                i = temp.i;
                j = temp.j;
                v = temp.v + 1; //change direction
                pop(S);
            }
        };
        if ((g == m) && (h == n) && (maze[m][n] == 0))
        {
            STACK dest;
            dest.i = g;
            dest.j = h;
            dest.v = v;
            push(dest, S); //push the destination into stack
            reversed_output(S);
            return;
        }
    } while ((!isempty(S)) || (v != (8 - 1)));
    cout << "the path doesn't exist!" << endl;
}

int main()
{
    int mark[14][17] = {0};
    GETMAZE(maze, mark, mymove, 11, 15);
    system("pause");
    return 0;
}

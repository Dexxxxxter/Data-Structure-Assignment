/*
实验一：算术表达式求值

1190201520 李想

要求：
键盘可以重复输入中缀算术表达式
编程实现转换成后缀表达式输出
再对该后缀表达式求值计算输出结果。
运算符： + - * / % 
数类型： 整数、小数、负数
*/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stack>
#include <string>
#include <map>

using namespace std;

#define MAXLEN 100 //表达式（结构体数组）最长长度
#define EPS 1e-8   //判断double为0 所用epsilon

//表达式的基本字符集
string base_num = "0123456789";
string base_op = "+-*/%(). "; //加入中文括号？

map<char, int> op_map;

typedef struct expression
{
    double val; //1
    char op;    //2
    int flag;   //flag==0 -> end of expression
} EX;

//判断字符是否在字符串中
int isinclude(char c, string s)
{
    size_t include = s.find_first_of(c);
    if (include != string::npos)
        return 1;
    else
        return 0;
}

//创建从运算符到运算优先级之间的映射
void make_op_map()
{
    op_map.insert(pair<char, int>('+', 1));
    op_map.insert(pair<char, int>('-', 1));
    op_map.insert(pair<char, int>('*', 2));
    op_map.insert(pair<char, int>('/', 2));
    op_map.insert(pair<char, int>('%', 2));
    op_map.insert(pair<char, int>('(', 3));
}

//打印结构体数组表达式
void print_expression(EX *ex)
{
    int i = 0;
    while (ex[i].flag != 0)
    {
        switch (ex[i].flag)
        {
        case 1:
            cout << ex[i].val << " ";
            break;
        case 2:
            cout << ex[i].op << " ";
            break;
        }
        i++;
    }
    cout << endl;
    return;
}

//读入string类型的表达式，并初步检查合法性
string input()
{
    string s;
    int valid = 0;
    do
    {
        getline(cin, s); //使用getline，兼容有空格的表达式
        if (s == "Q")
        {
            return s;
        }
        for (string::iterator it = s.begin(); it != s.end(); it++) //判断表达式中字符是否在基本字符集中
        {
            valid = isinclude(*it, base_num) + isinclude(*it, base_op);
            if (!valid)
            {
                cout << "表达式不合法，请重新输入" << endl;
                break;
            }
        }
    } while (valid == 0);
    return s;
}

//将读入的string类型表达式转换为结构体数组（区分数和运输符）
EX *Str2Ex(string s)
{
    double temp0 = 0, temp1 = 0;
    int pos = 0, times = 1;
    int flag = 0; //0:符号 1:计算整数部分 2:计算小数部分
    EX *res = new EX[MAXLEN];
    for (string::iterator it = s.begin(); it <= s.end(); it++)
    {
        if (isinclude(*it, base_num)) //如果是数字则计算整数部分
        {
            switch (flag)
            {
            case 0:
                flag = 1;
            case 1:
                temp0 = temp0 * 10 + *it - '0';
                break;
            case 2:
                temp1 += (*it - '0') * pow(0.1, times++);
                break;
            }
        }
        else if (*it == '.') //如果是小数点则开始计算小数部分
        {
            flag = 2;
        }
        else if (*it == ' ') //忽略空格
        {
            continue;
        }
        else //遇到操作符
        {
            //先存入数字
            if (flag)
            {
                res[pos].val = temp0 + temp1;
                res[pos++].flag = 1;
                flag = 0;
                times = 1;
                temp0 = temp1 = 0; //初始化存储数字的临时变量
            }
            res[pos].op = *it;   //存入当前操作符
            res[pos++].flag = 2; //pos指向下一个位置
        }
    }
    res[pos].flag = 0; //末尾标志
    return res;
}

//将中缀表达式转换为后缀表达式
/*
    对中缀表达式从左至右依次扫描，由于操作数的顺序保持不变，当遇到操作数时直接输出；
    为调整运算顺序，设立一个栈用以保存操作符，扫描到操作符时，将操作符压入栈中，
        进栈的原则是保持栈顶操作符的优先级要高于栈中其他操作符的优先级；
        否则，将栈顶操作符依次退栈并输出，直到满足要求为止；
    遇到 “（” 进栈，当遇到 “）” 时，退栈输出直到 “（” 为止
*/
EX *Infix2Suffix(EX *infix)
{
    int temp;
    int pos = 0;
    int i = 0;
    EX *suffix = new EX[MAXLEN];
    stack<EX> op;
    while (infix[i].flag != 0)
    {
        if (infix[i].flag == 1)  //如果是数字
        {
            suffix[pos++] = infix[i];
        }
        else  //如果是运算符
        {
            if (op.empty() || op.top().op == '(')
            {
                op.push(infix[i]);
            }
            else if (infix[i].op == '(')
            {
                op.push(infix[i]);
            }
            else if (infix[i].op == ')')
            {
                do
                {
                    suffix[pos++] = op.top();
                    op.pop();
                } while (op.top().op != '(');
                op.pop();
            }
            else  //如果是其它运算符，则入栈（考虑优先级）
            {
                if (op_map[infix[i].op] > op_map[op.top().op])
                {
                    op.push(infix[i]);
                }
                else
                {
                    while (!op.empty() && op_map[infix[i].op] <= op_map[op.top().op] && op.top().op != '(')
                    {
                        suffix[pos++] = op.top();
                        op.pop();
                    }
                    op.push(infix[i]);
                }
            }
        }
        i++;
    }
    suffix[pos].flag = 0;
    return suffix;
}

//对表达式中的负号/减号进行单独处理
EX *Neg_Process(EX *exp)
{
    EX *infix = new EX[MAXLEN];
    int i = 0;
    int pos = 0;
    while (exp[i].flag != 0)
    {
        if (exp[i].flag == 2 && exp[i].op == '-') //如果是负号
        {
            if (i == 0) //如果位于首位
            {
                if (exp[i + 1].flag == 1) //如果后面为数字
                //考虑越界？？
                {
                    exp[++i].val *= -1;
                    infix[pos++] = exp[i];
                }
                else if (exp[i + 1].flag == 2 && exp[i + 1].op == '(') //如果后面为左括号
                {
                    EX tmp1;
                    tmp1.val = 0;
                    tmp1.flag = 1;
                    infix[pos++] = tmp1;
                    infix[pos++] = exp[i];
                }
            }
            else if (exp[i - 1].flag == 2 && exp[i - 1].op == '(') //如果前面是左括号
            //考虑越界？？
            {
                if (exp[i + 1].flag == 1) //如果后面为数字
                //考虑越界？？
                {
                    exp[++i].val *= -1;
                    infix[pos++] = exp[i];
                }
            }
            else
            {
                infix[pos++] = exp[i];
            }
        }
        else
        {
            infix[pos++] = exp[i];
        }
        i++;
    }
    infix[pos].flag = 0;
    return infix;
}

//计算后缀表达式的结果
/*
    对后缀表达式从左至右依次扫描，遇到操作数时，将操作数进栈保存； 
    当遇到操作符时，从栈中退出两个操作数并作相应运算，将计算结果进栈保存；直到表达式结束，栈中唯一元素即为表达式的值。
*/
void calculate(EX *suffix)
{
    int i = 0;
    stack<char> op;
    stack<double> num;
    while (suffix[i].flag != 0)
    {
        if (suffix[i].flag == 1)
        {
            num.push(suffix[i].val);
        }
        else
        {
            double n1 = num.top();
            num.pop();
            double n2 = num.top();
            num.pop();
            switch (suffix[i].op)
            {
            case '+':
                num.push(n1 + n2);
                break;
            case '-':
                num.push(n2 - n1); //注意两个变量的次序！
                break;
            case '*':
                num.push(n1 * n2);
                break;
            case '/':
                if (abs(n1) <= EPS)
                {
                    cout << "Division by Zero!" << endl;
                    return;
                }
                else
                {
                    num.push(n2 / n1);
                }
                break;
            case '%':
                int N1 = static_cast<int>(n1), N2 = static_cast<int>(n2);
                if (abs(n1 - N1) <= EPS && abs(n2 - N2) <= EPS)
                {
                    num.push(N2 % N1);
                }
                else
                {
                    cout << "Mod by double!" << endl;
                    return;
                }
                break;
            }
        }
        i++;
    }
    if (num.size() == 1)
    {
        cout << num.top() << setprecision(8) << endl;
    }
    return;
}

//程序开始
void start()
{
    while (1) //循环
    {
        cout << "please enter your infix expression: (enter 'Q' to quit)" << endl;
        string raw;
        EX *infix, *suffix, *infix2;
        int infix_len = 0, suffix_len = 0;
        raw = input();
        if (raw == "Q")
        {
            //system("cls");
            cout << "end" << endl;
            system("pause");
            return;
        }
        else
        {
            infix = Str2Ex(raw);
            infix2 = Neg_Process(infix);
            print_expression(infix2);
            suffix = Infix2Suffix(infix2);
            print_expression(suffix);
            calculate(suffix);
            delete[] infix2;
            delete[] suffix;
        }
    }
}

int main()
{
    make_op_map();
    start();
    return 0;
}

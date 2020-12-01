/*
ʵ��һ���������ʽ��ֵ

1190201520 ����

Ҫ��
���̿����ظ�������׺�������ʽ
���ʵ��ת���ɺ�׺���ʽ���
�ٶԸú�׺���ʽ��ֵ������������
������� + - * / % 
�����ͣ� ������С��������
*/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stack>
#include <string>
#include <map>

using namespace std;

#define MAXLEN 100 //���ʽ���ṹ�����飩�����
#define EPS 1e-8   //�ж�doubleΪ0 ����epsilon

//���ʽ�Ļ����ַ���
string base_num = "0123456789";
string base_op = "+-*/%(). "; //�����������ţ�

map<char, int> op_map;

typedef struct expression
{
    double val; //1
    char op;    //2
    int flag;   //flag==0 -> end of expression
} EX;

//�ж��ַ��Ƿ����ַ�����
int isinclude(char c, string s)
{
    size_t include = s.find_first_of(c);
    if (include != string::npos)
        return 1;
    else
        return 0;
}

//��������������������ȼ�֮���ӳ��
void make_op_map()
{
    op_map.insert(pair<char, int>('+', 1));
    op_map.insert(pair<char, int>('-', 1));
    op_map.insert(pair<char, int>('*', 2));
    op_map.insert(pair<char, int>('/', 2));
    op_map.insert(pair<char, int>('%', 2));
    op_map.insert(pair<char, int>('(', 3));
}

//��ӡ�ṹ��������ʽ
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

//����string���͵ı��ʽ�����������Ϸ���
string input()
{
    string s;
    int valid = 0;
    do
    {
        getline(cin, s); //ʹ��getline�������пո�ı��ʽ
        if (s == "Q")
        {
            return s;
        }
        for (string::iterator it = s.begin(); it != s.end(); it++) //�жϱ��ʽ���ַ��Ƿ��ڻ����ַ�����
        {
            valid = isinclude(*it, base_num) + isinclude(*it, base_op);
            if (!valid)
            {
                cout << "���ʽ���Ϸ�������������" << endl;
                break;
            }
        }
    } while (valid == 0);
    return s;
}

//�������string���ͱ��ʽת��Ϊ�ṹ�����飨���������������
EX *Str2Ex(string s)
{
    double temp0 = 0, temp1 = 0;
    int pos = 0, times = 1;
    int flag = 0; //0:���� 1:������������ 2:����С������
    EX *res = new EX[MAXLEN];
    for (string::iterator it = s.begin(); it <= s.end(); it++)
    {
        if (isinclude(*it, base_num)) //����������������������
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
        else if (*it == '.') //�����С������ʼ����С������
        {
            flag = 2;
        }
        else if (*it == ' ') //���Կո�
        {
            continue;
        }
        else //����������
        {
            //�ȴ�������
            if (flag)
            {
                res[pos].val = temp0 + temp1;
                res[pos++].flag = 1;
                flag = 0;
                times = 1;
                temp0 = temp1 = 0; //��ʼ���洢���ֵ���ʱ����
            }
            res[pos].op = *it;   //���뵱ǰ������
            res[pos++].flag = 2; //posָ����һ��λ��
        }
    }
    res[pos].flag = 0; //ĩβ��־
    return res;
}

//����׺���ʽת��Ϊ��׺���ʽ
/*
    ����׺���ʽ������������ɨ�裬���ڲ�������˳�򱣳ֲ��䣬������������ʱֱ�������
    Ϊ��������˳������һ��ջ���Ա����������ɨ�赽������ʱ����������ѹ��ջ�У�
        ��ջ��ԭ���Ǳ���ջ�������������ȼ�Ҫ����ջ�����������������ȼ���
        ���򣬽�ջ��������������ջ�������ֱ������Ҫ��Ϊֹ��
    ���� ������ ��ջ�������� ������ ʱ����ջ���ֱ�� ������ Ϊֹ
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
        if (infix[i].flag == 1)  //���������
        {
            suffix[pos++] = infix[i];
        }
        else  //����������
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
            else  //��������������������ջ���������ȼ���
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

//�Ա��ʽ�еĸ���/���Ž��е�������
EX *Neg_Process(EX *exp)
{
    EX *infix = new EX[MAXLEN];
    int i = 0;
    int pos = 0;
    while (exp[i].flag != 0)
    {
        if (exp[i].flag == 2 && exp[i].op == '-') //����Ǹ���
        {
            if (i == 0) //���λ����λ
            {
                if (exp[i + 1].flag == 1) //�������Ϊ����
                //����Խ�磿��
                {
                    exp[++i].val *= -1;
                    infix[pos++] = exp[i];
                }
                else if (exp[i + 1].flag == 2 && exp[i + 1].op == '(') //�������Ϊ������
                {
                    EX tmp1;
                    tmp1.val = 0;
                    tmp1.flag = 1;
                    infix[pos++] = tmp1;
                    infix[pos++] = exp[i];
                }
            }
            else if (exp[i - 1].flag == 2 && exp[i - 1].op == '(') //���ǰ����������
            //����Խ�磿��
            {
                if (exp[i + 1].flag == 1) //�������Ϊ����
                //����Խ�磿��
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

//�����׺���ʽ�Ľ��
/*
    �Ժ�׺���ʽ������������ɨ�裬����������ʱ������������ջ���棻 
    ������������ʱ����ջ���˳�����������������Ӧ���㣬����������ջ���棻ֱ�����ʽ������ջ��ΨһԪ�ؼ�Ϊ���ʽ��ֵ��
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
                num.push(n2 - n1); //ע�����������Ĵ���
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

//����ʼ
void start()
{
    while (1) //ѭ��
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

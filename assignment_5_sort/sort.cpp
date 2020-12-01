/*
1190201520 李想

实现经典的排序算法，通过实验数据的设计，考察不同规模和分布的数据对排序算法运行时间影响的规律，验证理论分析结果的正确性。 
要求：实现以下四组排序算法中的任意三组： 
冒泡排序和快速排序； 
直接选择排序和堆排序。 
插入排序和希尔排序； 
（二路）归并排序, 基数排序；
产生不同规模和分布的数据，以“图或表”的方式给出输入规模和分布对排序方法运行时间变化趋势的影响（画出T(n)的曲线）。
并与理论分析结果比较。 
将上述“图或表”采用图片等形式贴在报告中，并作适当分析或说明。
*/

#include "sort.h"

using namespace std;

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}

/*
冒泡排序

时间复杂度:
    最好情况：O(n)
    最坏情况：O(n^2)
    平均情况：O(n^2)
空间复杂度:O(1)

就是自底向上检查一遍这个序列，比较相邻元素的关键字的顺序是否正确，不正确则交换顺序，经过一轮遍历后，最“轻”的元素到达序列末尾（位置正确）。
再对该元素之前的序列重新进行遍历，反复执行后，序列变为有序。
*/
void Bubble_sort(int *A, int i, int j)
{
    bool flag = false;
    for (int m = j; m >= i; m--)
    {
        for (int n = i; n < m; n++)
        {
            if (A[n] > A[n + 1])
            {
                swap(&A[n], &A[n + 1]);
                flag = true;
            }
        }
        if (!flag) //如果没有发生交换，则说明已经有序，直接返回
        {
            return;
        }
    }
    return;
}

/*
直接选择排序

时间复杂度:
    最好情况：O(n^2)
    最坏情况：O(n^2)
    平均情况：O(n^2)
空间复杂度:O(1)

每趟排序在当前待排序序列中选出关键字值最小（最大）的记录，添加到有序序列中：
    对待排序的记录序列进行n-1遍的处理：第1遍处理是将A[1…n]中最小者与A[1]交换位置，第2遍处理是将A[2…n]中最小者 与A[2]交换位置，......，第i遍处理是将A[i…n]中最小者与A[i]交换位置。
    这样，经过i遍处理之后，前i个记录的位置就已经按升序排列好了。
*/
void Straight_Select_sort(int *A, int i, int j)
{
    for (int m = j; m > i; m--) //m为待排序部分的末尾
    {
        int max_index = i;
        for (int n = i; n < m; n++) //找出最大元素（的下标）
        {
            if (A[n] > A[max_index])
            {
                max_index = n;
            }
        }
        if (max_index != m) //交换
        {
            swap(&A[max_index], &A[m]);
        }
    }
    return;
}

/*
插入排序

时间复杂度:
    最好情况：O(n)
    最坏情况：O(n^2)
    平均情况：O(n^2)
空间复杂度:O(1)

每次将一个待排序的记录按其关键字的大小插入到一个已经排好序的有序序列中，直到全部记录排好序为止。
*/
void Insert_sort(int *A, int i, int j)
{
    for (int m = i + 1; m <= j; m++)
    {
        int n = m;
        while (n > i && A[n] < A[n - 1])
        {
            swap(&A[n], &A[n - 1]);
            n--;
        }
    }
    return;
}

/*
希尔排序

时间复杂度：O(nlog_2 n) ~ O(n^2)
空间复杂度：O(1)

将整个待排序记录分割成若干个子序列，在子序列内分别进行直接插入排序，
待整个序列中的记录基本有序时，对全体记录进行直接插入排序。
*/
void Shell_sort(int *A, int i, int j)
{
    int num = j - i + 1;                  //待排序元素数量
    for (int d = num / 2; d >= 1; d /= 2) //子序列内间距递减
    {
        for (int m = i + d; m <= j; m++) //将A[m]插入到所属的子序列中 （m=d+1相当于从同一子序列的“第2个”开始）
        {
            int temp = A[m]; //暂存待插入记录
            int n = m - d;   //n指向所属子序列的最后一个记录
            while (n >= i && temp < A[n])
            {
                A[n + d] = A[n]; //记录后移d个位置
                n -= d;          //指向同一子序列的前一个记录
            }
            A[n + d] = temp; //在正确位置插入(因为前面多减了一次，所以要+d)
        }
    }
    return;
}

void Merge(int *A, int *B, int low, int mid, int high)
{
    //i:B前半部分，j:B后半部分 k:A
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high) //两个序列非空时，取较小的值输出到A上
    {
        A[k++] = (B[i] <= B[j]) ? B[i++] : B[j++];
    }
    while (i <= mid) //处理B剩余的前半部分
    {
        A[k++] = B[i++];
    }
    while (j <= high) //处理B剩余的后半部分
    {
        A[k++] = B[j++];
    }
    return;
}

//二路归并 递归部分（重载）
void Merge_sort(int *A, int *B, int low, int high)
{
    int mid = (low + high) / 2;
    if (low < high)
    {
        Merge_sort(B, A, low, mid);
        Merge_sort(B, A, mid + 1, high);
        Merge(A, B, low, mid, high);
    }
    return;
}

/*
（二路）归并排序

时间复杂度：O(nlog_2 n)
空间复杂度：O(n)

分解：将当前待排序的序列A[low], …, A[high]一分为二，即求分裂点 mid = ( low + high ) / 2 ；
求解：递归地对序列 A[low], …, A[mid]和A[mid+1], …, A[high]进行归并排序；
组合：将两个已排序子序列归并为一个有序序列。
*/
void Merge_sort(int *A, int i, int j)
{
    int *B = new int[MAX_SIZE];
    memcpy(B, A, sizeof(int) * MAX_SIZE);
    Merge_sort(A, B, i, j);
    return;
}

int Find_Pivot_index(int *A, int i, int j)
{
    int firstkey = A[i]; //第一个数的值
    for (int k = i + 1; k <= j; k++)
    {
        if (A[k] > firstkey) //如果比第一个数的值大则返回当前的下标
            return k;
        else if (A[k] < firstkey) //如果比第一个数的值小则返回第一个数的下标
            return i;
    }
    return -1; //如果全部相等则返回-1
}

int Partition(int *A, int i, int j, int pivotval)
{
    // int l, r;
    // do
    // {
    //     for (l = i; A[l] < pivotval; l++)
    //         ;
    //     for (r = j; A[r] >= pivotval; r--)
    //         ;
    //     if (l < r)
    //         swap(&A[l], &A[r]);
    // } while (l <= r);
    int l = i, r = j;
    do
    {
        for (; A[l] < pivotval; l++)
            ;
        for (; A[r] >= pivotval; r--)
            ;
        if (l < r)
            swap(&A[l], &A[r]);
    } while (l <= r);
    return l; //返回右子序列的起始下标
}

/*
快速排序

最好情况：每一次划分后，划分点的 左侧子表与右侧子表的长度相同
    时间复杂度为O(nlog2n) 空间复杂度为O(log2n)
最坏情况：每次划分只得到一个比上一次划分少一个记录的子序列（另一个子序列长度为1）
    时间复杂度为O(n2) 空间复杂度为O(n)

[分治方法]
设被排序的无序区为A[i],……,A[j] 
1. 基准元素选取：选择其中的一个记录的关键字 v 作为基准元素(控制关键字);
2. 划分：通过基准元素 v 把无序区A[i],……,A[j]划分成左、右两部分，使得左边的各记录的关键字都小于 v ，右边的各记录的关键字都大于等于 v ；
3. 递归求解：重复 (1)~(2)，分别对左边和右边部分递归地进行快速排序； 
4. 组合：左、右两部分均有序，整个序列有序。
*/
void Quick_sort(int *A, int i, int j)
{
    if (i == j)
    {
        return;
    }
    int pivot_index = Find_Pivot_index(A, i, j);
    if (pivot_index != -1)
    {
        int l = Partition(A, i, j, A[pivot_index]);
        Quick_sort(A, i, l - 1); //对左子序列进行快排
        Quick_sort(A, l, j);     //对右子序列进行快排
    }
    return;
}

void Pushdown(int *A, int first, int last)
{
    if (first == last)
    {
        return;
    }
    // 以0为起点的形式
    int r = first;

    while (r <= (last - 1) / 2)
    {
        if (r == (last - 1) / 2 && (last - 1) % 2 == 0) //度为1的节点(只有左孩子）
        {
            if (A[r] < A[2 * r + 1])
            {
                swap(A[r], A[2 * r + 1]);
            }
            r = last; //直接退出while循环
        }
        else if (A[r] < A[2 * r + 1] && A[2 * r + 1] >= A[2 * r + 2]) //左孩子比右孩子大,与左孩子交换
        {
            swap(A[r], A[2 * r + 1]);
            r = 2 * r + 1;
        }
        else if (A[r] < A[2 * r + 2] && A[2 * r + 2] > A[2 * r + 1]) //右孩子比左孩子大，与右孩子交换
        {
            swap(A[r], A[2 * r + 2]);
            r = 2 * r + 2;
        }
        else //？什么情况
        {
            r = last;
        }
    }
    return;
}

/*
堆排序

最好情况：O(nlog_2 n)
最坏情况：O(nlog_2 n)
空间复杂度 O(1)

由待排序的记录序列建立堆（根据升序/降序的排列顺序，建立最大/最小堆）
堆顶元素与当前堆中下标为 i (下标最大) 的元素交换
整理堆把以1为根，最大叶下标为i-1的完全二元树整理成堆，直到堆顶为止
*/
void Heap_sort(int *A, int i, int j)
{
    for (int k = (j - 1) / 2; k >= 0; k--) //初始建立最大堆
    {
        Pushdown(A, k, j);
    }
    //排序（最大堆->升序）
    for (int k = j; k >= 1; k--) //k减小相当于堆的大小减小（删去末尾的已经有序的节点）
    {
        swap(A[0], A[k]);      //将堆顶元素与末尾元素交换
        Pushdown(A, 0, k - 1); //调整堆顶元素位置
    }
    return;
}

int Find_max_digits(int *A, int i, int j)
{
    int max_n = A[i];
    for (int k = i; k <= j; k++)
    {
        if (A[k] > max_n)
        {
            max_n = A[k];
        }
    }
    int digits = 0;
    max_n -= 1;
    while (max_n != 0)
    {
        max_n /= 10;
        digits++;
    }
    return digits;
}

int Radix(int i, int p)
{
    int power = pow(10, p);
    return ((i / power) % 10);
}

/*
基数排序

时间复杂度：O(d(n+r)) d:关键字个数; n:记录数; r:基数;
空间复杂度：O(n+r)


设待排序的序列的关键字都是位相同的整数（不相同时,取位数的最大值），其位数为figure，每个关键字可以各自含有figure个分量，每个分量的值取值范围为0,1,…,9即基数为10。依次从低位考查每个分量；
首先把全部数据装入一个队列A，然后按下列步骤进行:
1.初态:设置10个队列，分别为Q[0],Q[1],…,Q[9]，并且均为空 
2.分配:依次从队列中取出每个数据data；第pass遍处时，考查data.key右起第pass位数字，设其为r，把data插入队列Q[r]，取尽A，则全部数据被分配到 Q[0],Q[1],…,Q[9].
3.收集:从Q[0]开始，依次取出Q[0],Q[1],…,Q[9]中的全部数据，并按照取出顺序，把每个数据插入排队A。
4.重复1,2,3步，对于关键字中有figure位数字的数据进行figure遍处理，即可 得到按关键字有序的序列。
*/
void Radix_sort(int *A, int i, int j, int MAX)
{
    queue<int> Q[10], Q_a;
    int pass, r, temp, temp_r;
    int max_digits = Find_max_digits(A, i, j);
    for (int k = i; k <= j; k++)
    {
        Q_a.push(A[k]);
    }
    for (int pass = 0; pass < max_digits; pass++)
    {
        while (!Q_a.empty()) //分配
        {
            temp = Q_a.front();
            Q_a.pop();
            temp_r = Radix(temp, pass);
            Q[temp_r].push(temp);
        }
        for (int i = 0; i <= 9; i++)
        {
            while (!Q[i].empty())
            {
                Q_a.push(Q[i].front());
                Q[i].pop();
            }
        }
    }
    for (int k = i; k <= j; k++)
    {
        A[k] = Q_a.front();
        Q_a.pop();
    }
    return;
}

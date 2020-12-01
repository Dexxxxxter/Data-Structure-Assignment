#include "sort.h"
#include "datamgr.h"

using namespace std;

void start()
{
    string default_path = ".\\example.txt";
    int len = 0;
    int select = 0, method = 0;
    int *A = new int[MAX_SIZE];
    string file_path = default_path;
    cout << "create new dataset? [ 0:no / 1:yes]" << endl;
    cin >> select;
    if (select == 1)
    {
        file_path = make_new_data();
    }
    load_file(file_path, A, len);
    cout << "file loaded, select sorting method:" << endl;
    cout << "----------------" << endl;
    cout << "1:bubble sort\n2:quick sort\n3:straight select sort\n4:heap sort\n5:insert sort\n6:shell sort\n7:merge sort\n8:radix sort\n";
    cout << "----------------" << endl;
    cin >> method;

    time_t begin_t, end_t;
    string methodname;
    begin_t = clock();
    switch (method)
    {
    case 1:
        methodname = "Bubble sort";
        Bubble_sort(A, 0, len - 1);
        break;
    case 2:
        methodname = "Quick sort";
        Quick_sort(A, 0, len - 1);
        break;
    case 3:
        methodname = "Straight Select sort";
        Straight_Select_sort(A, 0, len - 1);
        break;
    case 4:
        methodname = "Heap sort";
        Heap_sort(A, 0, len - 1);

        break;
    case 5:
        methodname = "Insert sort";
        Insert_sort(A, 0, len - 1);
        break;
    case 6:
        methodname = "Shell sort";
        Shell_sort(A, 0, len - 1);
        break;
    case 7:
        methodname = "Merge sort";
        Merge_sort(A, 0, len - 1);
        break;
    case 8:
        methodname = "Radix sort";
        Radix_sort(A, 0, len - 1, MAX_SIZE); //最高为6位
        break;
    default:
        cout << "\n method not found, data not sorted" << endl;
        return;
    }
    end_t = clock();
    cout << methodname << " finished, using " << (end_t - begin_t) << " ms" << endl;
    return;
}

int main()
{
    start();
    system("pause");
    return 0;
}
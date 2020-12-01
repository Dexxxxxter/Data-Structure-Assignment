#include <iostream>
using namespace std;

int main()
{
    cout << sizeof(int) << endl;
    cout << sizeof(unsigned char) << endl;
    cout << sizeof(char) << endl;
    FILE *fp = fopen("C://assignment/assignment_3_2/test_text.txt", "r");
    FILE *fp2 = fopen("C://assignment/assignment_3_2/test.txt", "w+");
    unsigned char a = fgetc(fp);
    unsigned char b = fgetc(fp);
    fputc(26, fp2);
    char tmp = fgetc(fp2);
    //cout << (int)fgetc(fp2) << endl;
    cout << fgetc(fp) << fgetc(fp) << fgetc(fp) <<fgetc(fp) <<fgetc(fp) <<fgetc(fp) <<fgetc(fp) <<endl;
    system("pause");
    return 0;
}
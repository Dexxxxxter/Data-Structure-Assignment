#ifndef _DATAMGR_H_
#define _DATAMGR_H_

#include <iostream>
#include <cstdlib>
#include <random>

using namespace std;

void ascending_order(int *A, int len);
void descending_order(int *A, int len);
void random_order(int *A, int len, int seed);
void save_as_file(string file_path, int *A, int len);
void load_file(string file_path, int *A, int &len);
string make_new_data();

#endif
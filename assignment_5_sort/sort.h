#ifndef _SORT_H_
#define _SORT_H_

#include <iostream>
#include <ctime>
#include <string>
#include <queue>
#include <cmath>
#include "datamgr.h"

#define MAX_SIZE 10000000

void swap(int *a, int *b);
void Bubble_sort(int *A, int i, int j);
void Straight_Select_sort(int *A, int i, int j);
void Insert_sort(int *A, int i, int j);
void Shell_sort(int *A, int i, int j);
void Merge(int *A, int *B, int low, int mid, int high);
void Merge_sort(int *A, int *B, int low, int high);
void Merge_sort(int *A, int i, int j);
int Find_Pivot_index(int *A, int i, int j);
int Partition(int *A, int i, int j, int pivotval);
void Quick_sort(int *A, int i, int j);
void Pushdown(int *A, int first, int last);
void Heap_sort(int *A, int i, int j);
int Find_max_digits(int n);
int Radix(int i, int p);
void Radix_sort(int *A, int i, int j, int MAX);

#endif
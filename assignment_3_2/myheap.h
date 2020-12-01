#ifndef _MYHEAP_H_
#define _MYHEAP_H_

#define MAX_HEAP_SIZE 270

typedef struct heap_node
{
    char ch;
    int weight;
    int pos;
} HeapNode;

typedef struct heap
{
    HeapNode node[MAX_HEAP_SIZE];  //
    int n; //当前元素个数计数器
} Heap;
//此处使用最小堆

bool IsHeapFull(Heap *heap);
bool IsHeapEmpty(Heap *heap);
void Heap_Insert(Heap *heap, HeapNode elem);
HeapNode Heap_Delete_Min(Heap *heap);
void Select_Min_using_Heap(Heap *heap, int i, int *p1, int *p2);

bool IsHeapFull(Heap *heap)
{
    return (heap->n == MAX_HEAP_SIZE);
}

bool IsHeapEmpty(Heap *heap)
{
    // if (heap->n == 0)
    //     return true;
    // else
    //     return true;
    return (!heap->n);
}

//时间复杂度 O(logn)
void Heap_Insert(Heap *heap, HeapNode elem)
{
    int i;
    if (!IsHeapFull(heap))
    {
        i = heap->n + 1; //堆底元素
        while (i != 1 && elem.weight < heap->node[i / 2].weight) //如果比其父节点更小
        {
            heap->node[i] = heap->node[i / 2]; //将父节点下推
            i /= 2;  //移到父节点位置
        }
    }
    heap->node[i] = elem;  //i位置赋值为elem
    heap->n++; //堆中元素数量+1
}

//取出堆顶元素
HeapNode Heap_Delete_Min(Heap *heap)
{
    int parent = 1, child = 2;
    HeapNode elem, tmp;
    if (!IsHeapEmpty(heap))
    {
        elem = heap->node[1]; //储存堆顶节点
        tmp = heap->node[heap->n--]; //末尾节点
        while (child <= heap->n)
        {
            if (child < heap->n && heap->node[child].weight > heap->node[child + 1].weight)
                child++; //找最小子节点（左右儿子的小者）
            if (tmp.weight <= heap->node[child].weight)
                break;
            heap->node[parent] = heap->node[child]; //上推
            parent = child;
            child *= 2;  //转到child节点的左子节点
        }
    }
    heap->node[parent] = tmp; //腾出位置后，把最后一个节点放到适当的位置中
    return elem;
}

//找到最小的两个节点的位置并返回
void Select_Min_using_Heap(Heap *heap, int *p1, int *p2)
{
    int min1 = -1, min2 = -1;
    min1 = Heap_Delete_Min(heap).pos;
    min2 = Heap_Delete_Min(heap).pos;
    *p1 = min1;
    *p2 = min2;
    return;
}

#endif


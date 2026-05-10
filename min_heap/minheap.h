#ifndef MINHEAP_H
#define MINHEAP_H

typedef struct{ // узел кучи
    int key;
    char* val;
} heap_node;

typedef struct{
    int cap; // максимальная ёмкость
    int len; // текущее количество элементов
    heap_node* data; // массив узлов
} minheap;

int get_parent(int ind); // индекс родителя
int get_l_child(int ind); // индекс левого потомка
int get_r_child(int ind); // индекс правого потомка
void heap_swap(heap_node* a, heap_node* b); // обмен двух узлов

minheap* minheap_create(int cap); // создать пустую кучу ёмкостью cap
void minheap_free(minheap *h); // освободить память

void heapify_up(minheap* h, int ind); // просеивание вверх от ind
void heapify_down(minheap* h, int ind); // просеивание вниз от ind

heap_node* minheap_min(minheap* h); // вернуть минимум (корень) без удаления
minheap* minheap_insert(minheap* heap, int key, char* val); // вставка
minheap* minheap_extractmin(minheap* heap); // извлечь минимум
minheap* build_minheap(int* arr, int n); // построить кучу из массива за O(n)
minheap* minheap_union(minheap* heap1, minheap* heap2); // слияние двух куч в новую
minheap* minheap_decrease_key(minheap* heap, int key, int newkey); // уменьшить ключ
minheap* minheap_delete(minheap* heap, int key); // удалить элемент по ключу

#endif

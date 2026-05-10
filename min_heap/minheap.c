#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "minheap.h"

int get_parent(int ind){
    return (ind - 1) / 2; // целочисленное деление: левый и правый потомок дают одного родителя
}

int get_l_child(int ind){
    return 2 * ind + 1;
}

int get_r_child(int ind){
    return 2 * ind + 2;
}

void heap_swap(heap_node* a, heap_node* b){
    heap_node temp = *a; // копируем весь узел (key + val) во временную переменную
    *a = *b;
    *b = temp;
}

minheap* minheap_create(int cap){
    minheap* h = malloc(sizeof(minheap));

    if(h != NULL){
        h->cap = cap;
        h->len = 0;

        h->data = malloc(sizeof(heap_node) * cap);
        if(h->data == NULL){
            free(h); // не допускаем утечку: h уже выделен, data - нет
            return NULL;
        }
    }

    return h;
}

void minheap_free(minheap *h){
    if(h != NULL){
        free(h->data); // сначала массив узлов
        free(h);       // затем саму структуру
    }
}

void heapify_up(minheap* h, int ind){
    int p = get_parent(ind);

    while(ind > 0 && h->data[ind].key < h->data[p].key){ // пока не корень и меньше родителя
        heap_swap(&(h->data[ind]), &(h->data[p]));
        ind = p;
        p = get_parent(ind);
    }
    // O(log n) время, O(1) память
}

void heapify_down(minheap* h, int ind){
    while(1){
        int left = get_l_child(ind);
        int right = get_r_child(ind);
        int smallest = ind; // предполагаем текущий узел наименьшим

        if(left < h->len && h->data[left].key < h->data[smallest].key)
            smallest = left;

        if(right < h->len && h->data[right].key < h->data[smallest].key)
            smallest = right;

        if(smallest != ind){
            heap_swap(&(h->data[ind]), &(h->data[smallest]));
            ind = smallest; // спускаемся в поддерево, где нарушен порядок
        } else
            break; // оба потомка >= текущего, свойство кучи выполнено
    }
    // O(log n) время, O(1) память
}

heap_node* minheap_min(minheap* h){
    if(h->len == 0){
        return NULL;
    }

    return &(h->data[0]); // минимум всегда в корне (индекс 0)
}

minheap* minheap_insert(minheap* heap, int key, char* val){
    if(heap->len >= heap->cap){
        return heap; // куча заполнена, вставка не выполняется
    }

    heap->data[heap->len].key = key;
    heap->data[heap->len].val = val;

    heapify_up(heap, heap->len); // len ещё не увеличен - он и есть индекс нового элемента
    heap->len++;

    // O(log n) время, O(1) память
    return heap;
}

minheap* minheap_extractmin(minheap* heap){
    if(heap->len == 0){
        return heap;
    }

    heap->data[0] = heap->data[heap->len - 1]; // последний элемент перемещается в корень
    heap->len--;

    if(heap->len > 1){
        heapify_down(heap, 0);
    }

    // O(log n) время, O(1) память
    return heap;
}

minheap* build_minheap(int* arr, int n){
    minheap* h = minheap_create(n);
    if(h == NULL)
        return NULL;

    for(int i = 0; i < n; i++){
        h->data[i].key = arr[i];
        h->data[i].val = NULL;
    }

    h->len = n;

    for(int i = get_parent(n - 1); i >= 0; i--) // от последнего нелистового узла до корня
        heapify_down(h, i);

    // O(n) время, O(1) доп. память
    return h;
}

minheap* minheap_union(minheap* heap1, minheap* heap2){
    int n = heap1->len + heap2->len;
    minheap* h = minheap_create(n);
    if(h == NULL)
        return NULL;

    for(int i = 0; i < heap1->len; i++)
        h->data[i] = heap1->data[i]; // копируем узлы первой кучи

    for(int i = 0; i < heap2->len; i++)
        h->data[heap1->len + i] = heap2->data[i]; // копируем узлы второй кучи со смещением

    h->len = n;

    for(int i = get_parent(n - 1); i >= 0; i--) // от последнего нелистового узла до корня
        heapify_down(h, i); // восстанавливаем свойство кучи (алгоритм Флойда)

    // O(n) время, O(n) память (новый массив размером n)
    return h;
}

minheap* minheap_decrease_key(minheap* heap, int key, int newkey){
    int ind = -1;

    for(int i = 0; i < heap->len; i++){
        if(heap->data[i].key == key){
            ind = i; // берём первое вхождение (дубликаты не поддерживаются)
            break;
        }
    }

    if(ind == -1 || newkey >= key){ // ключ не найден или новый ключ не меньше старого
        return heap;
    }

    heap->data[ind].key = newkey;
    heapify_up(heap, ind); // ключ уменьшился - узел мог подняться вверх

    // O(n) время (линейный поиск), O(1) память
    return heap;
}

minheap* minheap_delete(minheap* heap, int key){
    int ind = -1;

    for(int i = 0; i < heap->len; i++){
        if(heap->data[i].key == key){
            ind = i; // берём первое вхождение
            break;
        }
    }

    if(ind == -1)
        return heap;

    heap->data[ind].key = INT_MIN; // ставим -∞ чтобы узел всплыл в корень
    heapify_up(heap, ind);
    minheap_extractmin(heap); // удаляем корень

    // O(n) время (линейный поиск), O(1) память
    return heap;
}

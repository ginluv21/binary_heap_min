#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "experiment.h"
#include "../min_heap/minheap.h"

#define KEY_MIN   10000
#define KEY_MAX   1000000
#define REPEAT    1000    /* таблица 1 */
#define REPEAT_T2 1000    /* таблица 2 */

double wtime(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

static int* gen_array(int n){
    int* arr = malloc(sizeof(int) * n);
    for(int i = 0; i < n; i++)
        arr[i] = KEY_MIN + rand() % (KEY_MAX - KEY_MIN + 1);
    return arr;
}

void measure_table1(int n, double* t_insert, double* t_build){
    int* arr = gen_array(n);
    double t;

    /* Один malloc для обоих замеров — одинаковые условия */
    minheap* h = minheap_create(n);

    /* --- minheap_insert: N вставок в пустую кучу --- */
    t = wtime();
    for(int r = 0; r < REPEAT; r++){
        h->len = 0;
        for(int i = 0; i < n; i++)
            minheap_insert(h, arr[i], NULL);
    }
    *t_insert = (wtime() - t) / REPEAT;

    /* --- build_minheap: копируем данные + алгоритм Флойда ---
       malloc тоже один раз вне цикла — те же условия, что у insert */
    t = wtime();
    for(int r = 0; r < REPEAT; r++){
        for(int i = 0; i < n; i++){
            h->data[i].key = arr[i];
            h->data[i].val = NULL;
        }
        h->len = n;
        for(int i = get_parent(n - 1); i >= 0; i--)
            heapify_down(h, i);
    }
    *t_build = (wtime() - t) / REPEAT;

    minheap_free(h);
    free(arr);
}

void measure_table2(int n, double* t_extractmin, double* t_decrease){
    int* arr = gen_array(n);
    double total;

    /* По методичке: строим кучу из N элементов — это не замеряем.
       Snapshot позволяет восстанавливать кучу без пересборки. */
    minheap* h = build_minheap(arr, n);
    heap_node* snap_full = malloc(n * sizeof(heap_node));
    memcpy(snap_full, h->data, n * sizeof(heap_node));

    /* --- extractmin ---
       Замеряем одно извлечение минимума на полной куче */
    total = 0;
    for(int r = 0; r < REPEAT_T2; r++){
        memcpy(h->data, snap_full, n * sizeof(heap_node));
        h->len = n;
        double t0 = wtime();
        minheap_extractmin(h);
        total += wtime() - t0;
    }
    *t_extractmin = total / REPEAT_T2;

    /* --- decrease_key ---
       По методичке: N элементов + одно extractmin, затем decrease_key */
    memcpy(h->data, snap_full, n * sizeof(heap_node));
    h->len = n;
    minheap_extractmin(h); /* предварительное извлечение — не замеряем */

    int after_len = h->len;
    heap_node* snap_after = malloc(after_len * sizeof(heap_node));
    memcpy(snap_after, h->data, after_len * sizeof(heap_node));

    /* ключ из середины массива — стабильный O(n/2) поиск каждый раз */
    int target_key = snap_after[after_len / 2].key;

    total = 0;
    for(int r = 0; r < REPEAT_T2; r++){
        memcpy(h->data, snap_after, after_len * sizeof(heap_node));
        h->len = after_len;
        double t0 = wtime();
        minheap_decrease_key(h, target_key, target_key - 1);
        total += wtime() - t0;
    }
    *t_decrease = total / REPEAT_T2;

    free(snap_full);
    free(snap_after);
    minheap_free(h);
    free(arr);
}

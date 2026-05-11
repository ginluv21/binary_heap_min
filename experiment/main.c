#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "experiment.h"

int main(){
    srand((unsigned int)time(NULL));

    int sizes[] = {50000, 100000, 150000, 200000, 250000, 300000, 350000, 400000};
    int m = (int)(sizeof(sizes) / sizeof(sizes[0]));

    printf("Таблица 1. Добавление элементов в структуру данных\n");
    printf("+----+-----------+----------------------+----------------------+\n");
    printf("| #  | N         | minheap_insert, с    | build_minheap, с     |\n");
    printf("+----+-----------+----------------------+----------------------+\n");
    for(int i = 0; i < m; i++){
        double ti, tb;
        measure_table1(sizes[i], &ti, &tb);
        printf("| %-2d | %-9d | %-20.9f | %-20.9f |\n", i + 1, sizes[i], ti, tb);
    }
    printf("+----+-----------+----------------------+----------------------+\n");

    printf("\nТаблица 2. Извлечение и уменьшение приоритета элемента\n");
    printf("+----+-----------+------------------------+------------------------+\n");
    printf("| #  | N         | minheap_extractmin, с  | minheap_decreasekey, с |\n");
    printf("+----+-----------+------------------------+------------------------+\n");
    for(int i = 0; i < m; i++){
        double te, td;
        measure_table2(sizes[i], &te, &td);
        printf("| %-2d | %-9d | %-22.9f | %-22.9f |\n", i + 1, sizes[i], te, td);
    }
    printf("+----+-----------+------------------------+------------------------+\n");

    return 0;
}

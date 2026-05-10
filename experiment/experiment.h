#ifndef EXPERIMENT_H
#define EXPERIMENT_H

double wtime(); // wall-clock время в секундах

void measure_table1(int n, double* t_insert, double* t_build);
void measure_table2(int n, double* t_extractmin, double* t_decrease);

#endif

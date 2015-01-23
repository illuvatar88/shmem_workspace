/*
 * pi_lock.cpp
 *
 *  Created on: 23-Jan-2015
 *      Author: sony
 */

#include <iostream>
#include <iomanip>
#include <shmem.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

#define MASTER 0
#define ITER 10000

long lock = 0;

double partial_summation (int N);
void final_summation (double* pi_temp, double* pi);
double timerval ();

int main (int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "N not specified\n";
        return 1;
    }
    int N = atoi (argv[1]);
    start_pes(0);
    double* pi = (double*) shmalloc (sizeof(double));
    double* pi_temp = (double*) shmalloc (sizeof(double));
    double start_time = timerval();
    for (int i = 1 ; i <= ITER ; i++) {
        *pi_temp = partial_summation (N);
        final_summation (pi_temp, pi);
    }
    double end_time = timerval();
    if (_my_pe() == MASTER) {
        cout << "Value of pi : " << std::setprecision (51) << *pi << endl;
        cout << "Avg time taken : " << std::setprecision (8)
        << (end_time - start_time) * 1e6 / ITER << " us" << endl;
    }
    shfree(pi);
    return 0;
}

double partial_summation (int N) {
    int num_pe = _num_pes();
    double pi = 0;
    for (int k = _my_pe() ; k <= N ; k += num_pe) {
        pi += (4 / double(8 * k + 1) - 2 / double(8 * k + 4)
                - 1 / double(8 * k + 5) - 1 / double(8 * k + 6))
                / double(pow (16, k));
    }
    return pi;
}

void final_summation (double* pi_temp, double* pi) {
    double* pWrk = (double*) shmalloc (sizeof(double) * _SHMEM_REDUCE_MIN_WRKDATA_SIZE);
    long* pSync = (long*) shmalloc (sizeof(long) * _SHMEM_REDUCE_SYNC_SIZE);
    for (int i = 0 ; i < _SHMEM_REDUCE_SYNC_SIZE ; i++)
        pSync[i] = _SHMEM_SYNC_VALUE;
    shmem_barrier_all();
    cerr << "Check 1\n";
    shmem_double_sum_to_all(pi, pi_temp, 1, 0, 1, _num_pes(), pWrk, pSync);
}

double timerval () {
struct timespec st;
clock_gettime (CLOCK_REALTIME, &st);
return st.tv_sec + st.tv_nsec * 1e-9;
}

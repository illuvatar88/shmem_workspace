using namespace std;

#include <iostream>
#include <shmem.h>

int main () {
	start_pes(0);

	int my_pe = _my_pe();
	int r_pe = _num_pes() - my_pe - 1;

    cout << "pe " << my_pe << " of " << _num_pes() << " says hello" << endl;

    int* msg = (int*) shmalloc (sizeof(int));

    shmem_int_put (msg, &my_pe, sizeof(int), r_pe);

    shmem_barrier_all();

    cout << "pe " << my_pe << " received hello from " << *msg << endl;

    shfree(msg);

    return 1;
} 

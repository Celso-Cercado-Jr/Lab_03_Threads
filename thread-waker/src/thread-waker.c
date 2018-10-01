#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t *sem;


int main(void) {

	int numWakeup = 0;
	//sem_init(sem, 0 ,0);// initialize the sema
	sem = sem_open("mysem",0);

	printf("Thread-Waker PID = %d\n", getpid());
	do{
		printf("Enter the number of threads to wakeup (enter 0 to exit): \n"); // ask user to threads to wake up
		scanf("%d", &numWakeup);

		for(int i = 0; i < numWakeup; i++){
			sem_post(sem); //
		}

	}while(numWakeup != 0); // if zero

	sem_close(sem);
	sem_destroy(sem);
	return EXIT_SUCCESS;

}



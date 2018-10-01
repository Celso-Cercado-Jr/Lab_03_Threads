
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

void handler();		// handles interrupt - returns nothing.
void* childThread(void* arg);			// handles interrupt - returns nothing.
sig_atomic_t usr1Happened = 0;	// notify if signal is sent, 1, or is not sent 0;
sem_t *sem;				// pointer to a semaphore



int main(void) {

	struct sigaction action;// action structure of the interrupt
	int numThreads = 0;
	int error = 0;//set it to no error
	sem_t *errorpoint;
	pthread_attr_t attr;

	action.sa_flags = 0;							// set the signal flag to 0
	sigemptyset(&action.sa_mask);
	action.__sa_un._sa_handler = handler;	// set the signal handler

	//install signal and check for error
	error = sigaction(SIGUSR1, &action, NULL);
	if (error != 0)
	{
		perror("signal installed");
		exit(1);
	}


	printf("Enter the number of threads to create:");// promt user how many threads to create
	scanf("%d", &numThreads);

	sem = sem_open("mysem", O_CREAT, S_IWOTH, 0);
	errorpoint = sem;

	if (errorpoint == SEM_FAILED)
	{
		perror("sem_open");
		exit(1);
	}


	for (int i = 0; i < numThreads; i++) {

		error = pthread_attr_init(&attr);
		if (error != 0)
		{
			perror("thread init");
			exit(1);
		}
		error = pthread_create(NULL, &attr, &childThread, NULL);
		if (error != 0)
		{
			perror("thread create");
			exit(1);
		}
		error = pthread_attr_destroy(&attr);
		if (error != 0)
		{
			perror("thread destroy");
			exit(1);
		}
	}

	while(usr1Happened != 1){}

	error = sem_close(sem);
	if (error != 0)
	{
		perror("semaphore close");
		exit(1);
	}

	error = sem_destroy(sem);
	if (error != 0)
	{
		perror("Semaphore close");
		exit(1);
	}

	return EXIT_SUCCESS;
}

void handler() {
	usr1Happened = 1;	//sets variable to value of signal received
}


void* childThread(void* arg) {
	printf("Child thread %d created\n", pthread_self());
	int error = 0;
	while(1)
	{
		error = sem_wait(sem);
		if (error == 1)
		{
			perror("Error-Thread not created"); // error if not created
			exit(1);
		}
		printf("Child Thread %d unblocked\n", pthread_self());
		sleep(5);
	}
	return NULL;
}

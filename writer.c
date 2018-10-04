#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/resource.h>

/*
 * Writer Program
 * Lab 5
 *
 * @author Kasey Stowell + Kehlsey Lewis
 * @version October 2018
 *
 */
#define MAX 4096

// number processes performing read in crit section
int turn;

int main() {
    // creates shared memory segment
    // repeatedly accepts an arbitrary user input string from terminal
    // writes string into shared memory

	int shmId;
	char *shmPtr;
	key_t key;
	//char *message[MAX];
	struct Sharedmem {
		bool turn;
		char message[1024];
	};
	struct Sharedmem shared;
	shared.turn = false;

    //generate a unique key 
	key = ftok("shmkey",65); 

	if((shmId = shmget(key, 4096, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0 ) {
		perror("Error creating shared memory\n");
		exit(1);
	} 

	if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
		perror("Can't attach\n");
		exit(1);
	}

	while(1) {
		// request the critical section
		printf("shared: %d\n", shared.turn);
		while (shared.turn == 1) {
			// not time for the writer, check if token is changed.
			memcpy(&shared, shmPtr, sizeof(shared));
		}
		// enter critical section
		shared.turn = true;
		printf("Enter a message: \n" ); 
		scanf("%s", shared.message);
		// leave critical section
		printf("shared: %d\n", shared.turn);
		printf("Message written to memory: %s\n", shared.message);
		sleep(1);
	};

	//flag for knowing if string was read
	//flag = (char*) shmat(shmId,(void*)0,0);


	// printf("Enter a message: \n" ); 
	// scanf("%s", message);
	// printf("Message written to memory: %s\n", message);


    //detach   
	if (shmdt (shmPtr) < 0) {
		perror ("just can't let go\n");
		exit (1);
	}

	if (shmctl (shmId, IPC_RMID, 0) < 0) {
		perror ("can't deallocate\n");
		exit(1);
	}

	return 0; 
}

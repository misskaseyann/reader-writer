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
 * Reader Program
 * Lab 5
 * 
 * @author Kasey Stowell + Kehlsey Lewis
 * @version October 2018
 *
 */

int main() {
    // read new string from shared memory
    // display string on screen

	key_t key;
	int shmId;
	char *shmPtr; //, *message;
	struct Sharedmem {
		bool turn;
		char message[1024];
	};
	struct Sharedmem shared;
	shared.turn = false;

    //generate a unique key 
	key = ftok("shmkey",65); 

    //returns an identifier in shmid 
	if ((shmId = shmget(key, 4096, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
		perror("shared memory error");
		exit(1);
	} 

	// shmat to attach to shared memory  
	if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
		perror("Can't attach\n");
		exit(1);
	}

	while(1) {
		// request critical section
		while(!shared.turn) {
			//not time for the reader, check if token is changed.
			memcpy(&shared, shmPtr, sizeof(shared));
		}
		shared.turn = true;
		// enter critical section
		printf("Read from memory: %s\n", shared.message);
		// leave critical section
		printf("shared: %d\n", shared.turn);
	};
    

	//flag for knowing if string was read
	//flag = (char*) shmat(shmId,(void*)0,0);

	// detach from memory
	if (shmdt (shmPtr) < 0) {
		perror ("just can't let go\n");
		exit (1);
	}

	// destroy shared memory
	if (shmctl (shmId, IPC_RMID, 0) < 0) {
		perror ("can't deallocate\n");
		exit(1);
	}

	return 0; 
}
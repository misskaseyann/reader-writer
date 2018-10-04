#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void sigHandler(int);

/*
 * Reader Program
 * Lab 5
 * 
 * @author Kasey Stowell + Kehlsey Lewis
 * @version October 2018
 *
 */

key_t key;
int shmId;
char *shmPtr;
typedef struct {
		int turn;
		char message[1024];
	} SharedData;

int main() {
    // read new string from shared memory
    // display string on screen
	SharedData sdata;
	sdata.turn = 0;
	signal(SIGINT, sigHandler);

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
		while(!sdata.turn) {
			//not time for the reader, check if token is changed.
			memcpy(&sdata, shmPtr, sizeof(SharedData));
		}
		// enter critical section
		printf("Read from memory: %s\n", sdata.message);
		// leave critical section
		sdata.turn = 0;
		memcpy(shmPtr, &sdata, sizeof(SharedData));
	};

	return 0; 
}

void sigHandler(int sigNum) {
	printf(" received. That's it, I'm shutting you down...\n");
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
	exit(0);
}
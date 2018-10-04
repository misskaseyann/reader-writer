#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/*
 * Writer Program
 * Lab 5
 *
 * @author Kasey Stowell + Kehlsey Lewis
 * @version October 2018
 *
 */

void sigHandler(int);

key_t key;
int shmId;
char *shmPtr;
typedef struct {
		int turn;
		char message[1024];
	} SharedData;

int main() {
	SharedData sdata;
	sdata.turn = 0;
	signal(SIGINT, sigHandler);

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

	while(strcmp(sdata.message, "exit") != 0) {
		// request the critical section
		while (sdata.turn) {
			// not time for the writer, check if token is changed.
			memcpy(&sdata, shmPtr, sizeof(SharedData));
		}
		// enter critical section
		printf("Enter a message: \n" ); 
		scanf("%s", sdata.message);
		// leave critical section
		printf("Message written to memory: %s\n", sdata.message);
		sdata.turn = 1;
		memcpy(shmPtr, &sdata, sizeof(SharedData));
	};

	kill(getpid(),SIGINT);

	return 0; 
}

/* Shut down process */
void sigHandler(int sigNum) {
	printf("That's it, I'm shutting you down...\n");
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
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/*
 * Writer Program
 * Lab 5
 *
 * @author Kasey Stowell + Kehlsey Lewis
 * @version October 2018
 *
 */
#define MAX

int main() {
    // creates shared memory segment
    // repeatedly accepts an arbitrary user input string from terminal
    // writes string into shared memory

	int shmId;
	char *shmPtr;
	key_t key;
	char *message[MAX];

    //generate a key 
	key = ftok("here",'k'); 

	if((shmId = shmget(key, 4096, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0 ) {
		perror("Error creating shared memory\n");
		exit(1);
	} 

	if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
		perror("Can't attach\n");
		exit(1);
	}

	//flag for knowing if string was read
	//flag = (char*) shmat(shmId,(void*)0,0);


	printf("Enter a message: \n" ); 
	scanf("%s", message);


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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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
	char *shmPtr, *message;


    //generate key 
	key = ftok("here",'k'); 

	printf("here\n");

    //returns an identifier in shmid 
	if ((shmId = shmget(key,4096,S_IRUSR|S_IWUSR)) < 0){
		perror("shared memory error");
		exit(1);
	} 

	if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
		perror("Can't attach\n");
		exit(1);
	}


	printf("reader ready to go!\n");
    // shmat to attach to shared memory  

	//flag for knowing if string was read
	//flag = (char*) shmat(shmId,(void*)0,0);


	if (shmdt (shmPtr) < 0) {
		perror ("just can't let go\n");
		exit (1);
	}

	if (shmctl (shmId, IPC_RMID, 0) < 0) {
		perror ("can't deallocate\n");
		exit(1);
	}

	printf("reader read: %s\n", message);

	return 0; 
}
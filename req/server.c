#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/shm.h>

///////   /////////////  \\\\\\/
///////   G L O B A L S  \\\\\\/
///////   /////////////  \\\\\\/

int rcvfromClient = 0;


struct sharedMemory { 

    char buff[100]; 
    pid_t clientpid, serverpid; 
}; 

struct sharedMemory* shmaddr = NULL;






/* convert upper case to lower case or vise versa */
void conv(char *msg)
{
    int size = strlen(msg);
    for (int i = 0; i < size; i++)
        if (islower(msg[i]))
            msg[i] = toupper(msg[i]);
        else if (isupper(msg[i]))
            msg[i] = tolower(msg[i]);
}

void handler(int signum){
    printf("\n Ana gowa handler el server\n");

    if (signum == SIGUSR1){
        /*if (shmaddr =! NULL){
        conv(shmaddr->buff);

        printf("\nData found---------------------------------- = %s\n",shmaddr->buff);

        kill(shmaddr->clientpid,SIGUSR2);
        rcvfromClient = 1;
    }
        else{
            printf("\nStill Nulllllllll");
        }*/

    }
}

int main()
{
    // Server code
    pid_t srvPID = getpid();
    int shmid;
    key_t key = 5000;

    // declaring the signal used by the client to inform the server that the client has write a msg 
    // SIGUSR1 >>> Server
    signal(SIGUSR1, handler);


    // create shared memory segment
    shmid = shmget(key, sizeof(struct sharedMemory), IPC_CREAT|0644);

    if(shmid == -1){
        perror("Error in create");
        exit(-1);
    }

    else{
        printf("\nShared memory -- Server ID = %d\n", shmid);
    }
    
    
    // attach the Server segment to our space
    shmaddr = (struct sharedMemory*) shmat(shmid, (void *)0, 0);
      if(shmaddr == -1)
    {
        perror("Error in attach in reader");
        exit(-1);
    }
    printf("\nShared memory -- Server attached at address %x\n", shmaddr);

    //sleep(10);

    // prcess id of server
    shmaddr->serverpid = srvPID;

    while(1){
        // read client input and process it
        printf("\nData found = %s\n",shmaddr->buff);

        
        // conver the msg // processing
        //conv(shmaddr->buff);
        //kill(shmaddr->clientpid, SIGUSR2);


    }
    

    
    // send signal :: send to client that msg finished processing


    // detach server
    shmdt((void*)shmaddr); 

    return 0;
}

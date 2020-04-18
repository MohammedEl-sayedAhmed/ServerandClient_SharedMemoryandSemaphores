#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <sys/stat.h>
#include <sys/file.h>
#include <sys/shm.h>
#include <signal.h> 

///////   /////////////  \\\\\\/
///////   G L O B A L S  \\\\\\/
///////   /////////////  \\\\\\/
int rcvfromServer = 0;
int isExit = 0;


struct sharedMemory { 

    char buff[100]; 
    int clientpid, serverpid; 
}; 

struct sharedMemory* shmaddr; 



void handler(int signum){

    if (signum == SIGUSR2){

        printf("\nThe server has finished processing tha data  = %s\n",shmaddr->buff);
        rcvfromServer = 1;
    }
}

void exitHandler(int signum){
    
    isExit = 1;
}
    

        
int main()
{
    // Client code
    pid_t cliPID = getpid();
    int shmid;
    key_t key = 5000;

    
    // create shared memory segment
    shmid = shmget(key, sizeof(struct sharedMemory), IPC_CREAT|0644);

    if(shmid == -1){
        perror("Error in create");
        exit(-1);
    }

    else{
        printf("\nShared memory -- Client ID = %d\n", shmid);
    }
    
    
    // attach the Cleint segment to our space
    shmaddr = (struct sharedMemory*) shmat(shmid, (void *)0, 0);
      if(shmaddr == -1)
    {
        perror("Error in attach in reader");
        exit(-1);   
    }
    printf("\nShared memory -- Client attached at address %x\n", shmaddr);

    // prcess id of server
    shmaddr->clientpid = cliPID;


    // declaring the signal used by the client to inform the server that the client has write a msg 
    // SIGUSR2 >>> Client
    signal(SIGUSR2, handler);
    signal(SIGINT,exitHandler);

    printf("Enter your message: ");


    while(isExit ==0){
        //sleep(2);

        scanf("%s", shmaddr->buff);
        kill(shmaddr->serverpid , SIGUSR1);
        while (rcvfromServer != 1){
            
        }
        rcvfromServer = 0;
        printf("Enter your message: ");
    }

    // detach client
    shmdt((void*)shmaddr);
    // clear resources
    shmctl(shmid, IPC_RMID, NULL);
    exit(0);
    
   // if(!strcmp((char*) shmaddr, "quit")) >>> hay2fl
    strcpy((char*) shmaddr, "Initial string!");
    return 0;
}

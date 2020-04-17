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


struct sharedMemory { 

    char buff[100]; 
    int clientpid, serverpid; 
}; 

struct sharedMemory* shmaddr; 






/* convert upper case to lower case or vise versa */
/*void conv(char *msg)
{
    int size = strlen(msg);
    for (int i = 0; i < size; i++)
        if (islower(msg[i]))
            msg[i] = toupper(msg[i]);
        else if (isupper(msg[i]))
            msg[i] = tolower(msg[i]);
}*/

void handler(int signum){

    if (signum == SIGUSR2){
        printf("\nana gowa handleer el client\n");
    }
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

    while(1){
        printf("Enter your message: ");
        scanf("%s", shmaddr->buff);
        kill(shmaddr->serverpid , SIGUSR1);

    }
    

    // send signal :: client wrote data on shared memory

    // detach client
    shmdt((void*)shmaddr); 
    
    return 0;
}

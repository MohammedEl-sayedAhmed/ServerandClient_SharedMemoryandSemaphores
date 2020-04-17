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





#define FILLED 0 
#define Ready 1 
#define NotReady -1 


struct msgbuff
{
    long mtype;
    char mtext[70];
};

struct memory { 
    char buff[100]; 
    int status, pidS, pidC; 
}; 
struct memory* shmaddr;

void writer(int shmid)
{
    void *shmaddr = shmat(shmid, (void *)0, 0);
    if(shmaddr == -1)
    {
        perror("Error in attach in writer");
        exit(-1);
    }
    else
    {
        printf("\nShared memory attached at address %x\n", shmaddr);
        strcpy((char*) shmaddr, "Initial string!");
    }
    while(1)
    {
        scanf("%s", (char*) shmaddr);
        if(!strcmp((char*) shmaddr, "quit"))
            break;
    }
    shmdt(shmaddr);
}


void reader(int shmid)
{
    void *shmaddr = shmat(shmid, (void *)0, 0);
    if(shmaddr == -1)
    {
        perror("Error in attach in reader");
        exit(-1);
    }
    while(strcmp((char*) shmaddr, "quit"))
    {
        sleep(5);
        printf("\nData found = %s\n", (char*) shmaddr);
    }

    printf("\nAbout to destroy the shared memory area !\n");
    shmctl(shmid, IPC_RMID, (struct shmid_ds*)0);
}

void handler(int signum) 
{ 
    // if signum is SIGUSR2, then Client  is receiving a message from Server 
  
    if (signum == SIGUSR2) { 
        printf("Received From Server: "); 
        puts(shmaddr->buff); 
    } 
} 

int main()
{
    // Client code
    int pid = getpid();

    int shmid;

    // create shared memory segment
    shmid = shmget(IPC_PRIVATE, 4096, 0644);

    if(shmid == -1){
        perror("Error in create");
        exit(-1);
    }

    else{
        printf("\nShared memory -- Client ID = %d\n", shmid);
    }
    
    
 // attach the Cleint segment to our space
    shmaddr = (struct memory*) shmat(shmid, (void *)0, 0);
      if(shmaddr == -1)
    {
        perror("Error in attach in reader");
        exit(-1);
    }
    printf("\nShared memory -- Client attached at address %x\n", shmaddr);

    // store the process id of user2 in shared memory 
    shmaddr->pidC = pid;
    shmaddr->status = NotReady;
    signal(SIGUSR2,handler);


    // add some input user string
    char inputString[256];
    

    while (1){
        sleep(1);

        printf("Enter your message: ");
        scanf("%s", shmaddr->buff);
        shmaddr->status = Ready;
        //scanf("%s",inputString);

        // sending kill to server using kill fn
        kill(shmaddr->pidS, SIGUSR1);
        while (shmaddr->status == Ready)
        {
            continue;
        }
        
    }
    shmdt((void*)shmaddr); 
    






    return 0;
}

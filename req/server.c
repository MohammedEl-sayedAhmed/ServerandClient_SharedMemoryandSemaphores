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



#define FILLED 0 
#define Ready 1 
#define NotReady -1 


struct msgbuff
{
    long mtype;
    char mtext[70];
};

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

int main()
{

    // Server code

    int shmid, pid;
    char* shm;
    char* s;

    // create shared memory segment
    shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT|0644);

    if(shmid == -1){
        perror("Error in create");
        exit(-1);
    }

    else{
        printf("\nShared memory -- Server ID = %d\n", shmid);
    }
    
    
    // attach the Server segment to our space
    void *shmaddr = shmat(shmid, (void *)0, 0);
      if(shmaddr == -1)
    {
        perror("Error in attach in reader");
        exit(-1);
    }
    printf("\nShared memory -- Server attached at address %x\n", shmaddr);

    // read client input and process it
    char inputString[256];
    printf("\nData found = %s\n", (char*) shmaddr);
    //printf("\nData found = %s\n", (char*) inputString);



    return 0;
}

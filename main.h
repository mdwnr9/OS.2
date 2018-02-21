#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h> 

#define MAX_BUF_SIZE 1024
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define KEY_CODE 1234

static volatile sig_atomic_t signalflag; 
static sigset_t newmask, oldmask, nomask; 

struct shared_mem_struct
{
	char buffer [MAX_BUF_SIZE]; 
	int count;                 
};

static void signalHandler(int signo);
void initializeWait(void);
void signalParent(pid_t pid);
void waitParent(void);
void signalChild(pid_t pid);
void waitChild(void);
void producer(char *argv, pid_t pid);
void consumer();



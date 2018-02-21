#include "main.h"

static void signalHandler(int signo)
{
	signalflag = 1;
}

void initializeWait(void)
{
	if (signal(SIGUSR1, signalHandler) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGUSR2, signalHandler) == SIG_ERR)
	{
	perror("signal");
	exit(EXIT_FAILURE);
	}

	sigemptyset(&nomask);
	sigemptyset(&newmask);

	sigaddset(&newmask, SIGUSR1);
	sigaddset(&newmask, SIGUSR2);

	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
	{
		perror("sigprocmask");
		exit(EXIT_FAILURE);
	}
}

void signalParent(pid_t pid)
{
	kill(pid, SIGUSR2); 
}

void waitParent(void)
{
	while (signalflag == 0)
	{
		sigsuspend(&nomask);    
	}

	signalflag = 0; 

	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
	{
		perror("sigprocmask");

	}
}

void signalChild(pid_t pid)
{
	kill(pid, SIGUSR1); 
}

void waitChild(void)
{
	while (signalflag == 0)
	{
		sigsuspend(&nomask);    
	}

	signalflag = 0; 

	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
	{
		perror("sigprocmask");
	}
}







int main(int argc, char * argv[])
{ 
	pid_t	pid;    

	FILE *input_file, *output_file;
	char file_buffer[MAX_BUF_SIZE];

	int smem_id;
	key_t smem_key = KEY_CODE;
	void *temp_ptr = (void *)0;
	struct shared_mem_struct *shared_mem;


	if(argc == 1)
	{
		printf("Error: Please provide input file name\n");
		exit(EXIT_FAILURE);
	}
	if(argc != 2)
	{
		printf("Error: Incorrect number of arguments passed\n");
		exit(EXIT_FAILURE);
	}


	initializeWait();   
	if ((pid = fork()) < 0) 
	{ 
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid != 0)
	{   
		producer(argv[1], pid);
	else 
	{   
		consumer();
	} 
	exit(EXIT_SUCCESS); 
}   








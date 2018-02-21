#include "main.h"

void consumer()
{
	FILE *output_file;
	char file_buffer[MAX_BUF_SIZE];
	char pid_[5];
	int smem_id;
	key_t smem_key = KEY_CODE;
	void *temp_ptr = (void *)0;
	struct shared_mem_struct *shared_mem;
	if((output_file = fopen("output.txt","w")) == NULL)
	{
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	if((smem_id = shmget(smem_key, sizeof(struct shared_mem_struct), IPC_CREAT | 0666))<0)
	{  
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	if((temp_ptr = shmat(smem_id, (void *)0, 0)) == (void *)-1)
	{   
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	shared_mem = (struct shared_mem_struct *) temp_ptr;

	while(shared_mem->count != -1)
	{
		while(shared_mem->count == 0)
		{
			waitParent();
		}

		if(shared_mem->count != -1)
		{
			snprintf (pid_, sizeof(pid_), "%d",getpid());
			fputs("Cosumer PID: ", output_file);
			fputs(pid_, output_file);
			fputs(", DATA: ", output_file);
			fputs(shared_mem->buffer, output_file);
			shared_mem->count = 0;    
			signalParent(getppid());   
		}
	}
	fclose(output_file);   

	if(shmdt(shared_mem) == -1)
	{
		perror("shmdt");
		exit(EXIT_FAILURE);
	}

	kill(getpid(), SIGTERM);    
}


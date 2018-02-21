#include "main.h"

void producer(char *argv, pid_t pid)
{
	FILE *input_file;
	char file_buffer[MAX_BUF_SIZE];

	int smem_id;
	key_t smem_key = KEY_CODE;
	void *temp_ptr = (void *)0;
	struct shared_mem_struct *shared_mem;
	if((input_file = fopen(argv,"r")) == NULL)
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

	shared_mem->count = 0;    

	while(fgets(file_buffer, (MAX_BUF_SIZE-1), input_file) != NULL)
	{
		while(shared_mem->count != 0)
		{
			waitChild();
		}

		strcpy(shared_mem->buffer,file_buffer);
		shared_mem->count = strlen(shared_mem->buffer);
		signalChild(pid);
		initializeWait();
	}
	initializeWait();
	while(shared_mem->count != 0)
	{
		waitChild();
	}

	shared_mem->count = -1;
	signalChild(pid);
	waitpid(pid, NULL, 0);
	fclose(input_file);

	if(shmdt(shared_mem) == -1)
	{
		perror("shmdt");
		exit(EXIT_FAILURE);
	}

	if(shmctl(smem_id, IPC_RMID, 0) == -1)
	{
		perror("shmctl");
		exit(EXIT_FAILURE);
	}

	printf("\nSuccess: The input file provided by you has been successfully copied via shared memory to output file named \"ouput.txt\" in the current working directory.\n\n");
}


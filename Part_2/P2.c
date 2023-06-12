// header files
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>

int main()
{
	// we recieve the message
	printf("-- Waiting for P1 to start --\n");

	char* path_string = "/tmp/myfifo";

	// we first make fifo file
	int mkfifo_val = mkfifo(path_string, 0660);
	if (mkfifo_val == -1 && errno != EEXIST)
	{
		perror("");
		fprintf(stderr, "ERROR: Fifo couldn't be created\n");
		exit(1);
	}

	// variables storin number of strings and string size
	int string_no = 50;
	int string_size = 10; 

	for (int i=0; i<string_no; i += 5)
	{
		// creating space for string
		char* recieved_string = malloc((string_size + 3)*sizeof(char));
		char* max_index_string = malloc((string_size + 1)*sizeof(char));

		// max index and string for integer
		int max_index = -1;
		char full_int[2];

		// opening the fd
		// printf("Opening\n");
		int fd = open(path_string, O_RDONLY);
		if (fd == -1)
		{
			fprintf(stderr, "ERROR: File couldn't be opened\n");
			exit(1);
		}
		// printf("Opening\n");
		// recieve 5 consecutive integers
		for (int j=0; j<5; j++)
		{
			// printf("so you know, it has arrived here\n");
			int k = 0;
			bool flag = true;

				// printf("Waiting for read\n");
			int read_val = read(fd, recieved_string, 13);
			// printf("The character value is | %d : %c\n", k, recieved_string[k]);

			if (read_val == -1)
			{
				fprintf(stderr, "ERROR: Not able to read message sent from P1\n");
				exit(1);	
			}

			// printf("yay reading process has ended!!!!");

			full_int[0] = recieved_string[0];
			full_int[1] = recieved_string[1];
			int index = atoi(full_int);

			if (index > max_index)
			{
				max_index = index;
				memcpy(max_index_string, &recieved_string[2], 11);
			}

			fprintf(stdout, "Recieved Message (from P1): %s \n", recieved_string + 2);
			
		}

		// closing fd
		int close_val = close(fd);

		if (close_val == -1)
		{
			fprintf(stderr, "ERROR: Closure couldn't be properly done\n");
			exit(1);
		}		

		fprintf(stdout, "\nMax index : %d | Corresponding max index message (from P1): %s \n", max_index, max_index_string);

		int fd_int_write = open(path_string, O_WRONLY);

		// lets send this message index back to P1
		int write_val = write(fd_int_write, full_int, 3);

		// check if the message has been properly sent
		if (write_val == -1)
		{
			fprintf(stderr, "ERROR: Message with index could not properly sent to P1\n");
			exit(1);
		}
		else 
		{
			fprintf(stdout, "\nSent message to P1 (checking message): %s \n\n", full_int);
		}

		int close_val_write = close(fd_int_write);
		if (close_val_write == -1)
		{
			fprintf(stderr, "ERROR: Closure couldn't be properly done\n");
			exit(1);
		}	

		
	}

	return 0;
}
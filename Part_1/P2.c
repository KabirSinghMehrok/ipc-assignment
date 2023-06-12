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

#include <sys/socket.h>
#include <sys/un.h>

int main()
{
	// we recieve the message
	printf("-- Waiting for P1 to start --\n");

	// create a path string
	char* path_string = "/tmp/mysocket";

	// unlink it in starting if its not already unlinked
	unlink(path_string);

	// create a struct for socket
	struct sockaddr_un socket_add;

	// fills the memory block with given socket address
	memset(&socket_add, 0, sizeof(struct sockaddr_un));

	socket_add.sun_family = AF_UNIX;
	strcpy(socket_add.sun_path, path_string);

	// creates a socket
	int socket_val = socket(socket_add.sun_family, SOCK_STREAM, 0);

	if (socket_val == -1)
	{
		fprintf(stderr, "ERROR: Socket creation has failed for P1\n");
		exit(1);
	}

	// create a bind
	int bind_val = bind(socket_val, (const struct sockaddr *) &socket_add, sizeof(struct sockaddr_un));

	if (bind_val == -1)
	{
		fprintf(stderr, "ERROR: Socket binding has failed for P1\n");
		exit(1);
	}

	// listen to the socket
	if (listen(socket_val, 50))
	{
		fprintf(stderr, "ERROR: Failed to listen to P1\n");
		exit(1);
	}

	// accept the message
	int accept_val = accept(socket_val, NULL, NULL);

	if (accept_val == -1)
	{
		fprintf(stderr, "ERROR: Failed to accept\n");
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

		// recieve 5 consecutive integers
		for (int j=0; j<5; j++)
		{
			// printf("so you know, it has arrived here\n");

			int read_val = read(accept_val, recieved_string, 13);
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

		fprintf(stdout, "\nMax index : %d | Corresponding max index message (from P1): %s \n", max_index, max_index_string);

		// lets send this message index back to P1
		int write_val = write(accept_val, full_int, 3);

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
	}

	int close_val1 = close(accept_val);

	if (close_val1 == -1)
	{
		fprintf(stderr, "ERROR: Socket not properly closed (accept_val)\n");
		exit(1);
	}

	int close_val2 = close(socket_val);

	if (close_val2 == -1)
	{
		fprintf(stderr, "ERROR: Socket not properly closed (socket_val)\n");
		exit(1);
	}

	int unlink_val = unlink(path_string);

	if (unlink_val == -1)
	{
		fprintf(stderr, "ERROR: Socket not properly unlinked\n");
		exit(1);
	}

	return 0;
}
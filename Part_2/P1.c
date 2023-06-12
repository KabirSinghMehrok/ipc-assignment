// header files
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include <sys/socket.h>
#include <sys/un.h>



char* generate_string(int s, int index) {
	// generates a random string of the given size (s) using rand()

	char* string = malloc(s+3 * sizeof(char));
	// space of n+1 character needs to be given

	for (int i=2; i < s+2; i++)
	{
		string[i] = (char) ((rand()%26) + 65);	
		// printf("%c(%d)", string[i], i);
		// ok so generation is happening properly
	}
	// printf(" -- %s\n", string);

	// add the end character to the string
	string[s+2] = '\0';
	// add index at the front of the string
	string[0] = (char) ('0' + (index / 10));
	// printf("\n%c is the 0th character\n", string[0]);
	string[1] = (char) ('0' + (index % 10));
	// printf("\n%c is the 1st character\n", string[1]);

	// EXPECTED OUTPUT
	// 1X*HOU

	// printf("%s has been sent to generator\n", string);
	return string;
}

char** generate_array(int n, int s) {
	// generates an array having n strings of size s each

    char ** array = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++)
    {
    	array[i] = generate_string(s, i);
		// printf("%s is being generated\n", array[i]);
    }

    return array;
}

int main()
{
	char* path_string = "/tmp/myfifo";

	// we first make fifo file
	int mkfifo_val = mkfifo(path_string, 0660);
	if (mkfifo_val == -1 && errno != EEXIST)
	{
		fprintf(stderr, "ERROR: Fifo couldn't be created\n");
		exit(1);
	}

	// create the variable to generate array of strings
	int string_no = 50;
	int string_size = 10; 

	// generate array of strings
	char** string_arr = generate_array(string_no, string_size);

	// run a loop to print messages 50 times
	for (int i=0; i<string_no; i += 5)
	{
		// sending 5 consecutive strings
		for (int j=0; j<5; j++)
		{
			// printf("Opening\n");
			int fd = open(path_string, O_WRONLY);
			if (fd == -1)
			{
				fprintf(stderr, "ERROR: File couldn't be opened\n");
				exit(1);
			}

			// printf("Sending\n");

			int send_message_val =  write(fd, string_arr[i+j], string_size + 3);

			// check if the message has been properly sent
			if (send_message_val != string_size + 3)
			{
				fprintf(stderr, "ERROR: Message not properly sent to P2\n");
				exit(1);
			}
			else 
			{
				// prints the string to be sent (minus the index that is sent along with it)
				fprintf(stdout, "%d | Sent message: %s \n", i+j, string_arr[i+j] + 2);
			}

			int close_val = close(fd);

			if (close_val == -1)
			{
				fprintf(stderr, "ERROR: Closure couldn't be properly done\n");
				exit(1);
			}

		}

		//lets create a pointer (to recieve the two characters of index)
		char *msg_ptr = malloc(3 * sizeof(char));

		int fd = open(path_string, O_RDONLY);
		if (fd == -1)
		{
			fprintf(stderr, "ERROR: Fifo couldn't be opened (for integer reading)\n");
			exit(1);
		}

		int recieve_message_val =  read(fd, msg_ptr, 3 * sizeof(char));
		if (recieve_message_val == -1)
		{
			fprintf(stderr, "ERROR: Fifo couldn't be read\n");
			exit(1);
		}
		else 
		{
			fprintf(stdout, "\nConfirmation index (Sent by P2 for previoous 5 string): %s \n\n", msg_ptr);
		}


		int close_val = close(fd);
		if (close_val == -1)
		{
			fprintf(stderr, "ERROR: Closure couldn't be properly done (for integer reading)\n");
			exit(1);
		}
	}



	return 0;
}
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
	// Creates a incoming queue if it's not already there
	mqd_t mq_incoming = mq_open("/P2toP1_1234567890", O_CREAT | O_RDWR, 0777, NULL);
	mqd_t mq_outgoing = mq_open("/P1toP2_1234567890", O_CREAT | O_RDWR, 0777, NULL);

	// ERROR handling
	if (mq_incoming == -1)
	{
		perror("ok");
		fprintf(stderr, "ERROR: Can't create incoming message queue for P1\n");
		exit(1);
	}

	if (mq_outgoing == -1)
	{
		fprintf(stderr, "ERROR: Can't create outgoing message queue for P1\n");
		exit(1);
	}

	// create an array of strings
	int string_no = 50;
	int string_size = 10; 

	char** string_arr = generate_array(string_no, string_size);

	// create unsigned integer of message priority
	unsigned int msg_prio = 10;

	for (int i=0; i<string_no; i += 5)
	{
		// sending 5 consecutive strings
		for (int j=0; j<5; j++)
		{


			int mq_sent =  mq_send(mq_outgoing, string_arr[i+j], (size_t) string_size +  1, msg_prio);

			// check if the message has been properly sent
			if (mq_sent == -1)
			{
				fprintf(stderr, "ERROR: Message not properly sent\n");
				exit(1);
			}
			else 
			{
				// prints the string to be sent (minus the index that is sent along with it)
				fprintf(stdout, "%d | Sent message: %s \n", i+j, string_arr[i+j] + 2);
			}

		}

		//lets create a pointer (to recieve the two characters of index)
		char *msg_ptr = malloc(2 * sizeof(char));

		struct mq_attr attr;
		mq_getattr (mq_incoming, &attr);

		size_t mq_rcv = mq_receive(mq_incoming, msg_ptr, (size_t) attr.mq_msgsize, NULL);

		if (mq_rcv == -1)
		{
			fprintf(stderr, "ERROR: Message not properly recieved: %s\n", strerror(errno));
			exit(1);
		}

		else 
		{
			fprintf(stdout, "\nConfirmation index (Sent by P2 for previoous 5 string): %s \n\n", msg_ptr);
		}
	}

	mq_close(mq_outgoing);
	mq_close(mq_incoming);
	return 0;
}
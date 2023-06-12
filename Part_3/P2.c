// header files
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>


int max_finder(int a, int b)
{
	if (a < b)
	{
		return b;
	}
	return a;
}

int main()
{
	// Creates a incoming queue if it's not already there
	mqd_t mq_outgoing = mq_open("/P2toP1_1234567890", O_CREAT | O_RDWR, 0777, NULL);
	mqd_t mq_incoming = mq_open("/P1toP2_1234567890", O_CREAT |  O_RDWR, 0777, NULL);

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

	// variables storin number of strings and string size
	int string_no = 50;
	int string_size = 10; 

	// create unsigned integer of message priority
	unsigned int msg_prio = 10;

	// we recieve the message
	printf("-- Waiting for P1 to start --\n");

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
			struct mq_attr msg_attr;

			int msg_attr_val = mq_getattr(mq_incoming, &msg_attr);

			if (msg_attr_val == -1)
			{
				fprintf(stderr, "ERROR: Attribute of recieved message not properly set\n");
				exit(1);	
			}
			else 
			{
				int recieved_string_val = mq_receive(mq_incoming, recieved_string, msg_attr.mq_msgsize, NULL); 

				if (recieved_string_val == -1)
				{
					fprintf(stderr, "ERROR: Message not properly recieved from P1\n");
					exit(1);	
				}
				else
				{

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
			}
		}

		fprintf(stdout, "\nMax index : %d | Corresponding max index message (from P1): %s \n", max_index, max_index_string);

		// lets send this message back to P1
		int sending_string_val = mq_send(mq_outgoing, full_int, 2, msg_prio);

		// check if the message has been properly sent
		if (sending_string_val == -1)
		{
			fprintf(stderr, "ERROR: Message not properly sent\n");
			exit(1);
		}
		else 
		{
			fprintf(stdout, "\nSent message to P1 (checking message): %s \n\n", full_int);
		}
	}

	mq_close(mq_outgoing);
	mq_close(mq_incoming);
	return 0;
}
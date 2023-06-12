# Interprocess Communication Exercise

This project focuses on developing a strong understanding of Linux interprocess communication mechanisms. The task involves creating two programs, P1 and P2. P1 generates an array of 50 random strings and sends groups of five consecutive elements to P2 along with their corresponding IDs. P2 accepts the strings, prints the IDs and strings, and sends back the highest received ID to acknowledge the strings. The communication needs to be implemented using three techniques: Unix domain sockets, FIFOs, and message passing queues.

## Programs
There are three folders for the following project, with each folder containing versions of programs P1 and P2
- **Part 1** - *Unix Domain Socket Variant* : Implements interprocess communication using Unix domain sockets.
- **Part 2** - *FIFO Variant* : Implements interprocess communication using FIFOs.
- **Part 3** - *Message Passing Queue Variant* : Implements interprocess communication using message passing queues.

## Approach

### Part 1: Unix Socket

-   Strings are passed between processes P1 and P2 using sockets.
-   P1 generates random strings using the `rand()` function.
-   The `sockaddr_un` struct is created and filled with the necessary variables using `memset`.
-   A connection is established using the `connect` function.
-   Messages are sent between P1 and P2 using the `read` and `write` system calls.
-   P1 sends messages to P2, which reads the messages, prints the strings, and sends back the index with the maximum value.
-   P1 prints the index.
-   After sending 50 strings, the socket closes.

### Part 2: FIFO

### Part 3: Message Passing Queue

-   Strings are passed between processes P1 and P2 using message queues.
-   P1 generates random strings using the `rand()` function.
-   Two message queues are created, one for sending data from P1 to P2 and the other for sending data from P2 to P1.
-   P1 sends messages to P2 using `mq_send`, and P2 reads the messages using `mq_receive`.
-   P2 prints the strings it receives, sends back the index with the maximum value to P1, and prints the index.
-   After sending 50 strings, the message queue closes.

Please refer to the individual program files for more detailed implementation.

## Usage
To use the IPC Sockets project, follow the steps below:
1. Clone the repository
2. Navigate to the project directory
3. Build the programs using the provided Makefile: `make`
4. Run the P1 program to generate random strings and send them to P2: `./P1`
5. Run the P2 program to receive the strings and acknowledge them: `./P2`
6. P2 will print the received strings and their corresponding IDs on the console.

Note: Make sure to run P2 after starting P1 to ensure successful communication.

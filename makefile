build:
	@printf "\n-- Compiling Part 1 files (Socket) --\n"
	gcc Part_1/P1.c -o Part1/P1
	gcc Part_1/P2.c -o Part1/P2
	@printf "\n-- Compilation done --\n"

	@printf "\n-- Compiling Part 2 files (FIFO) --\n"
	# gcc Part_2/P1.c -o Part_2/P1
	# gcc Part_2/P2.c -o Part_2/P2
	@printf "\n-- Compilation done --\n"

	@printf "\n-- Compiling Part 3 files (Messaging) --\n"
	gcc Part_3/P1.c -o Part_3/P1
	gcc Part_3/P2.c -o Part_3/P2
	@printf "\n-- Compilation done --\n"

clean:
	rm */P1 */P2 -rf


	
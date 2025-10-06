COMP = gcc
FLAGS = -Wall -Wextra -fno-stack-protector

PROBLEM = marching_squares
build: $(PROBLEM)

marching_squares: marching_squares.c
	$(COMP) $(FLAGS) marching_squares.c -o marching_squares 

clean:
	rm -f $(PROBLEM) *.o
	
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./marching_squares
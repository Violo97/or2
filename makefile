tsp: src/main.c src/greedy.c src/utility.c
	gcc -O3 src/main.c src/greedy.c src/utility.c src/greedy.h src/tsp.h src/utility.h -o main -lm
default: hannibal washington successors
washington: washington.c
	$(CC) -O2 -Wall -o $@ $^ -lm
hannibal: hannibal.c
	$(CC) -O2 -Wall -o $@ $^ -lm
successors: successors.c
	$(CC) -O2 -Wall -o $@ $^ -lm

default: hannibal washington
washington: washington.c
	$(CC) -O2 -Wall -o $@ $^ -lm
hannibal: hannibal.c
	$(CC) -O2 -Wall -o $@ $^ -lm

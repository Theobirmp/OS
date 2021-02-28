CC=gcc

CFLAGS=-c
CRYPTO=-lcrypto
SSL=-lssl

all: main1 main2 main3 main4 main5

main1: my_main1.o semaphores.o shared_memory.o
	$(CC) -o main1 my_main1.o semaphores.o shared_memory.o $(CRYPTO) $(SSL)

main2: my_main2.o semaphores.o shared_memory.o
	$(CC) -o main2 my_main2.o semaphores.o shared_memory.o $(CRYPTO) $(SSL)

main3: my_main3.o semaphores.o shared_memory.o
	$(CC) -o main3 my_main3.o semaphores.o shared_memory.o $(CRYPTO) $(SSL)

main4: my_main4.o semaphores.o shared_memory.o
	$(CC) -o main4 my_main4.o semaphores.o shared_memory.o $(CRYPTO) $(SSL)

main5: my_main5.o semaphores.o shared_memory.o
	$(CC) -o main5 my_main5.o semaphores.o shared_memory.o $(CRYPTO) $(SSL)

my_main1.o: my_main1.c
	$(CC) $(CFLAGS) $(CRYPTO) $(SSL) my_main1.c

my_main2.o: my_main2.c
	$(CC) $(CFLAGS) $(CRYPTO) $(SSL) my_main2.c

my_main3.o: my_main3.c
	$(CC) $(CFLAGS) $(CRYPTO) $(SSL) my_main3.c

my_main4.o: my_main4.c
	$(CC) $(CFLAGS) $(CRYPTO) $(SSL) my_main4.c

my_main5.o: my_main5.c
	$(CC) $(CFLAGS) $(CRYPTO) $(SSL) my_main5.c

semaphores.o: semaphores.c
	$(CC) $(CFLAGS) semaphores.c

shared_memory.o: shared_memory.c
	$(CC) $(CFLAGS) shared_memory.c

clean:
	rm -rf *o main1 main2 main3 main4 main5

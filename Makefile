CC=gcc -g
CFLAGS=-I.

client: client.c requests.c helpers.c buffer.c
	$(CC) -o client client.c requests.c helpers.c buffer.c parson.c prompts.c utils.c -Wall -Wextra

run: client
	./client

clean:
	rm -f *.o client

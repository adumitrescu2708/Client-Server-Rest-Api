CC=gcc
CFLAGS=-I.

client: client.c utils/request.c utils/helpers.c parser/json_parser.c utils/buffer.c parser/parson.c parser/message_parser.c
	$(CC) -o client client.c utils/request.c utils/helpers.c utils/buffer.c parser/parson.c parser/json_parser.c parser/message_parser.c -Wall

run: client
	./client

clean:
	rm -f *.o client

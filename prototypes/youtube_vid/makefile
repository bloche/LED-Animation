# Everett Bloch 9/26/13

BIN = test
CC = gcc
CFLAGS = -Wall

$(BIN) : $(BIN).c yt-surl.o
	$(CC) -o $(BIN) $(CFLAGS) $(BIN).c yt-surl.o -lcurl

ty-surl.o : ty-surl.c
	$(CC) -c $(CFLAGS) ty-surl.c

debug:  $(BIN).c
	$(CC) -o $(BIN) $(CFLAGS) $(BIN).c yt-surl.c -g -lcurl

clean:
	rm *.o $(BIN)
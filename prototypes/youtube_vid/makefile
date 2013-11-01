# Everett Bloch 9/26/13

BIN = test
CC = gcc
CFLAGS = -Wall
SOURCE = ../../src/youtube_interface/yt-surl.c

$(BIN) : $(BIN).c yt-surl.o
	$(CC) -o $(BIN).out $(CFLAGS) $(BIN).c yt-surl.o -lcurl

yt-surl.o : $(SOURCE)
	$(CC) -c $(CFLAGS) $(SOURCE)

debug:  $(BIN).c
	$(CC) -o $(BIN).out $(CFLAGS) $(BIN).c $(SOURCE) -g -lcurl

clean:
	rm *.o *~ $(BIN) 
	rm -r $(BIN).dSYM
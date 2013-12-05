# makefile

BIN=leda

$(BIN) : main.o yt-surl.o vstream.o
	g++ -o $(BIN) main.o yt-surl.o vstream.o `pkg-config opencv --libs` -lcurl

main.o: main.cpp
	g++ -c main.cpp

yt-surl.o : youtube_interface/yt-surl.c
	gcc -c youtube_interface/yt-surl.c

vstream.o: ../tests/integration/url_stream/vstream.cpp
	g++ -c ../tests/integration/url_stream/vstream.cpp `pkg-config opencv --cflags`

clean:
	rm *.o *.mp4 leda
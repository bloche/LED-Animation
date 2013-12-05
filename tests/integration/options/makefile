# makefile

SRC1=../../../src/youtube_interface/yt-surl.c
SRC2=../url_stream/vstream.cpp

a.out : main.o yt-surl.o vstream.o
	g++ main.o yt-surl.o vstream.o `pkg-config opencv --libs` -lcurl

main.o: main.cpp
	g++ -c main.cpp

yt-surl.o : $(SRC1)
	gcc -c $(SRC1)

vstream.o: $(SRC2)
	g++ -c  $(SRC2) `pkg-config opencv --cflags`

clean:
	rm *.o *.mp4
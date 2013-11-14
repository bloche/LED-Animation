# compile youtube interface and video stream interface integration test files

SRC1=../../../src/youtube_interface/yt-surl.c 
SRC2=../../../src/video_stream_interface/vstream.cpp

all: yt-surl.o vstream.o test.o
	g++ test.o yt-surl.o vstream.o `pkg-config opencv --libs` -lcurl

test.o: test.cpp
	g++ -c test.cpp

yt-surl.o: $(SRC1)
	gcc -c $(SRC1)

vstream.o: $(SRC2)
	g++ -c $(SRC2) `pkg-config opencv --cflags`

clean:
	rm *.o *~
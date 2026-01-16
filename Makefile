CC = gcc
FLAGS = -g -Wall -Wextra -Iinclude

all: obj main

obj:
	mkdir -p obj

main: obj/main.o obj/image.o obj/video_pipe.o
	$(CC) -o main obj/main.o obj/image.o obj/video_pipe.o $(FLAGS)

obj/main.o: src/main.c include/decoder.h | obj
	$(CC) -c src/main.c $(FLAGS) -o obj/main.o

obj/image.o: src/image.c | obj
	$(CC) -c src/image.c $(FLAGS) -o obj/image.o

obj/video_pipe.o: src/video_pipe.c | obj
	$(CC) -c src/video_pipe.c $(FLAGS) -o obj/video_pipe.o

clean:
	rm -rf obj/*.o

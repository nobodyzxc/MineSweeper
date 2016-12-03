all:
	gcc -c include/mysocket.c GameMain.c
	gcc -o gameSvr GameSvr.c mysocket.o GameMain.o 

clean:
	rm -f gameSvr *.o


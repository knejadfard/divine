divine: divine.o
	g++ -o divine divine.o -I . -pthread

divine.o: main.cpp divine.h
	g++ -c main.cpp -o divine.o -I . -pthread

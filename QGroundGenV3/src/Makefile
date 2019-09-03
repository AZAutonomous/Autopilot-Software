output: main.o coordinate.o path.o
    g++ -c -Wall main.o coordinate.o path.o -o output
main.o: main.cpp
    g++ -c main.cpp
coordinate.o: coordinate.cpp coordinate.h
path.o: path.cpp path.h
    g++ -c -Wall path.cpp
clean:
    rm *.o output
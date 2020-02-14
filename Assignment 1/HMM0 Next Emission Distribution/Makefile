all: main

main: hmm0.o
	g++ -o main hmm0.o -W

hmm0.o: hmm0.cpp
	g++ -c hmm0.cpp -W

clean:
	rm -f main *.o

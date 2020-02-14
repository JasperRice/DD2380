all: main

main: hmm1.o
	g++ -o main hmm1.o -W

hmm0.o: hmm1.cpp
	g++ -c hmm1.cpp -W

clean:
	rm -f main *.o

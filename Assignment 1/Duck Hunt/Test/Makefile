all: main

main: test.o HiddenMarkovModel.o
	g++ -o main test.o HiddenMarkovModel.o -W

test.o: test.cpp
	g++ -c test.cpp -W

HiddenMarkovModel.o: HiddenMarkovModel.cpp
	g++ -c HiddenMarkovModel.cpp -W

test:
	./main < hmm4_01.in && ./main < hmm4_02.in && ./main < hmm4_03.in 

run:
	./main < NorthEmissions.in

valgrind:
	valgrind --leak-check=full ./main < NorthEmissions.in

clean:
	rm -f main *.o

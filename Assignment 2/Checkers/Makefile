compile:
	g++ -std=c++11 *.cpp -Wall -o checkers

fifo:
	mkfifo /tmp/pipe

run:
	./checkers init verbose < /tmp/pipe | ./checkers > /tmp/pipe

valgrind:
	valgrind --leak-check=full ./checkers init verbose < /tmp/pipe | ./checkers > /tmp/pipe

fifo2:
	mkfifo /tmp/pipe1 /tmp/pipe2

run2:
	./checkers init verbose < /tmp/pipe1 > /tmp/pipe2

run3:
	./checkers verbose > /tmp/pipe1 < /tmp/pipe2

debug:
	gdb --args ./checkers

clean:
	rm -f checkers

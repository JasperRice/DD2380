compile:
	g++ *.cpp -Wall -o TTT

fifo:
	mkfifo /tmp/pipe

run:
	./TTT init verbose < /tmp/pipe | ./TTT > /tmp/pipe

vs:
	./2D\ Tic-Tac-Toe/TTT init verbose < /tmp/pipe | ./2D\ Random/TTT > /tmp/pipe

valgrind:
	valgrind --leak-check=full ./TTT init verbose < /tmp/pipe | ./TTT > /tmp/pipe

fifo2:
	mkfifo /tmp/pipe1 /tmp/pipe2

run2:
	./TTT init verbose < /tmp/pipe1 > /tmp/pipe2

run3:
	./TTT verbose > /tmp/pipe1 < /tmp/pipe2

debug:
	gdb --args ./TTT

clean:
	rm -f TTT

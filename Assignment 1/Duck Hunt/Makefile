clean:
	rm -f main *.o *.gch

compile:
	g++ -std=c++11 *pp -g -o main

fifo:
	mkfifo /tmp/player2server /tmp/server2player

run:
	./main server < /tmp/player2server | ./main verbose > /tmp/player2server

valgrind:
	valgrind --leak-check=full ./main server < /tmp/player2server | ./main verbose > /tmp/player2server

# Test a certain environment
test:
	./main server load ParadiseEmissions.in < player2server | java Main verbose > player2server

# Terminal 1
server:
	./main verbose server < /tmp/player2server > /tmp/server2player

# Terminal 2
debug:
	gdb --args ./main

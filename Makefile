all: test_pqueue

test_pqueue: test_pqueue.cc 
	g++ -Wall -Werror -o $@ $< -std=c++11 -pthread -lgtest

clean:
	-rm -f test_pqueue *.dat

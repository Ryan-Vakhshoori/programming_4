all: zap unzap

zap: zap.cc huffman.h bstream.h pqueue.h
	g++ -Wall -Werror -o $@ $< -std=c++11

unzap: unzap.cc huffman.h bstream.h pqueue.h
	g++ -Wall -Werror -o $@ $< -std=c++11

clean:
	-rm -f *.txt *.zap *.unzap zap unzap test_bstream test_pqueue

all: zap unzap

zap: zap.cc huffman.h bstream.h pqueue.h
	g++ -Wall -Werror -std=c++11 -o zap zap.cc

unzap: unzap.cc huffman.h bstream.h pqueue.h
	g++ -Wall -Werror -std=c++11 -o unzap unzap.cc

clean:
	-rm -f *.txt *.zap *.unzap zap unzap test_bstream test_pqueue

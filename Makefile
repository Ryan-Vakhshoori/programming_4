all: zap unzap

zap: zap.cc huffman.h
	g++ -Wall -Werror -o $@ $< -std=c++11

unzap: unzap.cc huffman.h
	g++ -Wall -Werror -o $@ $< -std=c++11

clean:
	-rm -f *.txt *.zap *.unzap zap unzap

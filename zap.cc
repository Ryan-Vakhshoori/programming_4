#include "huffman.h"

int main(int argc, char* argv[]) {
  std::string filename{"frederick_douglass.txt"};
  std::ifstream ifs(filename);
  std::string ofilename{"frederick_douglass.txt.zap"};
  std::ofstream ofs(ofilename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  Huffman test;
  test.Compress(ifs, ofs);
  ofs.close();
  ifs.close();
}

/* 00000000: 00101000 01110100 00101010 00001000 00000000 00000000  (t*...
00000006: 00000001 11100111 11111111 11111111 11111111 11101010  ......
0000000c: 10101010 10101010 10101010 10101010 10100000 00000000  ......
00000012: 00000000 */ 
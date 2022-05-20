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
#include "huffman.h"

int main(int argc, char* argv[]) {
  std::string filename{"ABC"};
  std::ifstream ifs(filename);
  std::string ofilename{"ABC.zap"};
  std::ofstream ofs(ofilename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  Huffman test;
  test.Compress(ifs, ofs);
  ifs.close();
  ofs.close();
}
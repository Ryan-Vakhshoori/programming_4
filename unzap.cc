#include "huffman.h"

int main(int argc, char* argv[]) {
  std::string filename{"frederick_douglass.txt.zap"};
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  std::string ofilename{"frederick_douglass_test.txt"};
  std::ofstream ofs(ofilename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  Huffman unzap;
  unzap.Decompress(ifs, ofs);
  ifs.close();
  ofs.close();
}
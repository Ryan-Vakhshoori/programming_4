#include "huffman.h"

int main(int argc, char* argv[]) {
  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
  std::ofstream ofs(argv[2],
                    std::ios::out | std::ios::trunc | std::ios::binary);
  Huffman unzap;
  unzap.Decompress(ifs, ofs);
  ifs.close();
  ofs.close();
}
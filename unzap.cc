#include "huffman.h"

int main(int argc, char* argv[]) {
  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
  std::ofstream ofs(argv[2],
                    std::ios::out | std::ios::trunc | std::ios::binary);
  Huffman test;
  test.Decompress(ifs, ofs);
  ifs.close();
  ofs.close();
}
// 00101000011101000010101000001000
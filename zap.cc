#include "huffman.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <inputfile> <zapfile>" << std::endl;
    return 1;
  }
  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
  std::ofstream ofs(argv[2],
                    std::ios::out | std::ios::trunc | std::ios::binary);
  Huffman test;
  test.Compress(ifs, ofs);
  ofs.close();
  ifs.close();
  return 0;
}
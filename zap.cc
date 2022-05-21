#include "huffman.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <inputfile> <zapfile>" << std::endl;
    return 1;
  }
  std::ifstream ifs;
  ifs.open(argv[1], std::ios::in);
  if (ifs.is_open()) {
    std::ofstream ofs(argv[2],
                      std::ios::out | std::ios::trunc | std::ios::binary);
    Huffman test;
    test.Compress(ifs, ofs);
    ofs.close();
    ifs.close();
    return 0;
  }
  std::cerr << "Error: cannot open input file " << argv[1] << std::endl;
  return 1;
}
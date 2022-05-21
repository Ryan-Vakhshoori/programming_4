#include "huffman.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <zapfile> <outputfile>" << std::endl;
    return 1;
  }
  std::ifstream ifs;
  ifs.open(argv[1], std::ios::in | std::ios::binary);
  if (ifs.is_open()) {
    std::ofstream ofs(argv[2],
                      std::ios::out | std::ios::trunc | std::ios::binary);
    Huffman unzap;
    unzap.Decompress(ifs, ofs);
    ifs.close();
    ofs.close();
    return 0;
  }
  std::cerr << "Error: cannot open zap file " << argv[1] << std::endl;
  return 1;
}
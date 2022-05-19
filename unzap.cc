#include "huffman.h"

int main(int argc, char* argv[]) {
  std::string filename{"test_zap"};
  const unsigned char val[] = {
      0x28, 0x74, 0x2A, 0x08, 0x00, 0x00, 0x01, 0xE7, 0xFF, 0xFF,
      0xFF, 0xEA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA0, 0x00, 0x00,
  };
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  ofs.write(reinterpret_cast<const char*>(val), sizeof(val));
  ofs.close();
  Huffman test;
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  std::string ofilename{"output_zap"};
  std::ofstream ofs1(ofilename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  test.Decompress(ifs, ofs1);
  ofs1.close();
}
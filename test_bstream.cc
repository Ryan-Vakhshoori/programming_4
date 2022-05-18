#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>

#include "bstream.h"

TEST(BStream, input) {
  std::string filename{"test_bstream_input"};

  const unsigned char val[] = {
      0x58, 0x90, 0xab, 0x08, 0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0101100010010000101010110000100000000000010011101101101101000000
  // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we reading the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetChar(), 0x62);  // 01100010
  EXPECT_EQ(bis.GetChar(), 0x42);  // 01000010
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x58400276);  // 01011000010000000000001001110110

  ifs.close();

  std::remove(filename.c_str());
}

TEST(Bstream, output) {
  std::string filename{"test_bstream_output"};

  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);

  BinaryOutputStream bos(ofs);

 // 0x90, 0xab, 0x08, 0x00, 0x4e, 0xdb, 0x40,
  bos.PutChar(0x58);
  bos.PutChar(0xab);
  bos.PutChar(0x08);
  bos.PutChar(0x00);
  bos.PutChar(0x4e);
  bos.PutChar(0xdb);
  bos.PutChar(0x40);
  bos.PutInt(0x58400276);

  ofs.close();

  unsigned char val[11];
  std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
  ifs.read(reinterpret_cast<char *>(val), sizeof(val));
  ifs.close();

  std::remove(filename.c_str());

  EXPECT_EQ(val[0], 0x58);
  EXPECT_EQ(val[1], 0xab);
  EXPECT_EQ(val[2], 0x08);
  EXPECT_EQ(val[3], 0x00);
  EXPECT_EQ(val[4], 0x4e);
  EXPECT_EQ(val[5], 0xdb);
  EXPECT_EQ(val[6], 0x40);
  EXPECT_EQ(val[7], 0x58);
  EXPECT_EQ(val[8], 0x40);
  EXPECT_EQ(val[9], 0x02);
  EXPECT_EQ(val[10], 0x76);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

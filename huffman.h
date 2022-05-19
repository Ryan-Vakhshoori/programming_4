#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "bstream.h"
#include "pqueue.h"

class HuffmanNode {
 public:
  explicit HuffmanNode(char ch, size_t freq, HuffmanNode *left = nullptr,
                       HuffmanNode *right = nullptr)
      : ch_(ch), freq_(freq), left_(left), right_(right) {}

  bool IsLeaf() {
    // Node is a leaf if it doesn't have any children
    return left_ == nullptr && right_ == nullptr;
  }

  bool operator<(const HuffmanNode &n) const {
    // In case of equality, make it deterministic based on character
    if (freq_ == n.freq_) return ch_ < n.ch_;
    // Otherwise compare frequencies
    return freq_ < n.freq_;
  }

  size_t freq() { return freq_; }
  size_t data() { return ch_; }
  HuffmanNode *left() { return left_; }
  HuffmanNode *right() { return right_; }

 private:
  char ch_;
  size_t freq_;
  HuffmanNode *left_, *right_;
};

class Huffman {
 public:
  static void Compress(std::ifstream &ifs, std::ofstream &ofs);

  static void Decompress(std::ifstream &ifs, std::ofstream &ofs);

 private:
  HuffmanNode* Reconstruction(std::ifstream &ifs);
};

// To be completed below

void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  std::string s;
  while (ifs >> s) {
    s += ' ';
  }
  s.pop_back();
  std::cout << s << std::endl;
  std::stringstream ss(s);
  std::string word;
  // should be priority queue
  std::vector<HuffmanNode *> nodes;
  while (ss >> word) {
    nodes.push_back(new HuffmanNode(word[0], word.size()));
  }
  HuffmanNode *root = nullptr;
  // nodes.Top();
  // nodes.Pop();
  // nodes.Top
}

void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs) {
  // reading the Huffman tree
  BinaryInputStream bis(ifs);
  BinaryOutputStream bos(ofs);
  HuffmanNode *root = Reconstruction(ifs);
  int size = bis.GetInt();
  for (int i = 0; i < size; i++) {
    HuffmanNode *n = root;
    while (!n->IsLeaf()) {
      if (bis.GetBit() == 1)
        n = n->right();
      else if (bis.GetBit() == 0)
        n = n->left();
    }
    bos.PutChar(n->data());
  }
}

HuffmanNode* Huffman::Reconstruction(std::ifstream &ifs) {
  BinaryInputStream bis(ifs);
  if (bis.GetBit() == 0) {
    HuffmanNode *left = Reconstruction(ifs);
    HuffmanNode *right = Reconstruction(ifs);
    HuffmanNode *n = new HuffmanNode(0, 0, left, right);
  }
  return new HuffmanNode(bis.GetChar(), 0, nullptr, nullptr);
}

// 00101000011101000010101000001000

#endif  // HUFFMAN_H_

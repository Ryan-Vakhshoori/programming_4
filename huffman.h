#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

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
  static HuffmanNode *Reconstruction(BinaryInputStream &bis);
};

// To be completed below

void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  BinaryInputStream bis(ifs);
  BinaryOutputStream bos(ofs);
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
  HuffmanNode *node1, *node2 = nullptr;
  while (nodes.Size() != 1) {
    node1 = nodes.Top();
    nodes.Pop();
    node2 = nodes.Top();
    nodes.Pop();
    nodes.Push(new Huffman(0, node1->freq() + node2->freq(), node1, node2));
  }
  HuffmanNode *root = nodes.Top();
  std::stack<HuffmanNode*> stack;
  stack.push(root);
  while (!stack.empty()) {
    HuffmanNode *n = stack.top();
    if (n->IsLeaf()) {
      bos.PutBit(1);
      bos.PutChar(n->data());
    } else {
      bos.PutBit(0);
    }
    stack.pop();
    if (n->right())
      stack.push(n->right());
    if (n->left())
      stack.push(n->left());
  }
  std::map<char, std::string> code_table;
  std::stack<HuffmanNode*> stack;
  stack.push(root);
  while (!stack.empty()) {
    std::string code;
    HuffmanNode *n = stack.top();
    if (n->IsLeaf())
      code_table[n->data()] = code;
    stack.pop();
    if (n->right()) {
      code += "1";
      stack.push(n->right());
    }
    if (n->left()) {
      code += "0";
      stack.push(n->left());
  }
}

void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs) {
  // reading the Huffman tree
  BinaryInputStream bis(ifs);
  BinaryOutputStream bos(ofs);
  HuffmanNode *root = Reconstruction(bis);
  int size = bis.GetInt();
  for (int i = 0; i < size; i++) {
    HuffmanNode *n = root;
    while (!(n->IsLeaf())) {
      if (bis.GetBit() == 1) {
        n = n->right();
      } else {
        n = n->left();
      }
    }
    bos.PutChar(n->data());
  }
}

HuffmanNode *Huffman::Reconstruction(BinaryInputStream &bis) {
  if (bis.GetBit() == 0) {
    HuffmanNode *left = Reconstruction(bis);
    HuffmanNode *right = Reconstruction(bis);
    return new HuffmanNode(0, 0, left, right);
  }
  return new HuffmanNode(bis.GetChar(), 0, nullptr, nullptr);
}

#endif  // HUFFMAN_H_

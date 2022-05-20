#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
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
  static HuffmanNode *Reconstruction(BinaryInputStream &bis);
  static void BuildTable(HuffmanNode *node, std::string s,
                                  std::map<char, std::string> &code_table,
                                  int i);
};

// To be completed below

struct HuffmanNodePtr {
  bool operator()(HuffmanNode *r, HuffmanNode *s) const {
    if (r->freq() == s->freq()) return r->data() < s->data();
    return r->freq() < s->freq();
  }
};

void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  BinaryOutputStream bos(ofs);
  std::map<char, int> freq;
  std::string text = std::string((std::istreambuf_iterator<char>(ifs)),
                                 std::istreambuf_iterator<char>());

  for (char c : text) {
    freq[c]++;
  }
  PQueue<HuffmanNode *, HuffmanNodePtr> nodes;
  for (std::map<char, int>::iterator it = freq.begin(); it != freq.end();
       it++) {
    nodes.Push(new HuffmanNode(it->first, it->second, nullptr, nullptr));
  }
  int size = text.length();
  HuffmanNode *node1, *node2 = nullptr;
  while (nodes.Size() != 1) {
    node1 = nodes.Top();
    nodes.Pop();
    node2 = nodes.Top();
    nodes.Pop();
    nodes.Push(new HuffmanNode(0, node1->freq() + node2->freq(), node1, node2));
  }
  HuffmanNode *root = nodes.Top();
  std::stack<HuffmanNode *> stack;
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
    if (n->right()) stack.push(n->right());
    if (n->left()) stack.push(n->left());
  }
  bos.PutInt(size);
  std::map<char, std::string> code_table;
  std::string s;
  BuildTable(root, s, code_table, 0);
  for (char c : text) {
    for (char c : code_table[c]) {
      if (c == '0') {
        bos.PutBit(0);
      } else {
        bos.PutBit(1);
      }
    }
  }
}

void Huffman::BuildTable(HuffmanNode *node, std::string s,
                         std::map<char, std::string> &code_table, int i) {
  if (node->left()) {
    if (i == s.length()) {
      s += '0';
    } else {
      s[i] = '0';
    }
    BuildTable(node->left(), s, code_table, i + 1);
  }
  if (node->right()) {
    if (i == s.length()) {
      s += '1';
    } else {
      s[i] = '1';
    }
    BuildTable(node->right(), s, code_table, i + 1);
  }
  if (node->IsLeaf()) {
    code_table[node->data()] = s;
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

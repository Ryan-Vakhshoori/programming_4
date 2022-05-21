#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
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
    if (freq_ == n.freq_) {
      return ch_ < n.ch_;
    }
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
                         unsigned int i);
  static void PrintTree(std::stack<HuffmanNode *> stack,
                        BinaryOutputStream &bos);
  static void PrintEncoded(std::string &text,
                           std::map<char, std::string> &code_table,
                           BinaryOutputStream &bos);
};

// To be completed below

struct HuffmanNodePtr {
  bool operator()(HuffmanNode *r, HuffmanNode *s) const {
    if (r->freq() == s->freq()) {
      return r->data() < s->data();
    }
    return r->freq() < s->freq();
  }
};

void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  // open binary output stream
  BinaryOutputStream bos(ofs);
  // declare frequency map
  std::map<char, int> freq;
  // read file into string
  std::string text = std::string((std::istreambuf_iterator<char>(ifs)),
                                 std::istreambuf_iterator<char>());
  // iterate through string and increase frequency of each character
  for (char c : text) {
    freq[c]++;
  }
  // create priority q of huffman node pointers with custom comparator
  PQueue<HuffmanNode *, HuffmanNodePtr> nodes;
  // push each map entry as huffman node
  for (std::map<char, int>::iterator it = freq.begin(); it != freq.end();
       it++) {
    nodes.Push(new HuffmanNode(it->first, it->second, nullptr, nullptr));
  }
  HuffmanNode *node1 = nullptr;
  HuffmanNode *node2 = nullptr;
  // continue looping until only one huffman node pointer left
  while (nodes.Size() != 1) {
    // set node 1 to top
    node1 = nodes.Top();
    // pop node 1
    nodes.Pop();
    // set node 2 to top
    node2 = nodes.Top();
    // pop node 2
    nodes.Pop();
    // push new huffman internal node with freq equal to addition with left
    // pointer to node 1 and right pointer to node 2
    nodes.Push(
        new HuffmanNode('0', node1->freq() + node2->freq(), node1, node2));
  }
  // set root to only huffman node left
  HuffmanNode *root = nodes.Top();
  // declare stack for pre-order iteration
  std::stack<HuffmanNode *> stack;
  // push root to stack
  stack.push(root);
  PrintTree(stack, bos);
  // set size to lenght of string
  int size = text.length();
  // put size in compression
  bos.PutInt(size);
  // make map for code table
  std::map<char, std::string> code_table;
  // declare string to store code
  std::string s;
  // call function to build code table
  BuildTable(root, s, code_table, 0);
  PrintEncoded(text, code_table, bos);
}

void Huffman::PrintEncoded(std::string &text,
                           std::map<char, std::string> &code_table,
                           BinaryOutputStream &bos) {
  // iterate through each character in file string
  for (char c : text) {
    // for each character in code table, iterate through code
    for (char c : code_table[c]) {
      // place bits equal to code string
      if (c == '0') {
        bos.PutBit(0);
      } else {
        bos.PutBit(1);
      }
    }
  }
}

void Huffman::PrintTree(std::stack<HuffmanNode *> stack,
                        BinaryOutputStream &bos) {
  // loop until stack is empty
  while (!stack.empty()) {
    HuffmanNode *n = stack.top();
    // if node is leaf, put 1 and put character contained in node
    if (n->IsLeaf()) {
      bos.PutBit(1);
      bos.PutChar(n->data());
      // if node is internal, put 0
    } else {
      bos.PutBit(0);
    }
    stack.pop();
    if (n->right()) stack.push(n->right());
    if (n->left()) stack.push(n->left());
  }
}

void Huffman::BuildTable(HuffmanNode *node, std::string s,
                         std::map<char, std::string> &code_table,
                         unsigned int i) {
  // if node has left child, add 0
  if (node->left()) {
    if (i == s.length()) {
      s += '0';
    } else {
      s[i] = '0';
    }
    BuildTable(node->left(), s, code_table, i + 1);
  }
  // if node has right child, add 1
  if (node->right()) {
    if (i == s.length()) {
      s += '1';
    } else {
      s[i] = '1';
    }
    BuildTable(node->right(), s, code_table, i + 1);
  }
  // if node is leaf add corresponding code to code table
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
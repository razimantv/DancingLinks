#ifndef TRIE_H_RTXFOUAK
#define TRIE_H_RTXFOUAK

#include <string>

// Trie node structure
// Only lowercase English letters are considered
struct TrieNode {
  TrieNode* children[26];                // Pointers to children nodes
  bool end;                              // Flag to indicate end of a word
  TrieNode();                            // Constructor
  ~TrieNode();                           // Destructor
  void insert(const std::string& word);  // Insert a word into the trie
};

#endif /* end of include guard: TRIE_H_RTXFOUAK */

#include "Trie.h"

#include <string>

// Constructor
TrieNode::TrieNode() : children(), end(false) {}

// Destructor: Recursively delete all children
TrieNode::~TrieNode() {
  for (auto child: children)
    if (child) delete child;
}

// Insert a word into the trie
void TrieNode::insert(const std::string& word) {
  TrieNode* node = this;

  // Iterate through trie nodes for each character in the word
  for (char c : word) {
    // If the child node doesn't exist, create it
    if (!node->children[c - 'a']) node->children[c - 'a'] = new TrieNode();
    node = node->children[c - 'a'];
  }
  node->end = true; // Mark the end of the word
}

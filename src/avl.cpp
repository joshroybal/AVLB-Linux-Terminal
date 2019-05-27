#include <string>
#include <algorithm>
#include <iostream>
#include <cstring>
#include "avl.hpp"

// public class methods
void AVL::insert(const std::string& key, int idx_no)
{
   _root = insert(_root, key, idx_no);
}

// private class methods
// AVL insertion
int AVL::balance_factor(const Node* node) const
{
   if ( !node ) return 0;
   return node_height(node->_left) - node_height(node->_right);
}

AVL::Node* AVL::insert(Node* p, const std::string& key, int idx_no)
{
   // BST begin
   if ( !p ) return new Node(key, idx_no);

   if (key < p->_key) p->_left = insert(p->_left, key, idx_no);
   else if (key > p->_key) p->_right = insert(p->_right, key, idx_no);
   else { p->_index.push_back(idx_no); return p; }

   p->_height = 1 + std::max(node_height(p->_left), node_height(p->_right));
   // BST end

   // AVL begin
   int bal = balance_factor(p);
   if (bal >= -1 && bal <= 1) return p;
   int left_bal = balance_factor(p->_left);
   int right_bal = balance_factor(p->_right);

   // Left Left Case
   if (bal > 1 && left_bal > 0) {
      return rotateRight(p);
   }

   // Right Right Case
   if (bal < -1 && right_bal < 0) {
      return rotateLeft(p);
   }

   // Left Right Case
   if (bal > 1 && left_bal < 0) {
      p->_left = rotateLeft(p->_left);
      return rotateRight(p);
   }

   // Right Left Case
   if (bal < -1 && right_bal > 0) {
      p->_right = rotateRight(p->_right);
      return rotateLeft(p);
   }

   return p;
}

// just permute some pointers
AVL::Node* AVL::rotateLeft(Node* p)
{
   Node* q = p->_right;
   Node* localtemp = q->_left;

   q->_left = p;
   p->_right = localtemp;

   p->_height = std::max(node_height(p->_left), node_height(p->_right)) + 1;
   q->_height = std::max(node_height(q->_left), node_height(q->_right)) + 1;

   return q;
}

// just permute some pointers
AVL::Node* AVL::rotateRight(Node* p)
{
   Node* q = p->_left;
   Node* localtemp = q->_right;

   q->_right = p;
   p->_left = localtemp;

   p->_height = std::max(node_height(p->_left), node_height(p->_right)) + 1;
   q->_height = std::max(node_height(q->_left), node_height(q->_right)) + 1;

   return q;
}

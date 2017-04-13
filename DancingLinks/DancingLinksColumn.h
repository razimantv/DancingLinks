/* DancingLinks.h : Header file for DancingLinksColumn class
 *
 * A column in the adjacency doubly-linked list
 *
 * Author: Raziman T V
 *
 * License:
 * You may use this document for whatever purpose you desire, as long as the
 * following conditions are satisfied:
 * 1) You do not use it in a way that suggests that I endorse the use.
 * 2) If you redistribute this code, I would be grateful if you credit me as
 * the author of the code. This is not a requirement.
 * 3) If this code is modified and redistributed with credit to me as the
 * original author, the code should either contain a link to the source
 * where the unaltered code can be found, or state explicitly that the code
 * has been altered significantly since.
 */

#ifndef DANCINGLINKSCOLUMN_H
#define DANCINGLINKSCOLUMN_H

class DancingLinksCell;

// Columns in the adjacency list
class DancingLinksColumn {
 protected:
  // Horizontal neighbours
  DancingLinksColumn *left_ptr_, *right_ptr_;

  // Dummy cell to ease vertical movement
  DancingLinksCell *dummycell_;

  // Number of non-zero elements in the column
  int size_;

  // Whether the column is an essential constraint (Number of 1-elements in
  //  the column == 1) or a non-essential constraint (number of 1-elements in
  //  the column <= 1)
  bool essential_;

 public:
  DancingLinksColumn();
  ~DancingLinksColumn();

  // Accessors
  DancingLinksColumn *left_ptr() { return left_ptr_; }
  DancingLinksColumn *right_ptr() { return right_ptr_; }

  DancingLinksCell *dummycell() { return dummycell_; }

  int size() { return size_; }
  bool essential() { return essential_; }

  // Mutators
  void set_left_ptr(DancingLinksColumn *ptr) { left_ptr_ = ptr; }
  void set_right_ptr(DancingLinksColumn *ptr) { right_ptr_ = ptr; }

  void set_essential(bool flag) { essential_ = flag; }
  void set_size(int size) { size_ = size; }
  void IncrementSize() { size_++; }
  void DecrementSize() { size_--; }

  // Remove a column and all rows which give 1-cells in it
  void RemoveColumn();

  // Put the removed column back
  void UnremoveColumn();
};

#endif

/* DancingLinks.cpp : Implementation for DancingLinksColumn class
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

#include "DancingLinksColumn.h"
#include "DancingLinksCell.h"
#include <cstdlib>

DancingLinksColumn::DancingLinksColumn() {
  left_ptr_ = right_ptr_ = NULL;
  dummycell_ = new DancingLinksCell;
  size_ = 0;
  essential_ = true;
}

DancingLinksColumn::~DancingLinksColumn() { delete dummycell_; }

void DancingLinksColumn::RemoveColumn() {
  // Remove the column from the column list
  left_ptr_->set_right_ptr(right_ptr_);
  right_ptr_->set_left_ptr(left_ptr_);

  // Find the 1-cells in the column and remove the other 1-cells in those rows
  //  from their respective columns
  for (DancingLinksCell *ptr = dummycell_->down_ptr(); ptr != dummycell_;
       ptr = ptr->down_ptr()) {
    // Found a 1-cell in the column
    // Erase the row by finding its row neighbours and removing their
    //  vertical links
    for (DancingLinksCell *ptr2 = ptr->right_ptr(); ptr2 != ptr;
         ptr2 = ptr2->right_ptr()) {
      // 1-cell in the row
      // Remove vertical links
      ptr2->RemoveCell();
    }
  }
}

void DancingLinksColumn::UnremoveColumn() {
  // Put back the cells in the reverse order as they were removed
  for (DancingLinksCell *ptr = dummycell_->up_ptr(); ptr != dummycell_;
       ptr = ptr->up_ptr()) {
    for (DancingLinksCell *ptr2 = ptr->left_ptr(); ptr2 != ptr;
         ptr2 = ptr2->left_ptr()) {
      ptr2->UnremoveCell();
    }
  }

  // Insert the column to the column list
  left_ptr_->set_right_ptr(this);
  right_ptr_->set_left_ptr(this);
}


/* DancingLinksCell.cpp : Implementation for DancingLinksCell class
 *
 * A general cell in the adjacency doubly-linked list
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

#include "DancingLinksCell.h"
#include <cstdlib>

DancingLinksCell::DancingLinksCell() {
  left_ptr_ = right_ptr_ = up_ptr_ = down_ptr_ = NULL;
  column_ptr_ = NULL;
  row_id_ = -1;
}

void DancingLinksCell::RemoveCell() {
  up_ptr_->set_down_ptr(down_ptr_);
  down_ptr_->set_up_ptr(up_ptr_);
  column_ptr_->DecrementSize();
}

void DancingLinksCell::UnremoveCell() {
  up_ptr_->set_down_ptr(this);
  down_ptr_->set_up_ptr(this);
  column_ptr_->IncrementSize();
}

void DancingLinksCell::Erase() {
  RemoveCell();
  left_ptr_->set_right_ptr(right_ptr_);
  right_ptr_->set_left_ptr(left_ptr_);
}


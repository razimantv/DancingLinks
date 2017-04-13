/* DancingLinks.cpp : Implementation for DancingLinks class
 * This is where all the real magic happens
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

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <set>

#include "DancingLinks.h"
#include "DancingLinksCell.h"
#include "DancingLinksColumn.h"

DancingLinks::DancingLinks() {
  dummycolumn_ = new DancingLinksColumn;
  dummycolumn_->set_essential(false);
  columns_ = NULL;
  cells_ = NULL;
}

DancingLinks::~DancingLinks() {
  Destroy();
  delete dummycolumn_;
}

void DancingLinks::Create(int R, int C1, int C2, char *mat) {
  // Create linked structure
  assert((R >= 0) && (C1 >= 0) && (C2 >= 0));
  int C = C1 + C2;

  // Start by destroying existing structure, if any
  Destroy();

  // Allocate memory for the full structure
  // Being lavish here, essentially treating it like an adjacency matrix
  //  C DancingLinksColumns for columns and R*C DancingLinksCells for cells_
  //  TODO: Convert to an adjacency list maybe

  // First, the C columns
  DancingLinksColumn *columns_ = new DancingLinksColumn[C];

  // Set left and right links in both directions for the head
  dummycolumn_->set_right_ptr(&columns_[0]);
  dummycolumn_->set_left_ptr(&columns_[C - 1]);
  columns_[0].set_left_ptr(dummycolumn_);
  columns_[C - 1].set_right_ptr(dummycolumn_);

  // Set left and right pointers for remaining column elements
  for (int i = 0; i < C - 1; i++) {
    columns_[i].set_right_ptr(&columns_[i + 1]);
    columns_[i + 1].set_left_ptr(&columns_[i]);
  }

  // Set size and essentiality values for each column
  for (int i = 0; i < C; i++) {
    columns_[i].set_size(R);
    columns_[i].set_essential(i < C1);
  }

  // Now the cells
  DancingLinksCell *cells_ = new DancingLinksCell[R * C];

  // Assign row and column IDs and neighbours for all cells
  for (int i = 0, k = 0; i < R; i++) {
    for (int j = 0; j < C; j++, k++) {
      cells_[k].set_row_id(i);
      cells_[k].set_column_ptr(&columns_[j]);

      if (k % C == 0)
        cells_[k].set_left_ptr(&cells_[k + C - 1]);
      else
        cells_[k].set_left_ptr(&cells_[k - 1]);

      if (k % C == C - 1)
        cells_[k].set_right_ptr(&cells_[k - C + 1]);
      else
        cells_[k].set_right_ptr(&cells_[k + 1]);

      if (k < C) {
        cells_[k].set_up_ptr(columns_[j].dummycell());
        columns_[j].dummycell()->set_down_ptr(&cells_[k]);
      } else
        cells_[k].set_up_ptr(&cells_[k - C]);

      if (k >= (R - 1) * C) {
        cells_[k].set_down_ptr(columns_[j].dummycell());
        columns_[j].dummycell()->set_up_ptr(&cells_[k]);
      } else
        cells_[k].set_down_ptr(&cells_[k + C]);
    }
  }

  // If a matrix element is 0, delete it by changing neighbours' pointers
  for (int i = 0, k = 0; i < R; i++) {
    for (int j = 0; j < C; j++, k++) {
      if (mat[k] == 0) cells_[k].Erase();
    }
  }
}

void DancingLinks::Destroy() {
  // Delete memory allocated to the children at the header
  // Of course, use this only at the header and only after allocation has
  //  been done. Don't blame me if you screw up
  if (columns_ != NULL) {
    delete[] columns_;
    columns_ = NULL;
  }
  if (cells_ != NULL) {
    delete[] cells_;
    cells_ = NULL;
  }
  dummycolumn_->set_left_ptr(NULL);
  dummycolumn_->set_right_ptr(NULL);
}

bool DancingLinks::solve(std::vector<std::vector<int> > &allsolutions,
                         std::vector<int> &cursolution, bool allneeded) {
  if (dummycolumn_->right_ptr()->essential() == false) {
    // No more constraints left to be satisfied. Success
    allsolutions.push_back(cursolution);
    return true;
  }

  // Find the essential column with the lowest degree
  DancingLinksColumn *chosencolumn = dummycolumn_->right_ptr();
  int mincnt = chosencolumn->size();
  for (DancingLinksColumn *ptr = chosencolumn->right_ptr(); ptr->essential();
       ptr = ptr->right_ptr()) {
    if (ptr->size() < mincnt) {
      chosencolumn = ptr;
      mincnt = ptr->size();
    }
  }

  // No way to satisfy some constraint. Failure
  if (mincnt == 0) return false;
  bool flag = false;

  // Remove the chosen column
  chosencolumn->RemoveColumn();
  for (DancingLinksCell *ptr = chosencolumn->dummycell()->down_ptr();
       ptr != chosencolumn->dummycell(); ptr = ptr->down_ptr()) {
    // Pick this row in candidate solution

    // Remove columns for all other cells in this row
    cursolution.push_back(ptr->row_id());
    for (DancingLinksCell *ptr2 = ptr->right_ptr(); ptr2 != ptr;
         ptr2 = ptr2->right_ptr()) {
      ptr2->column_ptr()->RemoveColumn();
    }

    // Recurse to solve the modified board
    if (solve(allsolutions, cursolution, allneeded)) flag = true;

    // Unremove all those columns
    for (DancingLinksCell *ptr2 = ptr->right_ptr(); ptr2 != ptr;
         ptr2 = ptr2->right_ptr()) {
      ptr2->column_ptr()->UnremoveColumn();
    }
    cursolution.pop_back();

    // If only one solution needs to be found and we have found one, don't
    // bother
    // checking for the rest
    if ((flag == true) && (allneeded == false)) {
      chosencolumn->UnremoveColumn();
      return flag;
    }
  }
  // Unremove the chosen column
  chosencolumn->UnremoveColumn();
  return flag;
}


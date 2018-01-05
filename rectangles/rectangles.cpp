/* rectangle.cpp : Solve the rectangle/shihaku game using Dancing Links
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

#include "../DancingLinks/DancingLinks.h"
#include <cstring>
#include <iostream>
#include <iterator>
#include <tuple>

inline int elem(std::vector<std::vector<int>>& vec, int i, int j) {
  return (i < 0 || j < 0) ? 0 : vec[i][j];
}

int main() {
  // Read the dimensions of the board
  int H, W;
  std::cin >> H >> W;
  if (H < 1 || W<1) {
    std::cerr << "Invalid board size" << std::endl;
    return 0;
  }
  std::vector<std::vector<int>> board(H), prefixsum(H);

  // Read the board and compute prefix sums
  for (int i = 0; i < H; ++i) {
    board[i].resize(W);
    prefixsum[i].resize(W);
    for (int j = 0; j < W; ++j) {
      std::cin >> board[i][j];
      if (board[i][j] < 0) {
        std::cerr << "Invalid board size" << std::endl;
        return 0;
      }
      prefixsum[i][j] = elem(prefixsum, i, j - 1) + elem(prefixsum, i - 1, j) -
                        elem(prefixsum, i - 1, j - 1) + (board[i][j] > 0);
    }
  }

  // Generate the possible rectangles which contain the cell and have right area
  // These will be the subsets which we try to cover the set elements with
  std::vector<std::tuple<int, int, int, int>> rectangles; // i1, i2, j1, j2
  for (int i = 0; i < H; ++i) {
    for (int j = 0; j < W; ++j) {
      if (board[i][j] == 0) continue;
      // Loop over all possibilities of rectangle size
      for (int h = board[i][j] / (W + 1) + 1; h <= H; ++h) {
        if (board[i][j] % h) continue;
        int w = board[i][j] / h;

        // Loop over all possibilities of rectangle position
        for (int i1 = std::max(0, i - h + 1), i2 = i1 + h - 1;
             i1 <= i && i2 < H; ++i1, ++i2) {
          for (int j1 = std::max(0, j - w + 1), j2 = j1 + w - 1;
               j1 <= j && j2 < W; ++j1, ++j2) {
            // There should be only one non-zero cell in the rectangle
            int cnt = elem(prefixsum, i2, j2) - elem(prefixsum, i1 - 1, j2) -
                      elem(prefixsum, i2, j1 - 1) +
                      elem(prefixsum, i1 - 1, j1 - 1);
            if (cnt != 1) continue;
            rectangles.push_back(std::make_tuple(i1, i2, j1, j2));
          }
        }
      }
    }
  }

  // Rectangles are the rows and cells are the columns
  // Let us build the adjacency matrix now
  int R = rectangles.size(), C = H * W;
  char* mat;
  mat = new (std::nothrow) char[R * C];
  if (mat == NULL) {
    printf("Memory allocation error\n");
    return 1;
  }
  memset(mat, 0, R * C);
  for (int r = 0; r < rectangles.size(); ++r) {
    auto rect = rectangles[r];
    for (int i = std::get<0>(rect); i <= std::get<1>(rect); ++i) {
      for (int j = std::get<2>(rect); j <= std::get<3>(rect); ++j) {
        int c = i * W + j;
        mat[r * C + c] = 1;
      }
    }
  }

  // Solve using dancing links
  DancingLinks N;
  N.Create(R, C, 0, mat);
  std::vector<std::vector<int> > all;
  std::vector<int> cur;
  N.solve(all, cur, false);

  if(all.size() != 1) {
    std::cerr << "No solution found" << std::endl;
  }

  // Assign board vales as unique rectangle IDs
  for (int r = 0; r < all[0].size(); ++r) {
    auto rect = rectangles[all[0][r]];
    for (int i = std::get<0>(rect); i <= std::get<1>(rect); ++i) {
      for (int j = std::get<2>(rect); j <= std::get<3>(rect); ++j) {
        board[i][j] = r;
      }
    }
  }

  // And print the board
  for (const auto& row : board) {
    std::copy(row.begin(), row.end(),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
  }
  return 0;
}

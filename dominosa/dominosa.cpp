/* dominosa.cpp : Solve the dominosa game using Dancing Links
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
#include <cstdio>
#include <cstring>

using namespace std;

int main() {
  // Read the height of the board
  // In classic dominosa, the width and domino limit depend on H alone
  //  and need not be input separately
  int H, W, L;
  scanf("%d", &H);
  if (H < 1) {
    printf("Invalid board size\n");
    return 0;
  }
  W = H + 1;
  L = H;

  // Allocate memory for the board
  int **board;
  board = new (nothrow) int *[H];
  if (board == NULL) {
    printf("Memory allocation error\n");
    return 1;
  }
  for (int i = 0; i < H; i++) {
    board[i] = new (nothrow) int[W];
    if (board[i] == NULL) {
      printf("Memory allocation error\n");
      return 1;
    }
  }

  // Read the board
  for (int i = 0; i < H; i++)
    for (int j = 0; j < W; j++) {
      scanf("%d", &board[i][j]);
      if ((board[i][j] < 0) || (board[i][j] >= L)) {
        printf("Invalid entry found in the board\n");
        return 1;
      }
    }

  // Every domino is a subset. Hence R=H*(W-1)+W*(H-1)
  // Every position being filled and every pair of numbers appearing in a domino
  //  are essential constraints. Hence C1=H*W+L*L
  // There are no non-essential constraints. Hence C2=0
  int horiz_dom_count = H * (W - 1), vert_dom_count = W * (H - 1);
  int pos_count = H * W, dom_type_count = L * L;
  int R = horiz_dom_count + vert_dom_count, C1 = pos_count + dom_type_count,
      C2 = 0, C = C1 + C2;

  // Matrix to store the constraint adjacency matrix
  char *mat;
  mat = new (nothrow) char[R * C];
  if (mat == NULL) {
    printf("Memory allocation error\n");
    return 1;
  }
  memset(mat, 0, R * C);

  // Deal with horizontal dominoes from (i,j) to (i,j+1)
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W - 1; j++) {
      // The domino corresponds to row# i*(W-1)+j of adjacency matrix
      int r = i * (W - 1) + j;

      // Position constraint for (i,j) is column# i*W+j of adjacency matrix
      int c1 = i * W + j, c2 = i * W + j + 1;

      // Domino constraint for (d,e) is column# H*W+d*L+e of adjacency matrix
      int c3 = pos_count + board[i][j] * L + board[i][j + 1];
      int c4 = pos_count + board[i][j + 1] * L + board[i][j];

      mat[r * (H * W + L * L) + c1] = 1;
      mat[r * (H * W + L * L) + c2] = 1;
      mat[r * (H * W + L * L) + c3] = 1;
      mat[r * (H * W + L * L) + c4] = 1;
    }
  }

  // Deal with vertical dominoes from (i,j) to (i+1,j)
  for (int i = 0; i < H - 1; i++) {
    for (int j = 0; j < W; j++) {
      // The domino corresponds to row# horiz_dom_count+i*(W-1)+j
      int r = horiz_dom_count + i * W + j;

      // Columns of constraint are same as earlier
      int c1 = i * W + j, c2 = (i + 1) * W + j;
      int c3 = H * W + board[i][j] * L + board[i + 1][j],
          c4 = H * W + board[i + 1][j] * L + board[i][j];

      mat[r * (H * W + L * L) + c1] = 1;
      mat[r * (H * W + L * L) + c2] = 1;
      mat[r * (H * W + L * L) + c3] = 1;
      mat[r * (H * W + L * L) + c4] = 1;
    }
  }

  // Now just solve it with dancing links
  // Note that we are asking it to find only one solution
  DancingLinks N;
  N.Create(R, C1, C2, mat);
  std::vector<std::vector<int> > all;
  std::vector<int> cur;
  N.solve(all, cur, false);

  // If a solution exists, print it
  if (all.size() == 1) {
    char *outputboard;
    outputboard = new char[H * W];
    if (outputboard == NULL) {
      printf("Memory allocation error\n");
      return 1;
    }
    for (int i = 0; i < all.size(); i++) {
      for (int j = 0; j < all[i].size(); j++) {
        int id = all[i][j];
        if (id < H * (W - 1)) {
          int r = id / (W - 1), c = id % (W - 1);
          outputboard[r * W + c] = outputboard[r * W + c + 1] = '-';
        } else {
          id -= H * (W - 1);
          int r = id / W, c = id % W;
          outputboard[r * W + c] = outputboard[(r + 1) * W + c] = '|';
        }
      }

      for (int i = 0, k = 0; i < H; i++) {
        for (int j = 0; j < W; j++, k++) printf("%c", outputboard[k]);
        printf("\n");
      }
    }
  }
  return 0;
}

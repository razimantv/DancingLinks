/* nqueens.cpp : Solve the n-queens problem using Dancing Links
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
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

int main() {
  // Read N and whether the user wants the full list of solutions
  //  or just the count
  int N, getall;
  scanf("%d%d", &N, &getall);
  if (N < 1) {
    printf("Invalid board size\n");
    return 0;
  }

  // Every position of a queen is a subset. Hence R=N*N
  // Every row and every column should have exactly one queen. This gives 2*N
  //  essential constraints
  // Every diagonal should have at most one queen. This gives 4*N-2
  //  non-essential constraints
  int R = N * N, C1 = 2 * N, C2 = 4 * N - 2, C = C1 + C2;

  // Matrix to store the constraint adjacency matrix
  char *mat;
  mat = new (nothrow) char[R * C];
  if (mat == NULL) {
    printf("Memory allocation error\n");
    return 1;
  }
  memset(mat, 0, R * C);

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      // Queen position (i,j) corresponds to row# i*N+j of adjacency matrix
      int r = i * N + j;

      // Constraint of row i is column# i of adjacency matrix
      // Constraint of column i is column# N+i of adjacency matrix
      // Constraint of upward diagonal i is column# 2*N+i of adjacency matrix
      // Constraint of downward diagonal i is column# 4*N-1+i of adjacency
      // matrix
      int c1 = i, c2 = N + j, c3 = 2 * N + i + j, c4 = 5 * N - 2 + i - j;
      mat[r * C + c1] = 1;
      mat[r * C + c2] = 1;
      mat[r * C + c3] = 1;
      mat[r * C + c4] = 1;
    }
  }

  // Now just solve it with dancing links
  DancingLinks D;
  D.Create(R, C1, C2, mat);
  std::vector<std::vector<int> > all;
  std::vector<int> cur;
  D.solve(all, cur, true);

  // Number of solutions
  printf("%lu\n", all.size());

  if (getall != 0) {
    // User wants the locations of all the queens
    for (int i = 0; i < all.size(); i++) {
      printf("%d:\n", i);
      for (int j = 0; j < all[i].size(); j++) {
        printf("%d %d\n", all[i][j] / N, all[i][j] % N);
      }
    }
  }
  return 0;
}

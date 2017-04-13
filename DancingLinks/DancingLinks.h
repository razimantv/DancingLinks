/* DancingLinks.h : Header file for DancingLinks class
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

#include "DancingLinksColumn.h"
#include <vector>

// Forward declaration for all the classes
class DancingLinksCell;
class DancingLinksColumn;

// Head cell for the dancing links implementation
class DancingLinks {
 private:
  // Dummy column to ease horizontal movement
  DancingLinksColumn *dummycolumn_;

  // Pointers to store column and cell arrays
  DancingLinksColumn *columns_;
  DancingLinksCell *cells_;

 public:
  // Constructor
  DancingLinks();

  // Destructor
  ~DancingLinks();

  // Create link structure from adjacency matrix
  void Create(int R, int C1, int C2, char *mat);

  // Clear the link structure
  void Destroy();

  // Solve the problem
  bool solve(std::vector<std::vector<int> > &allsolutions,
             std::vector<int> &cursolution, bool allneeded = false);
};

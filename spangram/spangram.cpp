#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../DancingLinks/DancingLinks.h"
#include "Trie.h"

// Neighbours with 8-connectivity
std::vector<std::pair<int, int>> neighbours = {
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

using Cell = std::pair<int, int>;
using Path = std::vector<Cell>;
using Placement = std::pair<std::string, Path>;
using Grid = std::vector<std::string>;

bool contains_pair(const Path& path, const Cell& c1, const Cell& c2) {
  for (int i = 1, l = path.size(); i < l; ++i)
    if ((path[i] == c1 && path[i - 1] == c2) ||
        (path[i] == c2 && path[i - 1] == c1))
      return true;
  return false;
}

// Recursively find all placements of words in the grid
void find_placements(const Grid& grid, int m, int n, const TrieNode& node,
                     int i, int j, std::vector<std::vector<bool>>& visited,
                     Path& path, std::vector<Placement>& placements) {
  // Check if the current cell is valid and can extend the current word
  if (i < 0 || i >= m || j < 0 || j >= n || visited[i][j] ||
      !node.children[grid[i][j] - 'a'])
    return;

  // Process current cell
  visited[i][j] = true;
  path.push_back({i, j});
  const TrieNode* next = node.children[grid[i][j] - 'a'];
  if (next->end) {
    // If we are at a word end, we can have a placement here
    // Find the corresponding word and placement
    std::string word;
    for (const auto& [x, y] : path) word += grid[x][y];
    placements.push_back({word, path});
  }
  // Recurse to all neighbours
  for (const auto& [dx, dy] : neighbours)
    // If the step is diagonal, ensure it won't cause self-intersection
    if (abs(dx) != abs(dy) or !contains_pair(path, {i + dx, j}, {i, j + dy}))
      find_placements(grid, m, n, *next, i + dx, j + dy, visited, path,
                      placements);

  // Undo to backtrack
  path.pop_back();
  visited[i][j] = false;
}

// "Pretty" print the solution
void print_solution(const std::vector<std::string>& grid, int m, int n,
                    const std::vector<Placement>& placements) {
  // Output grid with spaces between characters to put paths
  std::vector<std::string> output(2 * m - 1, std::string(2 * n - 1, ' '));

  // Fill the output grid with the characters from the input grid
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++) output[2 * i][2 * j] = grid[i][j];

  // Process paths and output the words
  for (const auto& [word, path] : placements) {
    // Capitalise the first letter of the word
    auto [x, y] = path[0];
    std::cout << word << " ";
    output[2 * x][2 * y] = toupper(output[2 * x][2 * y]);

    // Draw the path on the output grid
    for (int i = 1, l = path.size(); i < l; i++) {
      int x1 = path[i - 1].first, y1 = path[i - 1].second, x2 = path[i].first,
          y2 = path[i].second, dx = x2 - x1, dy = y2 - y1;
      if (abs(dx) == 1 and dy == 0) {
        output[2 * x1 + dx][2 * y1] = '|';
      } else if (abs(dy) == 1 and dx == 0) {
        output[2 * x1][2 * y1 + dy] = '-';
      } else if (dx == dy) {
        output[2 * x1 + dx][2 * y1 + dy] = '\\';
      } else {
        output[2 * x1 + dx][2 * y1 + dy] = '/';
      }
    }
  }
  std::cout << "\n";

  // Print the output grid
  for (const auto& line : output) std::cout << line << "\n";
}

int main(int argc, char* argv[]) {
  std::unordered_set<std::string> exclude; // Words to exclude
  std::string filename = "words.txt";      // Default dictionary file
  int maxsel = -1;                         // Maximum number of words to select

  // Parse command line arguments
  //    Number: Maximum number of words to select
  //    Dot in the argument: Filename
  //    Otherwise: Words to exclude
  for (int i = 1; i < argc; i++) {
    auto cur = std::string(argv[i]);
    if (isdigit(cur[0])) {
      maxsel = std::stoi(cur);
    } else if (cur.find('.') != std::string::npos) {
      filename = cur;
    } else {
      exclude.insert(cur);
    }
  }

  // Create trie from all the words in the dictionary with >= 4 characters
  TrieNode trie;
  {
    std::ifstream dictionary(filename);
    for (std::string word; dictionary >> word;)
      if (word.size() >= 4 and not exclude.count(word)) trie.insert(word);
  }

  // Read the grid from the input file
  Grid grid;
  {
    for (std::string line; std::cin >> line;) {
      std::transform(line.begin(), line.end(), line.begin(),
                     [](unsigned char c) { return std::tolower(c); });
      grid.push_back(line);
    }
  }
  int rows = grid.size(), cols = grid[0].size();

  // Find all placements of words in the grid
  std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
  Path path;
  std::vector<Placement> placements;
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      find_placements(grid, rows, cols, trie, i, j, visited, path, placements);

  // Set up the adjacency matrix for dancing links
  // We want to cover the cells of the grid with words
  // Essential constraints: Every cell is covered by exactly one word
  // Non-essential constraints: The centre of a 2x2 square of characters can
  //    have at most one diagonal line passing through it (paths cannot cross)
  int R = placements.size(), C1 = rows * cols, C2 = (rows - 1) * (cols - 1),
      C = C1 + C2;
  char* mat = new (std::nothrow) char[R * C];
  if (mat == NULL) {
    printf("Memory allocation error\n");
    return 1;
  }
  memset(mat, 0, R * C);

  // Fill the adjacency matrix with the placements
  for (int i = 0; i < R; ++i) {
    const auto& [word, path] = placements[i];
    int l = path.size();
    for (const auto& [x, y] : path) mat[i * C + x * cols + y] = 1;
    for (int k = 1; k < l; ++k) {
      int x1 = path[k - 1].first, y1 = path[k - 1].second, x2 = path[k].first,
          y2 = path[k].second, dx = x2 - x1, dy = y2 - y1;
      if (abs(dx) == 1 and abs(dy) == 1)
        mat[i * C + C1 + std::min(x1, x2) * (cols - 1) + std::min(y1, y2)] = 1;
    }
  }

  // Now just solve it with dancing links
  DancingLinks D;
  D.Create(R, C1, C2, mat);
  std::vector<std::vector<int>> all;
  std::vector<int> cur;
  D.solve(all, cur, false, maxsel);

  // Print all solutions
  for (auto& sol : all) {
    std::vector<Placement> solution_placements;
    for (int i : sol) solution_placements.push_back(placements[i]);
    print_solution(grid, rows, cols, solution_placements);
    std::cout << "\n";
  }
  return 0;
}

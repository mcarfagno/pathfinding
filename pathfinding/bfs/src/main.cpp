#include <algorithm>
#include <unordered_map>
#include <queue>
#include <vector>
#include <iostream>

#include "breadth_first_search.cpp"
#include "read_map.cpp"

int main (int argc, char **argv)
{

  SquareGrid grid;
  open_pgn(grid, "../maps/test_map.pgm");
  
  GridLocation start{200, 200};
  GridLocation goal{300, 300};

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  std::unordered_map<GridLocation, GridLocation> came_from = breadth_first_search(grid, start, goal);

  //std::vector<GridLocation> path = reconstruct_path(start, goal, came_from);

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Elapsed = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

}

#include <vector>
#include <iostream>

#include "grid_with_weights.cpp"
// #include "grid.cpp"
#include "breadth_first_search.cpp"

int main (int argc, char **argv)
{
  // SquareGrid grid;
  //
  // //load locations from map file
  // grid.ALL_LOCATIONS.insert(GridLocation{0,0});
  // grid.ALL_LOCATIONS.insert(GridLocation{1,0});
  // grid.ALL_LOCATIONS.insert(GridLocation{2,0});
  // grid.ALL_LOCATIONS.insert(GridLocation{0,-1});
  //
  // std::vector<GridLocation> n = grid.neighbors(GridLocation{0,0});
  // for (auto i: n)
  // {
  //   std::cout << i.x << ' ' << i.y <<std::endl;
  // }
  //
  // auto parents = breadth_first_search(grid, GridLocation{0, 0});

  GridWithWeights grid;

  //load locations from map file
  grid.ALL_LOCATIONS.insert(GridLocation{0,0});
  grid.ALL_LOCATIONS.insert(GridLocation{1,0});
  grid.ALL_LOCATIONS.insert(GridLocation{1,1});
  grid.ALL_LOCATIONS.insert(GridLocation{1,-1});
  grid.ALL_LOCATIONS.insert(GridLocation{2,0});
  grid.ALL_LOCATIONS.insert(GridLocation{0,-1});

  std::vector<GridLocation> n = grid.neighbors(GridLocation{0,0});
  for (auto i: n)
  {
    // std::cout << i.x << ' ' << i.y <<std::endl;
    std::cout <<  "cost from [0,0] to ["<< i.x << ' ' << i.y << "]" <<std::endl;
    std::cout << grid.cost(GridLocation{0,0},i) <<std::endl;
  }

}

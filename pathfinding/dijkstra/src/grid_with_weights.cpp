#include <functional>
#include <unordered_set>
#include <vector>
#include <queue>
#include <cmath>
#include <iostream>

struct GridLocation
{
  int x, y, w;
};

/* implement hash function so we can put GridLocation into an unordered_set */
namespace std
{
  template <>
  struct hash<GridLocation>
  {
    std::size_t operator()(const GridLocation& id) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      return std::hash<int>()(id.x ^ (id.y << 4));
    }
  };
}

/* implement equal function so we can find GridLocation into an unordered_set */
bool operator == (const GridLocation &id1, const GridLocation &id2)
{
   if( (id1.x == id2.x)&&(id1.y == id2.y) )
     return true;
   else
     return false;
}

/* implement equal function so we can find GridLocation into an unordered_set */
bool operator < (const GridLocation &id1, const GridLocation &id2)
{
   if( (id1.w < id2.w) )
     return true;
   else
     return false;
}

/* implement not equal function so we can find GridLocation into an unordered_set */
bool operator != (const GridLocation &id1, const GridLocation &id2)
{
   if( (id1.x == id2.x)&&(id1.y == id2.y) )
     return false;
   else
     return true;
}

struct GridWithWeights
{
    /* all locations in the grid-> GRAPH NODES*/
    // std::vector<GridLocation> ALL_LOCATION;
    std::unordered_set<GridLocation> ALL_LOCATIONS; //set is more efficient

    const std::array<GridLocation, 8> DIRS = {GridLocation{1, 0},
                                            GridLocation{0, 1},
                                            GridLocation{-1, 0},
                                            GridLocation{0, -1},
                                            GridLocation{-1, -1},
                                            GridLocation{-1, 1},
                                            GridLocation{1, -1},
                                            GridLocation{1, 1}};

    /* check if a location is in the grid */
    bool in_bounds(GridLocation id) const
    {
      return ALL_LOCATIONS.find(id) != ALL_LOCATIONS.end();
    }

    /* get a location's neighbours -> GRAPH EDGES*/
    std::vector<GridLocation> neighbors(GridLocation id)
    {
      std::vector<GridLocation> results;

      for (GridLocation dir : DIRS)
      {

        GridLocation next{id.x + dir.x, id.y + dir.y };
        if (in_bounds(next))
        {
          auto element = ALL_LOCATIONS.find(next);
          /* */
          //next.w = element->w;
          results.push_back(*element);
        }
      }

      return results;
    }

    /*cost of moving from one cell to the other -> GRAPH EDGES COST*/
    double cost(GridLocation from_node, GridLocation to_node) const
    {
      return 1.0*to_node.w ;
    }

};

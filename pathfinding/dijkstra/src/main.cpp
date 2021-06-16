#include <unordered_map>
#include <queue>
#include <vector>
#include <iterator>
#include <algorithm> //std reverse

#include "read_costmap.cpp"
#include "dijkstra_search.cpp"

void overlay(std::vector<GridLocation> &path)
{

  Mat input_img = imread("../maps/test_map.pgm", IMREAD_UNCHANGED);
  //input_img = 255-input_img;

  // create 8bit color image. IMPORTANT: initialize image otherwise it will result in 32F
  Mat img_rgb(input_img.size(), CV_8UC3);

  // convert grayscale to color image
  cvtColor(input_img, img_rgb, COLOR_GRAY2RGB );

  for (auto i = path.begin(); i != path.end(); ++i)
    img_rgb.at<Vec3b>(i->x, i->y) = Vec3b(128, 0, 255);

  imshow("path", img_rgb);
  waitKey();

  imwrite( "images/path.jpg", img_rgb);

  return;
}

int main()
{
  GridWithWeights grid ;
  open_pgn(grid, "../maps/test_map.pgm");

  // for (auto const& i: grid.ALL_LOCATIONS) {
	// 	// std::cout << (int)i.w << std::endl;
  //   std::cout << grid.cost(i,i) << std::endl;
	// }

  GridLocation start{200, 200};
  GridLocation goal{300, 300};
  std::unordered_map<GridLocation, GridLocation> came_from;
  std::unordered_map<GridLocation, double> cost_so_far;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  dijkstra_search(grid, start, goal, came_from, cost_so_far);
  std::vector<GridLocation> path = reconstruct_path(start, goal, came_from);

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Elapsed = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

  //print path
  // for (auto i = path.begin(); i != path.end(); ++i)
  //   std::cout << i->x << ", " << i->y << std::endl;

  overlay(path);

}

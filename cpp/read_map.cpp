#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <chrono>

#include "grid.cpp"

using namespace cv;

/* Fills graph with freespace data from .pgm map */
void open_pgn(SquareGrid& grid, const char* location)
{
    Mat occ_map;
    bool invert_values=true;

    // read binary 8UC1
    Mat input_img = imread(location, IMREAD_UNCHANGED);

    // check if data has to be inverted:
    // occ ~1 and free ~0
    if(invert_values == true)
    {
      input_img = 255-input_img;
    }

    // covert 0,255 to 0,1 occupancy range
    input_img.convertTo(occ_map,CV_32FC1, 1.0/255.0);

    double min, max;
    cv::minMaxLoc(occ_map, &min, &max);
    std::cout << min << " " << max << std::endl;

    //unknown space value >= 0.196 -->OK
    std::cout << (float)occ_map.at<float>(0,0) << std::endl;

    //free space value < 0.196 -->OK
    std::cout << (float)occ_map.at<float>(300,300) << std::endl;

    imshow("Occupancy Map", occ_map);
    waitKey();

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // todo freespace to graph
    std::cout << "Converting map to graph" << std::endl;

    float freespace_tresh = 0.196;

    for(int i=0; i<occ_map.rows; i++)
    {
      for(int j=0; j<occ_map.cols; j++)
      {
        if (occ_map.at<float>(i,j) < freespace_tresh)
        {
          grid.ALL_LOCATIONS.insert(GridLocation{i,j});
        }
      }
    }

    //std::cout << grid.ALL_LOCATIONS.size() << std::endl;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Elapsed = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    /*COMPUTE CELL "COST" BY computing distance from walls*/
    // Perform the distance transform algorithm
    Mat dist;
    input_img = input_img.setTo(255, input_img > 1); // set anything not free to 255
    input_img = 255 - input_img; // revert to occ ~0 and free ~255
    distanceTransform(input_img, dist, DIST_L2, 3);
    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize it
    normalize(dist, dist, 0, 1.0, NORM_MINMAX);
    imshow("Distance Transform Image", dist);
    waitKey();

    return;
}

int main (int argc, char **argv)
{
  SquareGrid my_grid;
  open_pgn(my_grid, "./maps/test_map.pgm");
  std::cout << my_grid.ALL_LOCATIONS.size() << std::endl;
}

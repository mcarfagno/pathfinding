#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <chrono>

#include "grid.cpp"

using namespace cv;

// load abd view map file
void open_pgn(char* location)
{
    Mat occ_map;
    bool invert_values=true;

    // read binary 8UC1
    Mat input_img = imread(location, IMREAD_UNCHANGED);

    // covert 0,255 to 0,1 occupancy range
    input_img.convertTo(occ_map,CV_32FC1, 1.0/255.0);

    // check if data has to be inverted:
    // occ ~1 and free ~0
    if(invert_values == true)
    {
      occ_map = 1-occ_map;
    }

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

    SquareGrid grid;
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

    std::cout << grid.ALL_LOCATIONS.size() << std::endl;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Elapsed = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    //return grid
}

int main (int argc, char **argv)
{
  open_pgn("./test_map.pgm");
}

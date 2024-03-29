#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <chrono>
#include <cstring>

#include "grid_with_weights.cpp"

using namespace cv;

const unsigned char LETHAL_OBSTACLE = 255;
const unsigned char INSCRIBED_INFLATED_OBSTACLE = 254;

// from: https://github.com/ros-planning/navigation/blob/jade-devel/costmap_2d/include/costmap_2d/inflation_layer.h#L113
inline unsigned char computeCost(double distance)
{
  float inscribed_radius_ = 0.25; //robot radius
  float resolution_ = 0.05; //map resolution
  float weight_ = 5.;

  unsigned char cost = 0;
  if (distance == 0)
    cost = LETHAL_OBSTACLE;
  else if (distance * resolution_ <= inscribed_radius_)
    cost = INSCRIBED_INFLATED_OBSTACLE;
  else
  {
    // make sure cost falls off by Euclidean distance
    double euclidean_distance = distance * resolution_;
    double factor = std::exp(-1.0 * weight_ * (euclidean_distance - inscribed_radius_));
    cost = (unsigned char)((INSCRIBED_INFLATED_OBSTACLE - 1) * factor);
  }
  return cost;
}

/* Returns a 8bit image where 255 is fully occupied space and decreases to zero in freespace */
Mat compute_costmap(Mat& input_img)
{
  // Perform the distance transform algorithm
  Mat dist;
  input_img = input_img.setTo(255, input_img > 1); // set anything not free to 255
  input_img = 255 - input_img; // revert to occ ~0 and free ~255

  /* L2 returns euclidean distance CV_32FC1 image.
   where every element is pixel distance from non-free pixels */
  distanceTransform(input_img, dist, DIST_L2, 5);
  //imwrite( "images/distance_trasform.jpg", dist);

  Mat computed_costmap = Mat::zeros(dist.rows, dist.cols, CV_8UC1);
  for(int i=0; i<computed_costmap.rows; i++)
  {
    for(int j=0; j<computed_costmap.cols; j++)
    {
        computed_costmap.at<uchar>(i, j) = computeCost(dist.at<float>(i,j));
    }
  }

  // pretty images
  // make a color version for printing
  //Mat coulored_costmap;
  //applyColorMap(computed_costmap, coulored_costmap, COLORMAP_COOL);
  // imshow("CostMap Image", coulored_costmap);
  // waitKey();
  //imwrite( "images/costmap.jpg", coulored_costmap);
  //return computed_costmap;

  // // Normalize the distance image for range = {0.0, 1.0}
  // // so we can visualize it
  // normalize(dist, dist, 0, 1.0, NORM_MINMAX);
  // dist.convertTo(dist,CV_8UC1, 255.0);
  // imwrite( "images/distance_trasform_minmax.jpg", dist);

  return computed_costmap;
}

/* Fills graph with freespace data from .pgm map */
void open_pgn(GridWithWeights& grid, const char* location)
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


    Mat cmap;
    cmap = compute_costmap(input_img);

    // freespace to graph
    std::cout << "Converting map to graph" << std::endl;

    /*
    the costmap is a matrix of 8bit unsigned chars elements
    that have to be parsed as integer value in 0-255
    */
    int freespace_tresh = 254;
    for(int i=0; i<cmap.rows; i++)
    {
      for(int j=0; j<cmap.cols; j++)
      {
        if ((int)cmap.at<uchar>(i,j) < freespace_tresh)
        {
          grid.ALL_LOCATIONS.insert(GridLocation{i,j,(int)cmap.at<uchar>(i,j)});
        }
      }
    }

    //std::cout << grid.ALL_LOCATIONS.size() << std::endl;

    return;
}

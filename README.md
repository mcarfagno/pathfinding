pathfinding

## About
The idea is to use ROS occupancy grid maps to perform some path planning, for the sake of efficiciency I made everything in C++ and OpenCV.

| Input Occupancy Map | Pixel Distance Map | 
|:-------------------------:|:-------------------------:|
|![](pathfinding/images/input.jpg =400x) | ![](pathfinding/images/distance_trasform_minmax.jpg =400x)|

| Cost Map | Path Map Overlay | 
|:-------------------------:|:-------------------------:|
|![](pathfinding/images/costmap.jpg =400x) | ![](pathfinding/images/path.jpg =400x)|

## TODO
* Convert the path from cell coordinats to cartesian using the map data from the **.yaml**

## References
* [Red Blob Games](https://www.redblobgames.com/pathfinding/a-star/implementation.html#cplusplus)
* [ROS Wiki on 2D costmaps](http://wiki.ros.org/costmap_2d)
* [ros-planning on github](https://github.com/ros-planning/navigation)
* [OpenCV distance transform](https://docs.opencv.org/3.4/d2/dbd/tutorial_distance_transform.html)

cmake_minimum_required(VERSION 2.8)

project( OccMap )
find_package( OpenCV REQUIRED )

add_executable( read_map read_costmap.cpp )
target_link_libraries( read_map ${OpenCV_LIBS} )

add_executable( test_pathfinding astar_test.cpp )
target_link_libraries( test_pathfinding ${OpenCV_LIBS} )

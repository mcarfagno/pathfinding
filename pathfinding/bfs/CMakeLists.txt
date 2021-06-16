cmake_minimum_required(VERSION 2.8)

# optimization flags for performance
set(CMAKE_CXX_FLAGS "-O2")

project( OccMap )
find_package( OpenCV REQUIRED )

# add_executable( read_map read_costmap.cpp )
# target_link_libraries( read_map ${OpenCV_LIBS} )

add_executable( bfs_pathfinding src/main.cpp )
target_link_libraries( bfs_pathfinding ${OpenCV_LIBS} )

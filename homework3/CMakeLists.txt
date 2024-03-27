cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(OpenCV_DIR "D:/Learn/ThirdPart/opencv")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 REQUIRED)
include_directories(/usr/local/include)

include_directories("C:/Program Files (x86)/Eigen3/include/eigen3")
# include_directories("D:/Learn/ThirdPart/opencv/build/bin")
# include_directories("D:/Learn/ThirdPart/opencv/build/include/opencv2")
add_executable(Rasterizer main.cpp rasterizer.hpp rasterizer.cpp global.hpp Triangle.hpp Triangle.cpp Texture.hpp Texture.cpp Shader.hpp OBJ_Loader.h)
target_link_libraries(Rasterizer ${OpenCV_LIBS})
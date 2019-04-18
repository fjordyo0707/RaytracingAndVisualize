cmake_minimum_required(VERSION 3.13)
project(ray)

message( "PROJECT_SOURCE_DIR : " ${PROJECT_SOURCE_DIR} )
message( "CMAKE_CURRENT_SOURCE_DIR : " ${CMAKE_CURRENT_SOURCE_DIR} )
set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)

if(WIN32)
  set(OpenCV_DIR "d:/libs/opencv-2.4.8/build")
else()
  set(OpenCV_DIR "/usr/lib/opencv")
endif()

find_package(OpenCV)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${OpenCV_INCLUDE_DIRS}
)

set(ray_SOURCE_FILES
  ${PROJECT_SOURCE_DIR}/src/Scene.cpp
  ${PROJECT_SOURCE_DIR}/src/Ray.cpp
  ${PROJECT_SOURCE_DIR}/src/utils.cpp
)


add_library(mylib ${ray_SOURCE_FILES})
add_executable(output ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(output mylib ${OpenCV_LIBS})
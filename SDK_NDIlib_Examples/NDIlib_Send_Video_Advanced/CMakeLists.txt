cmake_minimum_required(VERSION 3.23)

project(NDIlib_Send_Video_Advanced)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY Bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY Bin)
set (CMAKE_BUILD_TYPE Release)

add_executable(Send_Video_Advanced NDIlib_Send_Video_Advanced.cpp)

target_include_directories(Send_Video_Advanced PRIVATE "C:/Program Files/NDI/NDI 5 SDK/Include")

target_link_libraries(Send_Video_Advanced "C:/Program Files/NDI/NDI 5 SDK/Lib/x64/Processing.NDI.Lib.x64.lib")
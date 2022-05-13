cmake_minimum_required(VERSION 3.12)

project(NDIlib_Tally_Echo)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY Bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY Bin)

add_executable(Tally_Echo NDIlib_Tally_Echo.cpp)

target_include_directories(Tally_Echo PRIVATE "C:/Program Files/NDI/NDI 5 SDK/Include")

target_link_libraries(Tally_Echo "C:/Program Files/NDI/NDI 5 SDK/Lib/x64/Processing.NDI.Lib.x64.lib")
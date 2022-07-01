cmake_minimum_required(VERSION 3.12)

project(NDIlib_Examples)

set(CMAKE_INSTALL_PREFIX ../..)

add_executable(Tally_Echo NDIlib_Tally_Echo/NDIlib_Tally_Echo.cpp)
add_executable(Send_Video_Advanced NDIlib_Send_Video_Advanced/NDIlib_Send_Video_Advanced.cpp)

target_include_directories(Tally_Echo PRIVATE "C:/Program Files/NDI/NDI 5 SDK/Include")
target_include_directories(Send_Video_Advanced PRIVATE "C:/Program Files/NDI/NDI 5 SDK/Include")

target_link_libraries(Tally_Echo "C:/Program Files/NDI/NDI 5 SDK/Lib/x64/Processing.NDI.Lib.x64.lib")
target_link_libraries(Send_Video_Advanced "C:/Program Files/NDI/NDI 5 SDK/Lib/x64/Processing.NDI.Lib.x64.lib")

install(TARGETS Tally_Echo Send_Video_Advanced
        DESTINATION deploy)

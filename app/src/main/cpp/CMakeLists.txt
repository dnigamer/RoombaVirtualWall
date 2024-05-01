cmake_minimum_required(VERSION 3.22.1)

project("rvwmain")

add_library(${CMAKE_PROJECT_NAME} SHARED
        rvwmain.cpp
        IRBuilder.cpp
        IRBuilder.hpp
        IRExpressionBuilder.cpp
        IRExpressionBuilder.hpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
    android
    log)

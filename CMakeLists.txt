cmake_minimum_required(VERSION 4.0.2)

project(TriLogic)

set(SRC "${CMAKE_SOURCE_DIR}/src")
include_directories(${SRC}) 

add_executable(${PROJECT_NAME} ${SRC}/main.cpp) 
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 
                                                 CXX_STANDARD_REQUIRED ON)

                                                 
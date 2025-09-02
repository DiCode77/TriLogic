cmake_minimum_required(VERSION 4.0.2)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(TriLogic)

set(SRC "${CMAKE_SOURCE_DIR}/src")

include_directories(${SRC}) 

find_package(wxWidgets CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SRC}/main.cpp) 
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 
                                                 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT_NAME} PRIVATE wx::core wx::base)

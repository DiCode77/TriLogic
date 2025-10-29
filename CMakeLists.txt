cmake_minimum_required(VERSION 4.0.2)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
set(MINIMUM_APP_VERSION "14.6")
set(CMAKE_OSX_DEPLOYMENT_TARGET "${MINIMUM_APP_VERSION}" CACHE STRING "Minimum OS X deployment version")

project(TriLogic)

set(SRC "${CMAKE_SOURCE_DIR}/src")

file(GLOB IMAGE_FILES CONFIGURE_DEPENDS 
                    "${CMAKE_SOURCE_DIR}/resources/TriLogic.icns" 
                    "${CMAKE_SOURCE_DIR}/resources/*.png")

include_directories(${SRC}) 

find_package(wxWidgets CONFIG REQUIRED)

file(GLOB SRC_FILES "${SRC}/*.cpp" "${SRC}/*.h" "${SRC}/*.hpp")
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC_FILES}) 

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 
                                                 CXX_STANDARD_REQUIRED ON
                                                 MACOSX_BUNDLE TRUE
                                                 MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
                                                 XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
                                                 XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
                                                 MACOSX_DEPLOYMENT_TARGET "${MINIMUM_APP_VERSION}")


                                                 
set_source_files_properties(${IMAGE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources") 

target_link_libraries(${PROJECT_NAME} PRIVATE wx::core wx::base wx::propgrid)

target_sources(${PROJECT_NAME} PRIVATE ${IMAGE_FILES})
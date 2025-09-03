cmake_minimum_required(VERSION 4.0.2)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
set(MINIMEM_APP_VERSION "15.0")
set(CMAKE_OSX_DEPLOYMENT_TARGET "${MINIMEM_APP_VERSION}" CACHE STRING "Minimum OS X deployment version")

project(TriLogic)

set(SRC "${CMAKE_SOURCE_DIR}/src")
set(ICON_MACOS_NAME "TriLogic.icns")
set(APP_ICON_MACOSX "${CMAKE_SOURCE_DIR}/resources/${ICON_MACOS_NAME}")

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
                                                 MACOSX_DEPLOYMENT_TARGET "${MINIMEM_APP_VERSION}")

                                                 

set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources") 

target_link_libraries(${PROJECT_NAME} PRIVATE wx::core wx::base)

target_sources(${PROJECT_NAME} PRIVATE ${APP_ICON_MACOSX})

set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_ICON_FILE "${ICON_MACOS_NAME}")
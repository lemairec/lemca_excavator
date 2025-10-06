cmake_minimum_required(VERSION 3.10)

# Utilisation de C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lemca_excavator)

# The version number.
set (PROJECT_VERSION_MAJOR 24)
set (PROJECT_VERSION_MINOR 3)
set (PROJECT_VERSION_PATCH 5)
set (PROJECT_VERSION_DESC prod)

configure_file (
    "${PROJECT_SOURCE_DIR}/src/environnement.hpp.in"
    "${PROJECT_BINARY_DIR}/environnement.hpp"
)
include_directories("${PROJECT_BINARY_DIR}")


if(APPLE)
    ADD_DEFINITIONS(-DAPPLE)
endif()


find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia SerialPort Network)

find_package(GDAL CONFIG REQUIRED)

set(CMAKE_AUTOMOC ON)

FILE(GLOB_RECURSE src_hpp src/*.hpp)
FILE(GLOB_RECURSE src_cpp src/*.cpp)
FILE(GLOB_RECURSE src_h src/*.h)
FILE(GLOB_RECURSE src_cc src/*.cc)

set(sources ${src_hpp} ${src_cpp} ${src_h} ${src_cc} ${PROJECT_BINARY_DIR}/environnement.hpp src/environnement.hpp.in README.md)

add_executable(lemca_excavator ${sources})

target_link_libraries(lemca_excavator Qt6::Core Qt6::Gui Qt6::Widgets Qt6::SerialPort Qt6::Multimedia Qt6::Network GDAL::GDAL)

###
# grouping files for visual studio or xcode
###

SET(dirlist "")
FOREACH(file_path ${sources})
    GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
    IF(dir_path STREQUAL "")
    ELSE()
        STRING(REPLACE "${PROJECT_SOURCE_DIR}/" "" dir_path2 ${dir_path})
        SET(dir_list ${dir_list} ${dir_path2})
    ENDIF()
ENDFOREACH()
LIST(REMOVE_DUPLICATES dir_list)

foreach(next_ITEM ${dir_list})
    file(GLOB   source_ExecutionProvider_source ${next_ITEM}/*)
    STRING(REPLACE "/" "\\\\" source_group_path ${next_ITEM})
    source_group(${source_group_path} FILES ${source_ExecutionProvider_source})
endforeach(next_ITEM ${MY_SOURCES})

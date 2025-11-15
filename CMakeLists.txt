
cmake_minimum_required(VERSION 3.5)
project(lemca_excavator)

# The version number.
set (PROJECT_VERSION_MAJOR 26)
set (PROJECT_VERSION_MINOR 3)
set (PROJECT_VERSION_PATCH 2)
set (PROJECT_VERSION_DESC test)


option (USE_CPACK "Use CPACK" OFF)
option (USE_ROBOT "Use Robot" ON)

# configure a header file to pass some of the CMake settings to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/src/environnement.hpp.in"
    "${PROJECT_BINARY_DIR}/environnement.hpp"
)
include_directories("${PROJECT_BINARY_DIR}")


# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h

FIND_PACKAGE( OpenCV REQUIRED )
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB_RECURSE    source_files_cpp        src/*.cpp)
file(GLOB_RECURSE    source_files_hpp        src/*.hpp)

set(CMAKE_AUTOMOC ON)
FIND_PACKAGE(Qt5Core)

option (USE_ROBOT "Use Robot" ON)

if(USE_ROBOT)
    ADD_DEFINITIONS(-DUSE_ROBOT)
else()
    REMOVE_DEFINITIONS(-DUSE_ROBOT)
endif()

if(UNIX)
    ADD_DEFINITIONS(-std=c++0x -Wall)
    ADD_DEFINITIONS(-DUNIX)
    ADD_DEFINITIONS(-O3)
    #ADD_DEFINITIONS(-g -Wall)
endif()

if(APPLE)
    ADD_DEFINITIONS(-DAPPLE)
endif()


SET(sources
    ${source_files_hpp} ${source_files_cpp} ${PROJECT_BINARY_DIR}/environnement.hpp
    ${source_files_cfg}
    )

# Declaration de l'executable
add_executable(
    lemca_excavator
    README.md
    src/environnement.hpp.in
    ${sources}
    ${source_files_moc}
    )

qt5_use_modules(lemca_excavator Widgets Multimedia SerialPort )


TARGET_LINK_LIBRARIES( lemca_excavator
    ${OpenCV_LIBS}
    ${QT_LIBRARIES}
    ${QT_QTNETWORK_LIBRARY}
    Threads::Threads

)


###
# grouping files for visual studio or xcode
###

SET(dirlist "")
FOREACH(file_path ${sources})
    GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
    STRING(REPLACE "${PROJECT_SOURCE_DIR}/" "" dir_path2 ${dir_path})
    SET(dir_list ${dir_list} ${dir_path2})
ENDFOREACH()
LIST(REMOVE_DUPLICATES dir_list)

foreach(next_ITEM ${dir_list})
    file(GLOB   source_ExecutionProvider_source ${next_ITEM}/*)
    STRING(REPLACE "/" "\\\\" source_group_path ${next_ITEM})
    source_group(${source_group_path} FILES ${source_ExecutionProvider_source})
endforeach(next_ITEM ${MY_SOURCES})

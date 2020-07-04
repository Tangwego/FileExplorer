cmake_minimum_required(VERSION 3.5)

project(FileExplorer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools Core REQUIRED)

#file(GLOB SC_FILES "*.cpp" "*.h")

set(QRC_SOURCE_FILES icon.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

#list(APPEND SOURCES ${SOURCES_DIR}/res.rc)

set(SRC_FILES
    # source files
    src/main.cpp
    src/FileExplorer.cpp
    src/BaseWindow.cpp
    src/FileUtil.cpp
    src/TreeView.cpp
    src/TreeViewItem.cpp
    src/ListView.cpp
    src/ListViewItem.cpp
    src/PathEdit.cpp
)

set(INCLUDE_FILES
    # header files
    include/FileExplorer.h
    include/BaseWindow.h
    include/Constant.h
    include/FileUtil.h
    include/Log.h
    include/TreeView.h
    include/TreeViewItem.h
    include/ListView.h
    include/ListViewItem.h
    include/PathEdit.h
)

set(RESOURCE_FILES
    # resource files
    res.qrc
)

add_executable(FileExplorer
    ${SRC_FILES}
    ${INCLUDE_FILES}
    ${RESOURCE_FILES}
)

#add_subdirectory(
#    "./icon/"
#)

include_directories(
    "/usr/local/include/"
    "./include"
    "./src"
)

link_directories(
    "/usr/local/lib/"
    "./lib"
)

target_link_libraries(FileExplorer PRIVATE Qt5::Widgets Qt5::Core

)


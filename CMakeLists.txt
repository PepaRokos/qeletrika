cmake_minimum_required(VERSION 3.23)
project(qeletrika)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_MACOSX_RPATH TRUE)
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Sql
        Network
        REQUIRED)

find_package(OpenXLSX REQUIRED)

find_path(GUMBO_INCLUDE_DIR
        NAMES "gumbo.h"
        PATHS "${CMAKE_FRAMEWORK_PATH}/include"
        REQUIRED)
message(STATUS "gumbo include: ${GUMBO_INCLUDE_DIR}")

find_path(GUMBO_QUERY_INCLUDE_DIR
        NAMES "gq/Document.h"
        PATHS "${CMAKE_FRAMEWORK_PATH}/include"
        REQUIRED)
message(STATUS "gumbo-query include: ${GUMBO_QUERY_INCLUDE_DIR}")
include_directories(${GUMBO_QUERY_INCLUDE_DIR} ${GUMBO_INCLUDE_DIR})

find_library(GUMBO_QUERY_LIB
        NAMES "gq"
        REQUIRED)
message(STATUS "gumbo query: ${GUMBO_QUERY_LIB}")

find_library(GUMBO_LIB
        NAMES "gumbo"
        REQUIRED)
message(STATUS "gumbo lib: ${GUMBO_LIB}")

add_executable(qeletrika
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        PricesDownloader.cpp
        PricesDownloader.h
        DataProvider.cpp
        DataProvider.h
        resources.qrc
        data.h
        data.cpp
        CoursesDownloader.cpp
        CoursesDownloader.h
        ReviewLoader.cpp
        ReviewLoader.h
        ItemDelegate.cpp
        ItemDelegate.h)

target_link_libraries(qeletrika
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Sql
        Qt5::Network
        OpenXLSX::OpenXLSX
        ${GUMBO_LIB}
        ${GUMBO_QUERY_LIB}
        )

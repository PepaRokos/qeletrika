cmake_minimum_required(VERSION 3.23)
project(qeletrika)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Sql
        Network
        REQUIRED)

find_package(OpenXLSX REQUIRED)

find_path(GUMBO_QUERY_INCLUDE_DIR
        NAMES "gq/Document.h"
        PATHS "${CMAKE_FRAMEWORK_PATH}/include"
        REQUIRED)
message(STATUS "gumbo-query include: ${GUMBO_QUERY_INCLUDE_DIR}")

find_library(GUMBO_QUERY_DEBUG
        NAMES "gq"
        REQUIRED)
find_library(GUMBO_QUERY_RELEASE
        NAMES "gq"
        REQUIRED)

add_executable(qeletrika main.cpp mainwindow.cpp mainwindow.h mainwindow.ui PricesDownloader.cpp PricesDownloader.h DataProvider.cpp DataProvider.h resources.qrc data.h data.cpp CoursesDownloader.cpp CoursesDownloader.h ReviewLoader.cpp ReviewLoader.h ItemDelegate.cpp ItemDelegate.h)
target_link_libraries(qeletrika
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Sql
        Qt5::Network
        OpenXLSX::OpenXLSX
        gq
        )

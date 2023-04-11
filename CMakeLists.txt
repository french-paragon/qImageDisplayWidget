cmake_minimum_required(VERSION 3.5)

option(buildExamples "Build the examples for the library" ON)

set(LIB_NAME qImageWidget)

include(CheckCXXCompilerFlag)
get_directory_property(hasParent PARENT_DIRECTORY)

project(${LIB_NAME} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Core Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Test REQUIRED)

set(LIB_SRC
    imageadapter.cpp
    imageadapter.h
    imagewidget.cpp
    imagewidget.h
    overlay.cpp
    overlay.h
)

add_library(
${LIB_NAME}
${LIB_SRC}
)

target_link_libraries(${LIB_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set(QIMAGEDISPLAYWIDGET_LIB ${LIB_NAME})
if (hasParent)
    set(QIMAGEDISPLAYWIDGET_LIB ${LIB_NAME} PARENT_SCOPE)
endif()

if (buildExamples)
    add_subdirectory(examples)
endif(buildExamples)

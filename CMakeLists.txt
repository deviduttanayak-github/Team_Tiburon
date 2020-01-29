cmake_minimum_required(VERSION 2.8.3)
project(image_threshing_tool)

add_definitions(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  qt_build
  roscpp
  roslib
  sensor_msgs
  image_transport
  cv_bridge
)

find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
set(QT_MOC include/image_threshing_tool/image_threshing_tool.h)
SET(QT_SOURCES src/image_threshing_tool.cpp)

################################################
## Setup QT
################################################
QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})


###################################
## catkin specific configuration ##
###################################
catkin_package()
include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS}
)

add_library(image_threshing_tool_lib ${QT_SOURCES} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(image_threshing_tool_lib ${QT_LIBRARIES} ${catkin_LIBRARIES} ${OpenCV_LIBS})

add_executable(image_threshing_tool src/main.cpp)
target_link_libraries(image_threshing_tool image_threshing_tool_lib)

cmake_minimum_required(VERSION 2.8.3)
project(behavior_tree_leaves)

find_package(catkin REQUIRED COMPONENTS
  genmsg
  actionlib_msgs
  actionlib
  message_generation
  roscpp
  roslib
  tf
  rospy
  std_msgs
  behavior_tree_core
  roslaunch

)
roslaunch_add_file_check(launch)

find_package(Boost REQUIRED COMPONENTS system thread program_options)

# Settings
add_definitions(-Wall -g -O0 -Wno-deprecated -static -Bstatic -std=gnu++0x)

link_directories(${catkin_LIBRARY_DIRS})

catkin_package(
  CATKIN_DEPENDS actionlib_msgs
  INCLUDE_DIRS include
  LIBRARIES ActionTemplate ConditionTemplate
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

set(COMMON_LIB ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_library(ActionTemplate src/ActionTemplate.cpp)
target_link_libraries(ActionTemplate ${catkin_LIBRARIES})

add_executable(ActionClient src/ActionClient.cpp)

target_link_libraries(
  ActionClient
  ${catkin_LIBRARIES}
)

add_dependencies(
 ActionClient
  ${behavior_tree_leaves_EXPORTED_TARGETS}
  ${PROJECT_NAME}_generate_messages_cpp
  behavior_tree_core_generate_messages_cpp
)

add_executable(ConditionClient src/ConditionClient.cpp)

target_link_libraries(
  ConditionClient
  ${catkin_LIBRARIES}
)

add_dependencies(
 ConditionClient
  ${behavior_tree_leaves_EXPORTED_TARGETS}
  ${PROJECT_NAME}_generate_messages_cpp
  behavior_tree_core_generate_messages_cpp

)

add_library(ConditionTemplate src/ConditionTemplate.cpp)
target_link_libraries(
  ConditionTemplate
  ${catkin_LIBRARIES}
)

add_executable(ActionExample src/ActionExample.cpp src/ActionTemplate.cpp)

target_link_libraries(
  ActionExample
  ${catkin_LIBRARIES}
)

add_dependencies(
 ActionExample
  ${behavior_tree_leaves_EXPORTED_TARGETS}
  ${PROJECT_NAME}_generate_messages_cpp
  behavior_tree_core_generate_messages_cpp
)

add_executable(ConditionExample src/ConditionExample.cpp src/ConditionTemplate.cpp)

target_link_libraries(
  ConditionExample
  ${catkin_LIBRARIES}
)

add_dependencies(
 ConditionExample
  ${behavior_tree_leaves_EXPORTED_TARGETS}
  ${PROJECT_NAME}_generate_messages_cpp
  behavior_tree_core_generate_messages_cpp
)

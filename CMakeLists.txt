
cmake_minimum_required(VERSION 3.5)
project(ros2_synchro_perf)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(std_main std_main.cpp)
ament_target_dependencies(std_main rclcpp)

add_executable(rcl_main rcl_main.cpp)
ament_target_dependencies(rcl_main rclcpp)

add_executable(rclcpp_main rclcpp_main.cpp)
ament_target_dependencies(rclcpp_main rclcpp)

install(TARGETS
  std_main
  rcl_main
  rclcpp_main
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()

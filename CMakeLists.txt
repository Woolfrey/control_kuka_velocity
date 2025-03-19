cmake_minimum_required(VERSION 3.8)
project(kuka_velocity_control)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler options for GCC and Clang
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ROS2 packages
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(serial_link_action_client REQUIRED)
find_package(serial_link_action_server REQUIRED)
find_package(serial_link_interfaces REQUIRED)
find_package(std_msgs REQUIRED)

# Joint command relay
add_executable(joint_command_relay src/joint_command_relay.cpp)

ament_target_dependencies(joint_command_relay "ament_cmake"
                                              "rclcpp"
                                              "serial_link_interfaces"
                                              "std_msgs")                                              

# Installation executable(s)
install(TARGETS joint_command_relay
        DESTINATION lib/${PROJECT_NAME}
)

# Install folders so ROS2 can find relevant files
install(DIRECTORY launch
                  config
                  urdf
        DESTINATION share/${PROJECT_NAME}
)

ament_package()

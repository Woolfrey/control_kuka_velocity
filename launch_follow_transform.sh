#!/bin/bash

# Get install paths using ros2 pkg prefix
PKG_PREFIX=$(ros2 pkg prefix kuka_velocity_control)
CONFIG_DIR="${PKG_PREFIX}/share/kuka_velocity_control/config"

# Open a new terminal and run the action server
gnome-terminal -- bash -c "ros2 launch kuka_velocity_control follow_transform.py; exec bash"

# Wait for the server to start
sleep 1

# Run the client with installed parameter file paths
ros2 run serial_link_action_client follow_transform_client \
  --ros-args \
  --params-file "${CONFIG_DIR}/iiwa_joint_configurations.yaml" \
  --params-file "${CONFIG_DIR}/tolerances.yaml"


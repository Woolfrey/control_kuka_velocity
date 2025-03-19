#!/bin/bash

# Open a new terminal and run the action server
gnome-terminal -- bash -c "ros2 launch kuka_velocity_control trajectory_tracking.py; exec bash"

# Wait a bit to ensure the server is up before starting the client
sleep 3

# Run the client
ros2 run serial_link_action_client trajectory_tracking_client \
  --ros-args \
  --params-file config/iiwa_endpoint_poses.yaml \
  --params-file config/iiwa_joint_configurations.yaml \
  --params-file config/tracking_tolerances.yaml

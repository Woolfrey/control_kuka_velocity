import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    scene = "/home/woolfrey/workspace/mujoco_menagerie/kuka_iiwa_14/scene.xml"
    
    if not os.path.exists(scene):
        raise FileNotFoundError(f"Scene file does not exist: {scene}.")

    directory = get_package_share_directory('mujoco_ros2')  # Gets relative path

    mujoco = Node(
        package="mujoco_ros2",
        executable="mujoco_node",
        output="screen",
        parameters=[
            os.path.join(directory, 'config', 'default_camera.yaml'),
            os.path.join(directory, 'config', 'default_sim.yaml'),
            {'mode': 'VELOCITY'},
            {'xml': scene}
        ]
    )

    return LaunchDescription([mujoco])

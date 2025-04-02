import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    this_directory = get_package_share_directory('kuka_velocity_control')

    # Node: Trajectory Tracking Server
    server = Node(
        package    = 'serial_link_action_server',
        executable = 'follow_transform_server',
        name       = 'follow_transform_server',
        output     = 'screen',
        parameters = [os.path.join(this_directory, 'config/control_parameters.yaml')],
        arguments  = [os.path.join(this_directory, 'urdf/iiwa14.urdf'),                             # URDF location
                     'link7',                                                                       # Endpoint name
                     'joint_command_relay',                                                         # Topic to publish joint commands to
                     'joint_state']                                                                 # Joint state topic to subscribe to
    )

    # Joint command relay
    relay = Node(
        package    = 'kuka_velocity_control',
        executable = 'joint_command_relay',
        name       = 'joint_command_relay',
        output     = 'screen',
        arguments  = ['joint_command_relay',                                                        # Node name
                      'joint_command_relay',                                                        # Subscribe topic
                      'joint_commands']                                                             # Publish topic
    )
    
    # Interactive marker
    interactive = Node(
        package    = 'serial_link_interfaces',
        executable = 'interactive_marker',
        name       = 'interactive_marker',
        output     = 'screen'
    )
    
    # RViz visualization
    rviz = Node(
        package    = 'rviz2',
        executable = 'rviz2',
        name       = 'rviz2',
        output     = 'screen',
        arguments  = ['-d', os.path.join(this_directory, 'rviz/follow_transform.rviz')]
    )
    return LaunchDescription([server, relay, interactive, rviz])


/**
 * @file    joint_command_relay.cpp
 * @author  Jon Woolfrey
 * @email   jonathan.woolfrey@gmail.com
 * @date    February 2025
 * @version 1.0
 * @brief   A node that converts a serial_link_interfaces/msg/JointCommand to a std_msgs/msg/Float64MultiArray
 * 
 * @details This class is designed to relay joint commands for use in another package that is not
 *          dependent on the serial_link_interfaces package. Specifically, it takes a serial_link_interfaces/msg/JointCommand
 *          and then publishes the command field as a std_msgs/msg/Float64MultiArray array. I designed this specifically
 *          to coordinate with my mujoco_ros2 package.
 * 
 * @copyright Copyright (c) 2025 Jon Woolfrey
 * 
 * @license GNU General Public License V3
 * 
 * @see http://github.com/Woolfrey/mujoco_ros2 for how it can be used for control.
 * @see https://docs.ros.org/en/humble/index.html for ROS 2 documentation.
 */

#include "rclcpp/rclcpp.hpp"
#include "serial_link_interfaces/msg/joint_command.hpp"                                             // Custom message
#include "std_msgs/msg/float64_multi_array.hpp"                                                     // For joint_commands topic

/**
 * @brief A class / node for sending joint commands as a std_msgs/msg/Float64MultiArray for use by a robot.
 */
class JointCommandRelay : public rclcpp::Node
{
    public:
        
        // For brevity
        using JointCommandMsg = serial_link_interfaces::msg::JointCommand;
        using Float64MultiArray = std_msgs::msg::Float64MultiArray;
        
        /**
         * @brief Constructor.
         * @nodeName As it says.
         * @subscriptionName Name of the joint command topic.
         * @publicationName Name for new topic to publish to.
         */
        JointCommandRelay(const std::string &nodeName = "joint_command_relay",
                          const std::string &subscriptionName = "joint_command_relay",
                          const std::string &publicationName = "joint_commands")
                          : Node(nodeName)
        {
            _jointCommandSubscriber = this->create_subscription<JointCommandMsg>
            (
                subscriptionName,                                                                   // Name of topic to subscribe to
                1,                                                                                  // No. of msgs to retain
                std::bind(&JointCommandRelay::relay, this, std::placeholders::_1)                   // Callback function for when message is received
            );

            _jointCommandPublisher = this->create_publisher<Float64MultiArray>(publicationName, 1);
            
            RCLCPP_INFO(this->get_logger(),
                        "Created the `%s` node. Subscribing to the `%s` topic. Publishing to the `%s` topic.",
                        nodeName.c_str(), subscriptionName.c_str(), publicationName.c_str());
        }

    private:

        rclcpp::Subscription<serial_link_interfaces::msg::JointCommand>::SharedPtr _jointCommandSubscriber; ///< Subscriber for custom JointCommandMsg
        
        rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr _jointCommandPublisher;      ///< Publisher for array of joint commands
        
        /**
         * @brief This method extracts the joint commands from the custom message and publishes it as an array.
         * @param msg The JointCommandMsg message type.
         */
        void relay(const JointCommandMsg::SharedPtr msg)
        {
            Float64MultiArray relayMsg;                                                             // Create Float64MultiArray on the stack

            relayMsg.data.reserve(msg->command.size());                                             // Reserve memory to avoid multiple allocations if size of command array is known

            relayMsg.data = std::move(msg->command);                                                // Use std::move to move data from the message's command vector if you know you don't need msg->command later

            _jointCommandPublisher->publish(relayMsg);                                              // Publish the message (no shared pointer required)
        }
};

  ////////////////////////////////////////////////////////////////////////////////////////////////////
 //                                           MAIN                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);                                                                       // Start up ROS2
    
    unsigned int returnCode = 0;
    
    if(argc < 3)
    {
        RCLCPP_ERROR(rclcpp::get_logger("main"), "Invalid number of arguments. Usage: <executable> <node_name> <subscription_name> <publication_name>");
        
        returnCode = 1;
    }
    else
    {
        // This is just for clarity:
        std::string nodeName         = argv[1];
        std::string subscriptionName = argv[2];
        std::string publicationName  = argv[3];
        
        rclcpp::spin(std::make_shared<JointCommandRelay>(nodeName, subscriptionName, publicationName)); // Create & run node
    }
    
    rclcpp::shutdown();
    
    return returnCode;
}


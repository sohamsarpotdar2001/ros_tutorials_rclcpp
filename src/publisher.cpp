#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

using namespace std::chrono_literals;

int main(int argc,char* argv[]){
    rclcpp::init(argc,argv);
    
    auto node = std::make_shared<rclcpp::Node>("publisher");
    auto publisher = node->create_publisher<std_msgs::msg::Int32>("int_topic", 10);
    
    std_msgs::msg::Int32 message;
    message.data = 0;

    rclcpp::Rate loop_rate(500ms);
    while (rclcpp::ok()){
        message.data += 1;
        publisher->publish(message);

        rclcpp::spin_some(node);
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}

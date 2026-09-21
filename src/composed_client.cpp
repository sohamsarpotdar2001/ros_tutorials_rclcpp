#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "ros_tutorials_rclcpp/srv/average_vel.hpp"

class ClientNode : public rclcpp::Node
{
public:
    ClientNode(const rclcpp::NodeOptions & options) : Node("minimal_client", options)
    {
        pose_sub_ = create_subscription<turtlesim::msg::Pose>(
                "/turtle1/pose", 
                10, 
                std::bind(&ClientNode::sub_callback, this, std::placeholders::_1));

        client_ = create_client<test_pkg::srv::AverageVel>("get_accel");
        timer_ = create_wall_timer(std::chrono::seconds(1), std::bind(&ClientNode::callback, this));
        request = std::make_shared<test_pkg::srv::AverageVel::Request>();
    }

private:
    void callback()
    {
        if (recorded)
        {
            RCLCPP_INFO(get_logger(), "Got Final Pose");

            request->final_lin_vel = std::round(current_lin_vel*100)/100.0;
            request->final_ang_vel = std::round(current_ang_vel*100)/100.0;
            
            request->time_interval = 1.0;
        
            if (!rclcpp::ok())
            {
                RCLCPP_ERROR(get_logger(), "Interrupted while calling the server");
            }
            else {
                RCLCPP_INFO(get_logger(), "Sending request to the server");
                client_->async_send_request(request,
                        [this](rclcpp::Client<test_pkg::srv::AverageVel>::SharedFuture future)
                        {
                            auto result = future.get();

                            RCLCPP_INFO(get_logger(),
                                    "Linear acceleration %.2f and Angular acceleration %.2f",
                                    result->linear_acceleration,
                                    result->angular_acceleration);
                        }
                );
            }

            recorded = false;
        }     
    }

    void sub_callback(const turtlesim::msg::Pose::SharedPtr msg)
    {
        current_lin_vel = msg->linear_velocity;
        current_ang_vel = msg->angular_velocity;

        if (!recorded) { 
            request->initial_lin_vel = std::round(msg->linear_velocity*100)/100.0;
            request->initial_ang_vel = std::round(msg->angular_velocity*100)/100.0;
            
            RCLCPP_INFO(get_logger(), "Got Initial Pose");

            recorded = true;
        }
    }

    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
    rclcpp::Client<test_pkg::srv::AverageVel>::SharedPtr client_;
    rclcpp::TimerBase::SharedPtr timer_;
    test_pkg::srv::AverageVel::Request::SharedPtr request;
    float current_lin_vel, current_ang_vel;
    bool recorded = false;
};

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(ClientNode)


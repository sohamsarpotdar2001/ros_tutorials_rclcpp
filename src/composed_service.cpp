#include "rclcpp/rclcpp.hpp"
#include "ros_tutorials_rclcpp/srv/average_vel.hpp"

class ServiceNode : public rclcpp::Node
{
public:
    ServiceNode(const rclcpp::NodeOptions options) : Node("minimal_service", options)
    {
        service_ = create_service<test_pkg::srv::AverageVel>("get_accel", 
                [this](std::shared_ptr<test_pkg::srv::AverageVel::Request> request,
                        std::shared_ptr<test_pkg::srv::AverageVel::Response> response)
                {
                    RCLCPP_INFO(get_logger(), "Calculating acceleration");
                    response->linear_acceleration = (request->final_lin_vel - request->initial_lin_vel) / request->time_interval;
                    response->angular_acceleration = (request->final_ang_vel - request->initial_ang_vel) / request->time_interval;
                }
        );
    }

private:
    rclcpp::Service<test_pkg::srv::AverageVel>::SharedPtr service_;
};

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(ServiceNode)

#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"

class Spawn_service : public rclcpp::Node
{
public:
    Spawn_service() : Node("spawn_service")
    {
        spawn_client_ = create_client<turtlesim::srv::Spawn>("/spawn");
    }

    auto send_request(float x, float y, float theta)
    {
        auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
        request->x = x;
        request->y = y;
        request->theta = theta;

        while (!spawn_client_->wait_for_service(std::chrono::seconds(1)))
        {
            if (!rclcpp::ok()) {
                RCLCPP_ERROR(get_logger(), "Interrupted while waiting for service");
            }
            RCLCPP_INFO(get_logger(), "Service not available, waiting again...");
        }
        auto result = spawn_client_->async_send_request(request);
        
        return result;
    }

private:
    rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr spawn_client_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc,argv);

    auto node = std::make_shared<Spawn_service>();

    float x = std::stof(argv[1]);
    float y = std::stof(argv[2]);
    float theta = std::stof(argv[3]);
    auto response = node->send_request(x, y, theta);

    if (rclcpp::spin_until_future_complete(node, response) == rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), 
                "Spawned a new turtle with name %s", 
                response.get()->name.c_str());
    }
    else {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), 
                "Failed to get result after calling spawn service");
    }

    rclcpp::shutdown();
    return 0;
}

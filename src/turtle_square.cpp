#include <atomic>
#include <vector>
#include "signal.h"
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "turtlesim/action/rotate_absolute.hpp"

std::atomic<bool> running = true;
using namespace std::chrono_literals;

class TurtlesimSquare : public rclcpp::Node
{
public:
    TurtlesimSquare() : Node("turtle_square")
    {
        publisher = create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        timer = create_wall_timer(std::chrono::milliseconds(500), 
                std::bind(&TurtlesimSquare::callback, this));
        rotate_absolute_client = rclcpp_action::create_client<turtlesim::action::RotateAbsolute>(this, 
                "/turtle1/rotate_absolute");
    }

    void callback()
    {
        if (running && moved){
            msg.linear.x = 2.0;
            try {
                publisher->publish(msg);
                RCLCPP_INFO(this->get_logger(), "Going forward");
                moved = true;
            }
            catch (...) {
                RCLCPP_INFO(this->get_logger(), "Failed going forward");
                moved = false;
            }
        }
        rclcpp::sleep_for(std::chrono::seconds(2));

        if (running && moved){
            switch(counter) {
                case 0:
                    RCLCPP_INFO(this->get_logger(), "Left turn");
                    sendGoal(1.5708f);
                    break;
                case 1:
                    RCLCPP_INFO(this->get_logger(), "Left turn");
                    sendGoal(3.1416f);
                    break;
                case 2:
                    RCLCPP_INFO(this->get_logger(), "Left turn");
                    sendGoal(-1.5708f);
                    break;
                case 3:
                    RCLCPP_INFO(this->get_logger(), "Left turn");
                    sendGoal(0.0f);
                    break;
                default:
                    moved = false;
                    break;
            }
            counter += 1;
            if (counter > 3) { counter = 0; }
        }
        rclcpp::sleep_for(std::chrono::seconds(2));
    }

private:
    std::atomic<bool> moved = true;
    const float linear = 2.0;
    std::vector<float>angles = {0.0, 1.5708, 3.1416, -1.5708};
    int counter = 0;
    geometry_msgs::msg::Twist msg;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher;
    rclcpp::TimerBase::SharedPtr timer;
    rclcpp_action::Client<turtlesim::action::RotateAbsolute>::SharedPtr rotate_absolute_client;
    rclcpp_action::ClientGoalHandle<turtlesim::action::RotateAbsolute>::SharedPtr goal_handle_;
    
    void sendGoal(float theta){
        auto goal = turtlesim::action::RotateAbsolute::Goal();
        goal.theta = theta;
        auto send_goal_options = rclcpp_action::Client<turtlesim::action::RotateAbsolute>::SendGoalOptions();
        send_goal_options.goal_response_callback = [this](rclcpp_action::ClientGoalHandle<turtlesim::action::RotateAbsolute>::SharedPtr goal_handle)
        {
            RCLCPP_INFO(this->get_logger(), "Turn goal recieved");
            this->goal_handle_ = goal_handle;
        };
        rotate_absolute_client->async_send_goal(goal, send_goal_options); 
        moved = true;
    }    

    void cancelGoal()
    {
        if (goal_handle_){
            RCLCPP_INFO(this->get_logger(), "Sending cancel goal request");
            try {
                rotate_absolute_client->async_cancel_goal(goal_handle_);
                moved = false;
            }
            catch(...){
            
            }
        }
    }
};

void quit(int sig)
{
    (void)sig;
    running = false;
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc,argv);

    auto node = std::make_shared<TurtlesimSquare>();

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}

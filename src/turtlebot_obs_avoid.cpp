#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

class ObstacleAvoidance : public rclcpp::Node
{
public:
    ObstacleAvoidance() : Node("turtlebot_obs_avoid")
    {
        timer_cb_grp_ = create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        sub_options_.callback_group = nullptr;
        publisher_ = create_publisher<geometry_msgs::msg::TwistStamped>(
                "/cmd_vel", 
                10);
        
        lidar_sub_ = create_subscription<sensor_msgs::msg::LaserScan>(
                "/scan", 
                10, 
                std::bind(&ObstacleAvoidance::scan_callback, 
                    this, 
                    std::placeholders::_1), 
                sub_options_);

        timer_ = create_wall_timer(
                std::chrono::milliseconds(100), 
                std::bind(&ObstacleAvoidance::movebot, this),
                timer_cb_grp_);
    }
    
    /*
    void callback(const sensor_msgs::msg::LaserScan::SharedPtr msg, const geometry_msgs::msg::TwistStamped::SharedPtr pub_msg)
    {
        ranges = msg->ranges;
        unsigned int i = 0;
        while (i != ranges.size())
        {
            RCLCPP_INFO(get_logger(), "value=%.3f counter=%d", ranges[i], i);
            i += 1;
        }
        RCLCPP_INFO(get_logger(), "End of scan. Got %ld values", ranges.size());
    }
    */

    void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        ranges = msg->ranges;
        for (int i = 0,j = 10; i < 2 && j < 12; i++,j++)
        {
            float sum1 = 0.0, sum2 = 0.0;
            for (int k = i*30,l = j*30; 
                    k < (i+1)*30 && l < (j+1)*30; 
                    k++,l++)
            {
                sum1 += std::round(ranges[k]*100)/100.0;
                sum2 += std::round(ranges[l]*100)/100.0;
            }           
            avg_scan[i] = sum1 / 30.0;
            avg_scan[j-8] = sum2 / 30.0;
        }
        RCLCPP_INFO(get_logger(), "Scan Updated");
    }

    void movebot()
    {
        geometry_msgs::msg::TwistStamped msg;
        
        float left  = (avg_scan[0] + avg_scan[1]) / 2.0;
        float right = (avg_scan[2] + avg_scan[3]) / 2.0;
        float front = (avg_scan[0] + avg_scan[3]) / 2.0;

        const double obstacle_dist = 1.0;

        if (front < obstacle_dist)
        {
            msg.twist.linear.x = 0.0;

            if (left > right)
                msg.twist.angular.z = 1.5;
            else
                msg.twist.angular.z = -1.5;
        }
        else
        {
            msg.twist.linear.x = 0.2;

            if (left < obstacle_dist)
            {
                msg.twist.angular.z = -1.5;
            }
            else if (right < obstacle_dist)
            {
                msg.twist.angular.z = 1.5;
            }
            else
            {
                msg.twist.angular.z = 0.0;
            }
        }
        publisher_->publish(msg);
        RCLCPP_INFO(get_logger(), "Sending velocity commands");
    }

private:
    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr publisher_;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr lidar_sub_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::SubscriptionOptions sub_options_;
    rclcpp::CallbackGroup::SharedPtr timer_cb_grp_;
    std::vector<float> ranges;
    std::array<float, 4> avg_scan = {0,0,0,0};
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ObstacleAvoidance>();

    rclcpp::executors::MultiThreadedExecutor executor;    
    executor.add_node(node);

    executor.spin();
    
    rclcpp::shutdown();
    return 0;
}

#include <string>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/publisher.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "std_msgs/msg/string.hpp"

class LifecycleTalker : public rclcpp_lifecycle::LifecycleNode
{
public:
    explicit LifecycleTalker(const std::string& node_name, bool intra_process_comms=false) : 
        rclcpp_lifecycle::LifecycleNode(node_name, 
                rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms))
    {}

    void publish()
    {
        static size_t count = 0;
        auto msg = std::make_unique<std_msgs::msg::String>();
        msg->data = "Lifecycle HelloWorld #" + std::to_string(++count);

        if (!pub_->is_activated())
        {
            RCLCPP_INFO(get_logger(), "Lifecycle Publisher currently inactive");
        } else {
            RCLCPP_INFO(get_logger(), 
                    "Lifecycle Publisher is active. Publishing: [%s]", msg->data.c_str());

        }

        pub_->publish(std::move(msg));
    }

    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn 
    on_configure(const rclcpp_lifecycle::State &)
    {
        pub_ = this->create_publisher<std_msgs::msg::String>("lifecycle_chatter", 10);
        timer_ = this->create_wall_timer(std::chrono::seconds(1), 
                [this]() { return this->publish(); });

        RCLCPP_INFO(get_logger(), "on_configure() is called");

        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn 
    on_activate(const rclcpp_lifecycle::State& state)
    {
        LifecycleNode::on_activate(state);
        
        RCLCPP_INFO(get_logger(), "on_activate() is called");

        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn 
    on_deactivate(const rclcpp_lifecycle::State& state)
    {
        LifecycleNode::on_deactivate(state);

        RCLCPP_INFO(get_logger(), "on_deactivate is called");

        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn 
    on_cleanup(const rclcpp_lifecycle::State &)
    {
        timer_.reset();
        pub_.reset();

        RCLCPP_INFO(get_logger(), "on_cleanup is called");

        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn 
    on_shutdown(const rclcpp_lifecycle::State &state)
    {
        timer_.reset();
        pub_.reset();

        RCLCPP_INFO(get_logger(), "on_shutdown is called from state %s", state.label().c_str());

        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

private:
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>> pub_;
    std::shared_ptr<rclcpp::TimerBase> timer_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<LifecycleTalker>("lc_talker");

    rclcpp::executors::SingleThreadedExecutor exe;
    exe.add_node(node->get_node_base_interface());

    exe.spin();

    rclcpp::shutdown();
    return 0;
}


#include <functional>
#include <memory>
#include <string>

#include "lifecycle_msgs/msg/transition_event.hpp"

#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/string.hpp"

class LifecycleListener : public rclcpp::Node
{
public:
  explicit LifecycleListener(const std::string & node_name)
  : Node(node_name)
  {
    sub_data_ = this->create_subscription<std_msgs::msg::String>(
      "lifecycle_chatter", 10,
      [this](std_msgs::msg::String::ConstSharedPtr msg) {return this->data_callback(msg);});

    sub_notification_ = this->create_subscription<lifecycle_msgs::msg::TransitionEvent>(
      "/lc_talker/transition_event",
      10,
      [this](lifecycle_msgs::msg::TransitionEvent::ConstSharedPtr msg) {
        return this->notification_callback(msg);
      }
    );
  }

  void data_callback(std_msgs::msg::String::ConstSharedPtr msg)
  {
    RCLCPP_INFO(get_logger(), "data_callback: %s", msg->data.c_str());
  }

  void notification_callback(lifecycle_msgs::msg::TransitionEvent::ConstSharedPtr msg)
  {
    RCLCPP_INFO(
      get_logger(), "notify callback: Transition from state %s to %s",
      msg->start_state.label.c_str(), msg->goal_state.label.c_str());
  }

private:
  std::shared_ptr<rclcpp::Subscription<std_msgs::msg::String>> sub_data_;
  std::shared_ptr<rclcpp::Subscription<lifecycle_msgs::msg::TransitionEvent>>
  sub_notification_;
};

int main(int argc, char ** argv)
{
  // force flush of the stdout buffer.
  // this ensures a correct sync of all prints
  // even when executed simultaneously within the launch file.
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  rclcpp::init(argc, argv);

  auto lc_listener = std::make_shared<LifecycleListener>("lc_listener");
  rclcpp::spin(lc_listener);

  rclcpp::shutdown();

  return 0;
}

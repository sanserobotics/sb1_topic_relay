#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
//#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"

//std::string topic_in="/diffbot_base_controller/cmd_vel_unstamped";
//std::string topic_out="/cmd_vel_test";

std::string topic_out = "/diffbot_base_controller/cmd_vel_unstamped";
std::string topic_in = "/cmd_vel";

class TopicRelay : public rclcpp::Node
{
  public:
    TopicRelay()
    : Node("topic_relay")
    {
      publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(topic_out, 10);
      subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
      topic_in, 10, std::bind(&TopicRelay::topic_callback, this, std::placeholders::_1));
    }

  private:
    void topic_callback(const geometry_msgs::msg::Twist::SharedPtr msg) const
    {
      auto message = geometry_msgs::msg::Twist();
      message.linear = msg->linear;
      message.angular = msg->angular;
      publisher_->publish(message);
    }
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TopicRelay>());
  rclcpp::shutdown();
  return 0;
}

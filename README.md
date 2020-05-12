# ros2-synchronization-test
Performance evaluation of different synchronization approaches in ROS 2


Results for a RaspberryPi 3 board:

 - `std::condition_variable` : 1.8% CPU usage
 - `rcl_guard_condition` : 2.9% CPU usage
 - `rclcpp::GuardCondition` : 2.9% CPU usage




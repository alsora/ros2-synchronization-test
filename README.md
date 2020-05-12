# ros2-synchronization-test
Performance evaluation of different synchronization approaches in ROS 2

This repository contains 3 dummy applications that perform the following task: spawn a producer and a consumer thread and let the producer push data into a buffer while the consumer waits for new data and reads it when available.

Data is pused into the queue at 100Hz frequency.

The difference between the applications is only how the synchronization primitives are implemtend.

Results for a RaspberryPi 3 board:

 - `std::condition_variable` : 1.8% CPU usage
 - `rcl_guard_condition` + `rcl_waitset` : 2.9% CPU usage
 - `rclcpp::GuardCondition` + `rclcpp::Waiset` : 2.9% CPU usage




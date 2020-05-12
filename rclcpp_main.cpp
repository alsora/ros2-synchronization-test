#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <chrono>

#include <rclcpp/rclcpp.hpp>

int main()
{
    std::queue<int> produced_nums;
    std::mutex m;
    std::condition_variable cond_var;
    bool done = false;

    rclcpp::init(0, nullptr);

    auto gc = std::make_shared<rclcpp::GuardCondition>();

    rclcpp::StaticWaitSet<0, 1, 0, 0, 0, 0> wait_set(
        std::array<rclcpp::StaticWaitSet<0, 1, 0, 0, 0, 0>::SubscriptionEntry, 0>{},
        std::array<rclcpp::GuardCondition::SharedPtr, 1>{{gc}},
        std::array<rclcpp::TimerBase::SharedPtr, 0>{},
        std::array<rclcpp::ClientBase::SharedPtr, 0>{},
        std::array<rclcpp::ServiceBase::SharedPtr, 0>{},
        std::array<rclcpp::StaticWaitSet<0, 1, 0, 0, 0, 0>::WaitableEntry, 0>{});

    const int num_samples = 10000;
    std::thread producer([&]() {
        for (int i = 0; i < num_samples; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            {
                std::unique_lock<std::mutex> lock(m);
                //std::cout << "producing " << i << '\n';
                produced_nums.push(i);
            }
            gc->trigger();
        }

        done = true;
        gc->trigger();
    });

    std::thread consumer([&]() {
        while (!done) {
            // Wait for a notification
            auto wait_result = wait_set.wait();
            std::unique_lock<std::mutex> lock(m);
            while (!produced_nums.empty()) {
                //std::cout << "consuming " << produced_nums.front() << '\n';
                produced_nums.pop();
            }
        }
    });

    producer.join();
    consumer.join();
}
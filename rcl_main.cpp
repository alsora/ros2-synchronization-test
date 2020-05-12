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

    rcl_guard_condition_options_t guard_condition_options =
      rcl_guard_condition_get_default_options();

    auto context = rclcpp::contexts::get_global_default_context();

    rcl_guard_condition_t gc = rcl_get_zero_initialized_guard_condition();
    rcl_ret_t ret = rcl_guard_condition_init(&gc, context->get_rcl_context().get(), guard_condition_options);

    rcl_wait_set_t wait_set = rcl_get_zero_initialized_wait_set();
    ret = rcl_wait_set_init(&wait_set, 0, 1, 0, 0, 0, 0, context->get_rcl_context().get(), rcl_get_default_allocator());
    (void)ret;

    const int num_samples = 10000;
    std::thread producer([&]() {
        rcl_ret_t ret;
        for (int i = 0; i < num_samples; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            {
                std::unique_lock<std::mutex> lock(m);
                //std::cout << "producing " << i << '\n';
                produced_nums.push(i);
            }
            ret = rcl_trigger_guard_condition(&gc);
        }

        done = true;
        ret = rcl_trigger_guard_condition(&gc);
        (void)ret;
    });

    std::thread consumer([&]() {
        rcl_ret_t ret;
        while (!done) {
            // Wait for a notification
            ret = rcl_wait_set_clear(&wait_set);
            ret = rcl_wait_set_add_guard_condition(&wait_set, &gc, NULL);
            ret = rcl_wait(&wait_set, -1);

            std::unique_lock<std::mutex> lock(m);
            while (!produced_nums.empty()) {
                //std::cout << "consuming " << produced_nums.front() << '\n';
                produced_nums.pop();
            }
        }
        ret = rcl_wait_set_fini(&wait_set);
        (void)ret;
    });

    producer.join();
    consumer.join();
}
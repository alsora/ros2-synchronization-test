#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <chrono>

int main()
{
    std::queue<int> produced_nums;
    std::mutex m;
    std::condition_variable cond_var;
    bool done = false;

    const int num_samples = 10000;
    std::thread producer([&]() {
        for (int i = 0; i < num_samples; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            {
                std::unique_lock<std::mutex> lock(m);
                //std::cout << "producing " << i << '\n';
                produced_nums.push(i);
            }
            cond_var.notify_one();
        }

        done = true;
        cond_var.notify_one();
    });

    std::thread consumer([&]() {
        std::unique_lock<std::mutex> lock(m);
        while (!done) {
            cond_var.wait(lock);
            while (!produced_nums.empty()) {
                //std::cout << "consuming " << produced_nums.front() << '\n';
                produced_nums.pop();
            }
        }
    });

    producer.join();
    consumer.join();
}
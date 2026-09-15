#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

template <typename T>
class ThreadSafeQueue
{
public:
    void push(T value)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(value));
        }

        condition_.notify_one();
    }

    bool waitPopFor(
        T& value,
        std::chrono::milliseconds timeout)
    {
        std::unique_lock<std::mutex> lock(mutex_);

        if (!condition_.wait_for(
                lock,
                timeout,
                [this] { return !queue_.empty(); }))
        {
            return false;
        }

        value = std::move(queue_.front());
        queue_.pop();

        return true;
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
};
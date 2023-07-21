#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>
#include <vector>

namespace raytracing {

class ThreadPool {
   private:
    std::queue<std::function<void()>> m_Tasks;
    std::condition_variable_any m_NotifyTask;
    std::shared_mutex m_TasksMutex;

    std::vector<std::thread> m_Threads;
    std::atomic<int> m_ActiveThreads;

    std::atomic<bool> m_ClearSignal;
    std::atomic<bool> m_StopSignal;
    std::atomic<bool> m_QuickExitSignal;

   public:
    ThreadPool(uint64_t count);
    ~ThreadPool();

    uint64_t Count() const { return m_Threads.size(); }
    void Resize(uint64_t count);

    // Atleast 1 thread is executing a task
    bool IsActive() const { return m_ActiveThreads != 0; };

    // Clear the queue and exit current tasks early if possible
    void Clear();

    template <class Function, class... Args>
    void AddTask(Function&& func, Args&&... args) {
        auto task = std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
        {
            std::unique_lock<std::shared_mutex> lock(m_TasksMutex);
            m_Tasks.emplace([task]() -> void { task(); });
        }

        m_NotifyTask.notify_one();
    }

    // QS -> Quick Exit
    // Passes a quick exit flag (atomic_bool&) as the last argument to the task function
    // The threadpool will use it to signal when the function should exit early
    template <class Function, class... Args>
    void AddTaskQE(Function&& func, Args&&... args) {
        m_QuickExitSignal = false;
        auto task = std::bind(std::forward<Function>(func), std::forward<Args>(args)..., std::ref(m_QuickExitSignal));
        {
            std::unique_lock<std::shared_mutex> lock(m_TasksMutex);
            m_Tasks.emplace([task]() -> void { task(); });
        }
        m_NotifyTask.notify_one();
    }

   private:
    void ThreadExecution();

    void Init(uint64_t count);
    void Stop();
};

}  // namespace raytracing
#include "Thread.h"

namespace raytracing {

uint32_t ThreadPool::s_MaxThreadCount = std::thread::hardware_concurrency();

ThreadPool::ThreadPool(uint64_t count) {
    Init(count);
}

ThreadPool::~ThreadPool() {
    Stop();
}

void ThreadPool::Init(uint64_t count) {
    RT_PROFILE_FUNC;
    if (count == 0) {
        RT_WARN("Creating threadpool with no threads: {}", count);
    }

    m_ClearSignal = false;
    m_StopSignal = false;
    m_QuickExitSignal = false;
    m_WaitingSignal = false;

    m_ActiveThreads = 0;
    m_Threads.reserve(count);
    for (uint64_t c = 0; c < count; c++) {
        m_Threads.emplace_back(std::bind(&ThreadPool::ThreadExecution, this, c));
    }
}

void ThreadPool::ThreadExecution(uint64_t index) {
    std::string name = "ThreadPool " + std::to_string(index);
    RT_PROFILE_THREAD_N(name.c_str());
    RT_PROFILE_FUNC;

    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::shared_mutex> lock(m_TasksMutex);

            m_NotifyTask.wait(lock, [this, &task] {
                return m_StopSignal.load() || !m_Tasks.empty();
            });

            if (m_StopSignal && m_Tasks.empty()) {
                break;
            }

            task = std::move(m_Tasks.front());
            m_Tasks.pop();
        }

        m_ActiveThreads++;
        task();
        m_ActiveThreads--;

        {
            std::shared_lock<std::shared_mutex> lock(m_TasksMutex);

            if (m_WaitingSignal && (m_ActiveThreads == 0) && m_Tasks.empty()) {
                m_DoneTasks.notify_all();
            }
        }
    }
}

void ThreadPool::Stop() {
    RT_PROFILE_FUNC;
    m_QuickExitSignal = true;
    m_StopSignal = true;
    m_NotifyTask.notify_all();
    for (auto& thread : m_Threads) {
        thread.join();
    }
    m_Threads.clear();
}

void ThreadPool::Resize(uint64_t count) {
    RT_PROFILE_FUNC;
    if (m_Threads.size() != count) {
        // TODO: Optimize!
        Stop();
        Init(count);
    }
}

bool ThreadPool::IsActive() {
    RT_PROFILE_FUNC;
    std::shared_lock<std::shared_mutex> lock(m_TasksMutex);
    return (m_ActiveThreads != 0) || !m_Tasks.empty();
}

void ThreadPool::Clear() {
    RT_PROFILE_FUNC;
    m_QuickExitSignal = true;

    {
        std::unique_lock<std::shared_mutex> lock(m_TasksMutex);
        while (!m_Tasks.empty()) {
            m_Tasks.pop();
        }
        m_NotifyTask.notify_all();
    }

    // Note: It is not guaranteed that the threads have all stopped working on tasks by the time this returns
    //       Use IsActive() or WaitForTasks().
}

void ThreadPool::WaitForTasks() {
    RT_PROFILE_FUNC;
    std::shared_lock<std::shared_mutex> lock(m_TasksMutex);
    m_WaitingSignal = true;
    m_DoneTasks.wait(lock, [this] {
        return (m_ActiveThreads == 0) && m_Tasks.empty();
    });
    m_WaitingSignal = false;
}

}  // namespace raytracing
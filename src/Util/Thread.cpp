#include "Thread.h"

namespace raytracing {

ThreadPool::ThreadPool(uint64_t count) {
    Init(count);
}

ThreadPool::~ThreadPool() {
    Stop();
}

void ThreadPool::Init(uint64_t count) {
    m_ClearSignal = false;
    m_StopSignal = false;
    m_QuickExitSignal = false;

    m_ActiveThreads = 0;
    m_Threads.reserve(count);
    for (uint64_t c = 0; c < count; c++) {
        m_Threads.emplace_back(std::bind(&ThreadPool::ThreadExecution, this));
    }
}

void ThreadPool::ThreadExecution() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::shared_mutex> lock(m_TasksMutex);

            m_NotifyTask.wait(lock, [this, &task] {
                return m_StopSignal.load() || !m_Tasks.empty();
            });

            if (m_StopSignal && m_Tasks.empty()) {
                return;
            }

            task = std::move(m_Tasks.front());
            m_Tasks.pop();
        }

        m_ActiveThreads++;
        task();
        m_ActiveThreads--;
    }
}

void ThreadPool::Stop() {
    m_QuickExitSignal = true;
    m_StopSignal = true;
    m_NotifyTask.notify_all();
    for (auto& thread : m_Threads) {
        thread.join();
    }
    m_Threads.clear();
}

void ThreadPool::Resize(uint64_t count) {
    if (m_Threads.size() != count) {
        // TODO: Optimize!
        Stop();
        Init(count);
    }
}

void ThreadPool::Clear() {
    m_QuickExitSignal = true;

    {
        std::unique_lock<std::shared_mutex> lock(m_TasksMutex);
        while (!m_Tasks.empty()) {
            m_Tasks.pop();
        }
        m_NotifyTask.notify_all();
    }

    // Note: It is not guaranteed that the threads have all stopped working on tasks by the time this returns
    //       Use IsRunning() to check.
}

}  // namespace raytracing
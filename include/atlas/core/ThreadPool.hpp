/**
 *	\file ThreadPool.hpp
 *	\brief Defines a simple thread pool using C++11 threads.
 *	
 */

#ifndef ATLAS_INCLUDE_ATLAS_CORE_THREAD_POOL_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_THREAD_POOL_HPP

#pragma once

#include "Core.hpp"
#include "Exception.hpp"
#include "Queue.hpp"

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace atlas
{
    namespace core
    {
        class ThreadPool
        {
        private:
            class ThreadTask
            {
            public:
                ThreadTask() = default;
                ThreadTask(ThreadTask const& rhs) = delete;
                ThreadTask(ThreadTask&& rhs) = default;
                virtual ~ThreadTask() = default;

                ThreadTask& operator=(ThreadTask const& rhs) = delete;
                ThreadTask& operator=(ThreadTask&& rhs) = default;

                virtual void execute() = 0;
            };

            template <typename FnType>
            class Task : public ThreadTask
            {
            public:
                Task(FnType&& fn) :
                    mFn(std::move(fn))
                { }

                Task(Task const& rhs) = delete;
                Task(Task&& rhs) = default;
                virtual ~Task() = default;

                Task& operator=(Task const& rhs) = delete;
                Task& operator=(Task&& rhs) = default;

                void execute() override
                {
                    mFn();
                }

            private:
                FnType mFn;
            };

        public:
            template <typename GenType>
            class TaskFuture
            {
            public:
                TaskFuture(std::future<GenType>&& future) :
                    mFuture(std::move(future))
                { }

                TaskFuture(TaskFuture const& rhs) = delete;
                TaskFuture(TaskFuture&& rhs) = default;
                ~TaskFuture()
                {
                    if (mFuture.valid())
                    {
                        //mFuture.get();
                    }
                }

                TaskFuture& operator=(TaskFuture const& rhs) = delete;
                TaskFuture& operator=(TaskFuture&& rhs) = default;

                auto get()
                {
                    return mFuture.get();
                }

            private:
                std::future<GenType> mFuture;
            };

        public:
            ThreadPool() :
                ThreadPool{ std::max(std::thread::hardware_concurrency(), 2u) - 1u }
            { }

            explicit ThreadPool(const std::uint32_t numThreads) :
                mDone(false),
                mRunningTasks{},
                mWorkQueue{},
                mThreads{}
            {
                try
                {
                    for (std::uint32_t i = 0u; i < numThreads; ++i)
                    {
                        mThreads.emplace_back(&ThreadPool::worker, this, i);
                        mRunningTasks.emplace_back(false);
                    }
                }
                catch (...)
                {
                    destroy();
                    throw;
                }
            }

            ThreadPool(ThreadPool const& rhs) = delete;
            ~ThreadPool()
            {
                destroy();
            }

            ThreadPool& operator=(const ThreadPool& rhs) = delete;

            template <typename FnType, typename... Args>
            auto submit(FnType&& fn, Args&&... args)
            {
                auto boundTask = std::bind(std::forward<FnType>(fn),
                    std::forward<Args>(args)...);

                using ResType = std::result_of_t<decltype(boundTask)()>;
                using PackagedTask = std::packaged_task<ResType()>;
                using TaskType = Task<PackagedTask>;

                PackagedTask task{ std::move(boundTask) };
                TaskFuture<ResType> result{ task.get_future() };
                mWorkQueue.push(std::make_unique<TaskType>(std::move(task)));
                return result;
            }

            void waitOnTasks()
            {
                for (;;)
                {
                    bool running = false;
                    for (auto const& task : mRunningTasks)
                    {
                        if (task)
                        {
                            running = true;
                            break;
                        }
                    }

                    if (!running)
                    {
                        break;
                    }
                }
            }

            private:
                void worker(std::uint32_t id)
                {
                    while (!mDone)
                    {
                        std::unique_ptr<ThreadTask> task;
                        if (mWorkQueue.waitPop(task))
                        {
                            mRunningTasks[id] = true;
                            task->execute();
                            mRunningTasks[id] = false;
                        }
                    }
                }

                void destroy()
                {
                    mDone = true;
                    mWorkQueue.invalidate();
                    for (auto& thread : mThreads)
                    {
                        if (thread.joinable())
                        {
                            thread.join();
                        }
                    }
                }

        private:
            std::atomic_bool mDone;
            std::vector<bool> mRunningTasks;
            Queue<std::unique_ptr<ThreadTask>> mWorkQueue;
            std::vector<std::thread> mThreads;
        };

        namespace global
        {
            template <typename FnType, typename... Args>
            inline auto submitJob(FnType&& fn, Args&&... args)
            {
                return getThreadPoolInstance().submit(std::forward<FnType>(fn),
                    std::forward<Args>(args)...);
            }

            inline ThreadPool& getThreadPoolInstance()
            {
                static ThreadPool globalPool;
                return globalPool;
            }
        }
    }
}

#endif
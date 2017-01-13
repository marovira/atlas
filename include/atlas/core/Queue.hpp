#ifndef ATLAS_INCLUDE_ATLAS_CORE_QUEUE_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_QUEUE_HPP

#pragma once

#include "Core.hpp"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

namespace atlas
{
    namespace core
    {
        template <typename GenType>
        class Queue
        {
        public:
            ~Queue()
            {
                invalidate();
            }

            bool tryPop(GenType& out)
            {
                std::lock_guard<std::mutex> lock{ mMutex };
                if (mQueue.empty() || !mValid)
                {
                    return false;
                }

                out = std::move(mQueue.front());
                mQueue.pop();
                return true;
            }

            bool waitPop(GenType& out)
            {
                std::unique_lock<std::mutex> lock{ mMutex };
                mCondition.wait(lock, 
                    [this]()
                    {
                    return !mQueue.empty() || !mValid;
                    });

                if (!mValid)
                {
                    return false;
                }

                out = std::move(mQueue.front());
                mQueue.pop();
                return true;
            }

            void push(GenType value)
            {
                std::lock_guard<std::mutex> lock{ mMutex };
                mQueue.push(std::move(value));
                mCondition.notify_one();
            }

            bool empty() const
            {
                std::lock_guard<std::mutex> lock{ mMutex };
                return mQueue.empty();
            }

            void clear()
            {
                std::lock_guard<std::mutex> lock{ mMutex };
                while (!mQueue.empty())
                {
                    mQueue.pop();
                }
                mCondition.notify_all();
            }

            void invalidate()
            {
                std::lock_guard<std::mutex> lock{ mMutex };
                mValid = false;
                mCondition.notify_all();
            }

            bool isValid() const
            {
                std::lock_guard<std::mutex> lock{ mMutex };
                return mValid;
            }

        private:
            std::atomic_bool mValid{ true };
            mutable std::mutex mMutex;
            std::queue<GenType> mQueue;
            std::condition_variable mCondition;
            
        };
    }
}

#endif
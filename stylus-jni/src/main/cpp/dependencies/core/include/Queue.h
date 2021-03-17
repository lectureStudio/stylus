/*
 * Copyright 2016 Alex Andres
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef STYLUS_QUEUE_H_
#define STYLUS_QUEUE_H_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class Queue
{
	public:
		Queue() {}

		T pop() 
		{
			std::unique_lock<std::mutex> mlock(mutex);

			while (queue.empty()) {
				cond.wait(mlock);
			}

			auto val = queue.front();
			queue.pop();
			return val;
		}

		void pop(T & item)
		{
			std::unique_lock<std::mutex> mlock(mutex);

			while (queue.empty()) {
				cond.wait(mlock);
			}

			item = queue.front();
			queue.pop();
		}

		void push(const T& item)
		{
			std::unique_lock<std::mutex> mlock(mutex);
			queue.push(item);
			mlock.unlock();
			cond.notify_one();
		}

		void clear()
		{
			std::unique_lock<std::mutex> mlock(mutex);

			while (!queue.empty()) {
				queue.pop();
			}
		}

	private:
		Queue(const Queue &);            // disable copying
		Queue& operator=(const Queue &); // disable assignment

		std::queue<T> queue;
		std::mutex mutex;
		std::condition_variable cond;
};

#endif

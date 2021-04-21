//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parallel/task_scheduler.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/mutex.hpp"
#include "guinsoodb/common/vector.hpp"
#include "guinsoodb/parallel/task.hpp"

namespace guinsoodb {

struct ConcurrentQueue;
struct QueueProducerToken;
class ClientContext;
class TaskScheduler;

struct SchedulerThread;

struct ProducerToken {
	ProducerToken(TaskScheduler &scheduler, unique_ptr<QueueProducerToken> token);
	~ProducerToken();

	TaskScheduler &scheduler;
	unique_ptr<QueueProducerToken> token;
	std::mutex producer_lock;
};

//! The TaskScheduler is responsible for managing tasks and threads
class TaskScheduler {
	// timeout for semaphore wait, default 50ms
	constexpr static int64_t TASK_TIMEOUT_USECS = 50000;

public:
	TaskScheduler();
	~TaskScheduler();

	static TaskScheduler &GetScheduler(ClientContext &context);

	unique_ptr<ProducerToken> CreateProducer();
	//! Schedule a task to be executed by the task scheduler
	void ScheduleTask(ProducerToken &producer, unique_ptr<Task> task);
	//! Fetches a task from a specific producer, returns true if successful or false if no tasks were available
	bool GetTaskFromProducer(ProducerToken &token, unique_ptr<Task> &task);
	//! Run tasks forever until "marker" is set to false, "marker" must remain valid until the thread is joined
	void ExecuteForever(bool *marker);

	//! Sets the amount of active threads executing tasks for the system; n-1 background threads will be launched.
	//! The main thread will also be used for execution
	void SetThreads(int32_t n);
	//! Returns the number of threads
	int32_t NumberOfThreads();

private:
	void SetThreadsInternal(int32_t n);

	//! The task queue
	unique_ptr<ConcurrentQueue> queue;
	//! The active background threads of the task scheduler
	vector<unique_ptr<SchedulerThread>> threads;
	//! Markers used by the various threads, if the markers are set to "false" the thread execution is stopped
	vector<unique_ptr<bool>> markers;
};

} // namespace guinsoodb

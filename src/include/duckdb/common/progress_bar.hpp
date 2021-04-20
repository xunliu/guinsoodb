//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/progress_bar.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#ifndef DUCKDB_NO_THREADS
#include <thread>
#include <future>
#endif

#include "guinsoodb.h"
#include "guinsoodb/execution/executor.hpp"

namespace guinsoodb {
class ProgressBar {
public:
	explicit ProgressBar(Executor *executor, idx_t show_progress_after, idx_t time_update_bar = 100)
	    : executor(executor), show_progress_after(show_progress_after), time_update_bar(time_update_bar) {

	                                                                    };

	//! Starts the thread
	void Start();
	//! Stops the thread
	void Stop();
	//! Gets current percentage
	int GetCurrentPercentage();

private:
	const string PROGRESS_BAR_STRING = "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||";
	static constexpr const idx_t PROGRESS_BAR_WIDTH = 60;
	Executor *executor = nullptr;
#ifndef DUCKDB_NO_THREADS
	std::thread progress_bar_thread;
	std::condition_variable c;
	std::mutex m;
#endif
	idx_t show_progress_after;
	idx_t time_update_bar;
	int current_percentage = -1;
	bool stop = false;
	//! In case our progress bar tries to use a scan operator that is not implemented we don't print anything
	bool supported = true;
	//! Starts the Progress Bar Thread that prints the progress bar
	void ProgressBarThread();

#ifndef DUCKDB_NO_THREADS
	template <class DURATION>
	bool WaitFor(DURATION duration) {
		std::unique_lock<std::mutex> l(m);
		return !c.wait_for(l, duration, [this]() { return stop; });
	}
#endif
};
} // namespace guinsoodb
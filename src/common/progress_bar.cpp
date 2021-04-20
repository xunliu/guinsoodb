#include "guinsoodb/common/progress_bar.hpp"
#include "guinsoodb/common/printer.hpp"
#include "guinsoodb/main/client_context.hpp"

namespace guinsoodb {

void ProgressBar::ProgressBarThread() {
#ifndef GUINSOODB_NO_THREADS
	WaitFor(std::chrono::milliseconds(show_progress_after));
	while (!stop) {
		int new_percentage;
		supported = executor->GetPipelinesProgress(new_percentage);
		current_percentage = new_percentage;
		if (supported && current_percentage > -1 && executor->context.print_progress_bar) {
			Printer::PrintProgress(current_percentage, PROGRESS_BAR_STRING.c_str(), PROGRESS_BAR_WIDTH);
		}
		WaitFor(std::chrono::milliseconds(time_update_bar));
	}
#endif
}

int ProgressBar::GetCurrentPercentage() {
	return current_percentage;
}

void ProgressBar::Start() {
#ifndef GUINSOODB_NO_THREADS
	current_percentage = 0;
	progress_bar_thread = std::thread(&ProgressBar::ProgressBarThread, this);
#endif
}

void ProgressBar::Stop() {
#ifndef GUINSOODB_NO_THREADS
	if (progress_bar_thread.joinable()) {
		{
			std::lock_guard<std::mutex> l(m);
			stop = true;
		}
		c.notify_one();
		progress_bar_thread.join();
		if (supported && current_percentage > 0 && executor->context.print_progress_bar) {
			Printer::FinishProgressBarPrint(PROGRESS_BAR_STRING.c_str(), PROGRESS_BAR_WIDTH);
		}
	}
#endif
}
} // namespace guinsoodb

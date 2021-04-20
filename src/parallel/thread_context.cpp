#include "guinsoodb/parallel/thread_context.hpp"

#include "guinsoodb/main/client_context.hpp"

namespace guinsoodb {

ThreadContext::ThreadContext(ClientContext &context) : profiler(context.profiler.IsEnabled()) {
}

} // namespace guinsoodb

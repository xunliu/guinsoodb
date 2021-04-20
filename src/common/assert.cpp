#include "guinsoodb/common/assert.hpp"
#include "guinsoodb/common/exception.hpp"

namespace guinsoodb {

void GuinsooDBAssertInternal(bool condition, const char *condition_name, const char *file, int linenr) {
	if (condition) {
		return;
	}
	throw InternalException("Assertion triggered in file \"%s\" on line %d: %s", file, linenr, condition_name);
}

} // namespace guinsoodb

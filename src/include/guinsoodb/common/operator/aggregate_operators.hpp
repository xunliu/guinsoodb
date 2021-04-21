//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/operator/aggregate_operators.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include "guinsoodb/common/operator/comparison_operators.hpp"

namespace guinsoodb {

struct Min {
	template <class T>
	static inline T Operation(T left, T right) {
		return LessThan::Operation(left, right) ? left : right;
	}
};

struct Max {
	template <class T>
	static inline T Operation(T left, T right) {
		return GreaterThan::Operation(left, right) ? left : right;
	}
};

} // namespace guinsoodb

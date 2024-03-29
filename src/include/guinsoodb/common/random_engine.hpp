//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/random_engine.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/limits.hpp"
#include <random>

namespace guinsoodb {

struct RandomEngine {
	std::mt19937 random_engine;
	explicit RandomEngine(int64_t seed) {
		if (seed < 0) {
			std::random_device rd;
			random_engine.seed(rd());
		} else {
			random_engine.seed(seed);
		}
	}

	//! Generate a random number between min and max
	double NextRandom(double min, double max) {
		std::uniform_real_distribution<double> dist(min, max);
		return dist(random_engine);
	}
	//! Generate a random number between 0 and 1
	double NextRandom() {
		return NextRandom(0, 1);
	}
	uint32_t NextRandomInteger() {
		std::uniform_int_distribution<uint32_t> dist(0, NumericLimits<uint32_t>::Maximum());
		return dist(random_engine);
	}
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/enums/date_part_specifier.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"

namespace guinsoodb {

enum class DatePartSpecifier : uint8_t {
	YEAR,
	MONTH,
	DAY,
	DECADE,
	CENTURY,
	MILLENNIUM,
	MICROSECONDS,
	MILLISECONDS,
	SECOND,
	MINUTE,
	HOUR,
	EPOCH,
	DOW,
	ISODOW,
	WEEK,
	QUARTER,
	DOY
};

DatePartSpecifier GetDatePartSpecifier(string specifier);

} // namespace guinsoodb

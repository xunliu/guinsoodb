#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/types.hpp"
#include "fmt/format.h"
#include "fmt/printf.h"

namespace guinsoodb {

template <>
ExceptionFormatValue ExceptionFormatValue::CreateFormatValue(PhysicalType value) {
	return ExceptionFormatValue(TypeIdToString(value));
}
template <>
ExceptionFormatValue
ExceptionFormatValue::CreateFormatValue(LogicalType value) { // NOLINT: templating requires us to copy value here
	return ExceptionFormatValue(value.ToString());
}
template <>
ExceptionFormatValue ExceptionFormatValue::CreateFormatValue(float value) {
	return ExceptionFormatValue(double(value));
}
template <>
ExceptionFormatValue ExceptionFormatValue::CreateFormatValue(double value) {
	return ExceptionFormatValue(double(value));
}
template <>
ExceptionFormatValue ExceptionFormatValue::CreateFormatValue(string value) {
	return ExceptionFormatValue(move(value));
}
template <>
ExceptionFormatValue ExceptionFormatValue::CreateFormatValue(const char *value) {
	return ExceptionFormatValue(string(value));
}
template <>
ExceptionFormatValue ExceptionFormatValue::CreateFormatValue(char *value) {
	return ExceptionFormatValue(string(value));
}

string ExceptionFormatValue::Format(const string &msg, vector<ExceptionFormatValue> &values) {
	std::vector<guinsoodb_fmt::basic_format_arg<guinsoodb_fmt::printf_context>> format_args;
	for (auto &val : values) {
		switch (val.type) {
		case ExceptionFormatValueType::FORMAT_VALUE_TYPE_DOUBLE:
			format_args.push_back(guinsoodb_fmt::internal::make_arg<guinsoodb_fmt::printf_context>(val.dbl_val));
			break;
		case ExceptionFormatValueType::FORMAT_VALUE_TYPE_INTEGER:
			format_args.push_back(guinsoodb_fmt::internal::make_arg<guinsoodb_fmt::printf_context>(val.int_val));
			break;
		case ExceptionFormatValueType::FORMAT_VALUE_TYPE_STRING:
			format_args.push_back(guinsoodb_fmt::internal::make_arg<guinsoodb_fmt::printf_context>(val.str_val));
			break;
		}
	}
	return guinsoodb_fmt::vsprintf(msg, guinsoodb_fmt::basic_format_args<guinsoodb_fmt::printf_context>(
	                                     format_args.data(), static_cast<int>(format_args.size())));
}

} // namespace guinsoodb

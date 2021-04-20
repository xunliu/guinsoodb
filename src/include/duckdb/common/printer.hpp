//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/printer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"

namespace guinsoodb {

//! Printer is a static class that allows printing to logs or stdout/stderr
class Printer {
public:
	//! Print the object to stderr
	static void Print(const string &str);
	//! Prints Progress
	static void PrintProgress(int percentage, const char *pbstr, int pbwidth);
	//! Prints an empty line when progress bar is done
	static void FinishProgressBarPrint(const char *pbstr, int pbwidth);
};
} // namespace guinsoodb

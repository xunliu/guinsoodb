//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/function_set.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/aggregate_function.hpp"
#include "guinsoodb/function/scalar_function.hpp"
#include "guinsoodb/function/table_function.hpp"

namespace guinsoodb {

template <class T>
class FunctionSet {
public:
	explicit FunctionSet(string name) : name(name) {
	}

	//! The name of the function set
	string name;
	//! The set of functions
	vector<T> functions;

public:
	void AddFunction(T function) {
		function.name = name;
		functions.push_back(function);
	}
};

class ScalarFunctionSet : public FunctionSet<ScalarFunction> {
public:
	explicit ScalarFunctionSet(string name) : FunctionSet(move(name)) {
	}
};

class AggregateFunctionSet : public FunctionSet<AggregateFunction> {
public:
	explicit AggregateFunctionSet(string name) : FunctionSet(move(name)) {
	}
};

class TableFunctionSet : public FunctionSet<TableFunction> {
public:
	explicit TableFunctionSet(string name) : FunctionSet(move(name)) {
	}
};

} // namespace guinsoodb

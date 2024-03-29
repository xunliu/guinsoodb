//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parsed_expression_iterator.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"

#include <functional>

namespace guinsoodb {

class ParsedExpressionIterator {
public:
	static void EnumerateChildren(const ParsedExpression &expression,
	                              const std::function<void(const ParsedExpression &child)> &callback);
	static void EnumerateChildren(ParsedExpression &expr, const std::function<void(ParsedExpression &child)> &callback);
	static void EnumerateChildren(ParsedExpression &expr,
	                              const std::function<void(unique_ptr<ParsedExpression> &child)> &callback);
};

} // namespace guinsoodb

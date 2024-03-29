//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/bound_result_modifier.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/result_modifier.hpp"
#include "guinsoodb/planner/bound_statement.hpp"
#include "guinsoodb/planner/expression.hpp"
#include "guinsoodb/common/limits.hpp"

namespace guinsoodb {

//! A ResultModifier
class BoundResultModifier {
public:
	explicit BoundResultModifier(ResultModifierType type) : type(type) {
	}
	virtual ~BoundResultModifier() {
	}

	ResultModifierType type;
};

struct BoundOrderByNode {
	BoundOrderByNode(OrderType type, OrderByNullType null_order, unique_ptr<Expression> expression)
	    : type(type), null_order(null_order), expression(move(expression)) {
	}

	OrderType type;
	OrderByNullType null_order;
	unique_ptr<Expression> expression;
};

class BoundLimitModifier : public BoundResultModifier {
public:
	BoundLimitModifier() : BoundResultModifier(ResultModifierType::LIMIT_MODIFIER) {
	}
	//! LIMIT
	int64_t limit_val = NumericLimits<int64_t>::Maximum();
	//! OFFSET
	int64_t offset_val = 0;
	//! Expression in case limit is not constant
	unique_ptr<Expression> limit;
	//! Expression in case limit is not constant
	unique_ptr<Expression> offset;
};

class BoundOrderModifier : public BoundResultModifier {
public:
	BoundOrderModifier() : BoundResultModifier(ResultModifierType::ORDER_MODIFIER) {
	}

	//! List of order nodes
	vector<BoundOrderByNode> orders;
};

class BoundDistinctModifier : public BoundResultModifier {
public:
	BoundDistinctModifier() : BoundResultModifier(ResultModifierType::DISTINCT_MODIFIER) {
	}

	//! list of distinct on targets (if any)
	vector<unique_ptr<Expression>> target_distincts;
};

} // namespace guinsoodb

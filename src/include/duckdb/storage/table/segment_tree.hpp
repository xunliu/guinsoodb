//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/table/segment_tree.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"
#include "guinsoodb/storage/storage_lock.hpp"
#include "guinsoodb/storage/table/segment_base.hpp"
#include "guinsoodb/common/vector.hpp"
#include "guinsoodb/common/mutex.hpp"

namespace guinsoodb {

struct SegmentNode {
	idx_t row_start;
	SegmentBase *node;
};

//! The SegmentTree maintains a list of all segments of a specific column in a table, and allows searching for a segment
//! by row number
class SegmentTree {
public:
	//! The initial segment of the tree
	unique_ptr<SegmentBase> root_node;
	//! The nodes in the tree, can be binary searched
	vector<SegmentNode> nodes;
	//! Lock to access or modify the nodes
	mutex node_lock;

public:
	//! Gets a pointer to the first segment. Useful for scans.
	SegmentBase *GetRootSegment();
	//! Gets a pointer to the last segment. Useful for appends.
	SegmentBase *GetLastSegment();
	//! Gets a pointer to a specific column segment for the given row
	SegmentBase *GetSegment(idx_t row_number);
	//! Append a column segment to the tree
	void AppendSegment(unique_ptr<SegmentBase> segment);

	//! Replace this tree with another tree, taking over its nodes in-place
	void Replace(SegmentTree &other);

	//! Get the segment index of the column segment for the given row (does not lock the segment tree!)
	idx_t GetSegmentIndex(idx_t row_number);
};

} // namespace guinsoodb

#include "guinsoodb/storage/table/segment_tree.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/string_util.hpp"
namespace guinsoodb {

SegmentBase *SegmentTree::GetRootSegment() {
	return root_node.get();
}

SegmentBase *SegmentTree::GetLastSegment() {
	return nodes.back().node;
}

SegmentBase *SegmentTree::GetSegment(idx_t row_number) {
	lock_guard<mutex> tree_lock(node_lock);
	return nodes[GetSegmentIndex(row_number)].node;
}

idx_t SegmentTree::GetSegmentIndex(idx_t row_number) {
	idx_t lower = 0;
	idx_t upper = nodes.size() - 1;
	// binary search to find the node
	while (lower <= upper) {
		idx_t index = (lower + upper) / 2;
		auto &entry = nodes[index];
		if (row_number < entry.row_start) {
			upper = index - 1;
		} else if (row_number >= entry.row_start + entry.node->count) {
			lower = index + 1;
		} else {
			return index;
		}
	}
	throw InternalException("Could not find node in column segment tree!");
}

void SegmentTree::AppendSegment(unique_ptr<SegmentBase> segment) {
	D_ASSERT(segment);
	// add the node to the list of nodes
	SegmentNode node;
	node.row_start = segment->start;
	node.node = segment.get();
	nodes.push_back(node);

	if (nodes.size() > 1) {
		// add the node as the next pointer of the last node
		D_ASSERT(!nodes[nodes.size() - 2].node->next);
		nodes[nodes.size() - 2].node->next = move(segment);
	} else {
		root_node = move(segment);
	}
}

void SegmentTree::Replace(SegmentTree &other) {
	root_node = move(other.root_node);
	nodes = move(other.nodes);
}

} // namespace guinsoodb

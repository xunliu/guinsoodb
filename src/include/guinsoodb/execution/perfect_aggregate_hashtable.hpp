//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/perfect_aggregate_hashtable.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/base_aggregate_hashtable.hpp"

namespace guinsoodb {

class PerfectAggregateHashTable : public BaseAggregateHashTable {
public:
	PerfectAggregateHashTable(BufferManager &buffer_manager, vector<LogicalType> group_types,
	                          vector<LogicalType> payload_types_p, vector<AggregateObject> aggregate_objects,
	                          vector<Value> group_minima, vector<idx_t> required_bits);
	~PerfectAggregateHashTable() override;

public:
	//! Add the given data to the HT
	void AddChunk(DataChunk &groups, DataChunk &payload);

	//! Combines the target perfect aggregate HT into this one
	void Combine(PerfectAggregateHashTable &other);

	//! Scan the HT starting from the scan_position
	void Scan(idx_t &scan_position, DataChunk &result);

protected:
	Vector addresses;
	//! The required bits per group
	vector<idx_t> required_bits;
	//! The total required bits for the HT (this determines the max capacity)
	idx_t total_required_bits;
	//! The total amount of groups
	idx_t total_groups;
	//! The tuple size
	idx_t tuple_size;

	// The actual pointer to the data
	data_ptr_t data;
	//! The owned data of the HT
	unique_ptr<data_t[]> owned_data;
	//! Information on whether or not a specific group has any entries
	unique_ptr<bool[]> group_is_set;

	//! The minimum values for each of the group columns
	vector<Value> group_minima;

private:
	void Combine(Vector &source_addresses, Vector &target_addresses, idx_t combine_count);
	//! Destroy the perfect aggregate HT (called automatically by the destructor)
	void Destroy();
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/partitionable_hashtable.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/aggregate_hashtable.hpp"

namespace guinsoodb {

struct RadixPartitionInfo {
	explicit RadixPartitionInfo(idx_t _n_partitions_upper_bound);
	idx_t n_partitions;
	idx_t radix_bits;
	hash_t radix_mask;
	constexpr static idx_t RADIX_SHIFT = 40;
};

typedef vector<unique_ptr<GroupedAggregateHashTable>> HashTableList;

class PartitionableHashTable {
public:
	PartitionableHashTable(BufferManager &buffer_manager_p, RadixPartitionInfo &partition_info_p,
	                       vector<LogicalType> group_types_p, vector<LogicalType> payload_types_p,
	                       vector<BoundAggregateExpression *> bindings_p);

	idx_t AddChunk(DataChunk &groups, DataChunk &payload, bool do_partition);
	void Partition();
	bool IsPartitioned();

	HashTableList GetPartition(idx_t partition);
	HashTableList GetUnpartitioned();

	void Finalize();

private:
	BufferManager &buffer_manager;
	vector<LogicalType> group_types;
	vector<LogicalType> payload_types;
	vector<BoundAggregateExpression *> bindings;

	bool is_partitioned;
	RadixPartitionInfo &partition_info;
	vector<SelectionVector> sel_vectors;
	vector<idx_t> sel_vector_sizes;
	DataChunk group_subset, payload_subset;
	Vector hashes, hashes_subset;

	HashTableList unpartitioned_hts;
	unordered_map<hash_t, HashTableList> radix_partitioned_hts;

private:
	idx_t ListAddChunk(HashTableList &list, DataChunk &groups, Vector &group_hashes, DataChunk &payload);
};
} // namespace guinsoodb

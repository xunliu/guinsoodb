#include "guinsoodb/execution/partitionable_hashtable.hpp"

namespace guinsoodb {

RadixPartitionInfo::RadixPartitionInfo(idx_t n_partitions_upper_bound) : n_partitions(1), radix_bits(0), radix_mask(0) {
	while (n_partitions <= n_partitions_upper_bound / 2) {
		n_partitions *= 2;
		if (n_partitions >= 256) {
			break;
		}
	}
	// finalize_threads needs to be a power of 2
	D_ASSERT(n_partitions > 0);
	D_ASSERT(n_partitions <= 256);
	D_ASSERT((n_partitions & (n_partitions - 1)) == 0);

	auto radix_partitions_copy = n_partitions;
	while (radix_partitions_copy - 1) {
		radix_bits++;
		radix_partitions_copy >>= 1;
	}

	D_ASSERT(radix_bits <= 8);

	// we use the fifth byte of the 64 bit hash as radix source
	for (idx_t i = 0; i < radix_bits; i++) {
		radix_mask = (radix_mask << 1) | 1;
	}
	radix_mask <<= RADIX_SHIFT;
}

PartitionableHashTable::PartitionableHashTable(BufferManager &buffer_manager_p, RadixPartitionInfo &partition_info_p,
                                               vector<LogicalType> group_types_p, vector<LogicalType> payload_types_p,
                                               vector<BoundAggregateExpression *> bindings_p)
    : buffer_manager(buffer_manager_p), group_types(move(group_types_p)), payload_types(move(payload_types_p)),
      bindings(move(bindings_p)), is_partitioned(false), partition_info(partition_info_p) {

	sel_vectors.resize(partition_info.n_partitions);
	sel_vector_sizes.resize(partition_info.n_partitions);
	group_subset.Initialize(group_types);
	if (!payload_types.empty()) {
		payload_subset.Initialize(payload_types);
	}
	hashes.Initialize(LogicalType::HASH);
	hashes_subset.Initialize(LogicalType::HASH);

	for (hash_t r = 0; r < partition_info.n_partitions; r++) {
		sel_vectors[r].Initialize();
	}
}

idx_t PartitionableHashTable::ListAddChunk(HashTableList &list, DataChunk &groups, Vector &group_hashes,
                                           DataChunk &payload) {
	if (list.empty() || list.back()->Size() + groups.size() > list.back()->MaxCapacity()) {
		if (!list.empty()) {
			// early release first part of ht and prevent adding of more data
			list.back()->Finalize();
		}
		list.push_back(make_unique<GroupedAggregateHashTable>(buffer_manager, group_types, payload_types, bindings,
		                                                      HtEntryType::HT_WIDTH_32));
	}
	return list.back()->AddChunk(groups, group_hashes, payload);
}

idx_t PartitionableHashTable::AddChunk(DataChunk &groups, DataChunk &payload, bool do_partition) {
	groups.Hash(hashes);

	// we partition when we are asked to or when the unpartitioned ht runs out of space
	if (!IsPartitioned() && do_partition) {
		Partition();
	}

	if (!IsPartitioned()) {
		return ListAddChunk(unpartitioned_hts, groups, hashes, payload);
	}

	// makes no sense to do this with 1 partition
	D_ASSERT(partition_info.n_partitions > 0);

	for (hash_t r = 0; r < partition_info.n_partitions; r++) {
		sel_vector_sizes[r] = 0;
	}

	D_ASSERT(hashes.GetVectorType() == VectorType::FLAT_VECTOR);
	auto hashes_ptr = FlatVector::GetData<hash_t>(hashes);

	for (idx_t i = 0; i < groups.size(); i++) {
		auto partition = (hashes_ptr[i] & partition_info.radix_mask) >> partition_info.RADIX_SHIFT;
		D_ASSERT(partition < partition_info.n_partitions);
		sel_vectors[partition].set_index(sel_vector_sizes[partition]++, i);
	}

#ifdef DEBUG
	// make sure we have lost no rows
	idx_t total_count = 0;
	for (idx_t r = 0; r < partition_info.n_partitions; r++) {
		total_count += sel_vector_sizes[r];
	}
	D_ASSERT(total_count == groups.size());
#endif
	idx_t group_count = 0;
	for (hash_t r = 0; r < partition_info.n_partitions; r++) {
		group_subset.Slice(groups, sel_vectors[r], sel_vector_sizes[r]);
		payload_subset.Slice(payload, sel_vectors[r], sel_vector_sizes[r]);
		hashes_subset.Slice(hashes, sel_vectors[r], sel_vector_sizes[r]);

		group_count += ListAddChunk(radix_partitioned_hts[r], group_subset, hashes_subset, payload_subset);
	}
	return group_count;
}

void PartitionableHashTable::Partition() {
	D_ASSERT(!IsPartitioned());
	D_ASSERT(radix_partitioned_hts.size() == 0);
	D_ASSERT(partition_info.n_partitions > 1);

	vector<GroupedAggregateHashTable *> partition_hts;
	for (auto &unpartitioned_ht : unpartitioned_hts) {
		for (idx_t r = 0; r < partition_info.n_partitions; r++) {
			radix_partitioned_hts[r].push_back(make_unique<GroupedAggregateHashTable>(
			    buffer_manager, group_types, payload_types, bindings, HtEntryType::HT_WIDTH_32));
			partition_hts.push_back(radix_partitioned_hts[r].back().get());
		}
		unpartitioned_ht->Partition(partition_hts, partition_info.radix_mask, partition_info.RADIX_SHIFT);
		unpartitioned_ht.reset();
	}
	unpartitioned_hts.clear();
	is_partitioned = true;
}

bool PartitionableHashTable::IsPartitioned() {
	return is_partitioned;
}

HashTableList PartitionableHashTable::GetPartition(idx_t partition) {
	D_ASSERT(IsPartitioned());
	D_ASSERT(partition < partition_info.n_partitions);
	D_ASSERT(radix_partitioned_hts.size() > partition);
	return move(radix_partitioned_hts[partition]);
}
HashTableList PartitionableHashTable::GetUnpartitioned() {
	D_ASSERT(!IsPartitioned());
	return move(unpartitioned_hts);
}

void PartitionableHashTable::Finalize() {
	if (IsPartitioned()) {
		for (auto &ht_list : radix_partitioned_hts) {
			for (auto &ht : ht_list.second) {
				D_ASSERT(ht);
				ht->Finalize();
			}
		}
	} else {
		for (auto &ht : unpartitioned_hts) {
			D_ASSERT(ht);
			ht->Finalize();
		}
	}
}

} // namespace guinsoodb

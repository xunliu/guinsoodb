#include "guinsoodb/execution/operator/aggregate/physical_hash_aggregate.hpp"
#include "guinsoodb/execution/operator/aggregate/physical_perfecthash_aggregate.hpp"
#include "guinsoodb/execution/operator/aggregate/physical_simple_aggregate.hpp"
#include "guinsoodb/execution/operator/aggregate/physical_window.hpp"
#include "guinsoodb/execution/operator/filter/physical_filter.hpp"
#include "guinsoodb/execution/operator/helper/physical_execute.hpp"
#include "guinsoodb/execution/operator/helper/physical_limit.hpp"
#include "guinsoodb/execution/operator/helper/physical_pragma.hpp"
#include "guinsoodb/execution/operator/helper/physical_prepare.hpp"
#include "guinsoodb/execution/operator/helper/physical_reservoir_sample.hpp"
#include "guinsoodb/execution/operator/helper/physical_set.hpp"
#include "guinsoodb/execution/operator/helper/physical_streaming_sample.hpp"
#include "guinsoodb/execution/operator/helper/physical_transaction.hpp"
#include "guinsoodb/execution/operator/helper/physical_vacuum.hpp"
#include "guinsoodb/execution/operator/join/physical_blockwise_nl_join.hpp"
#include "guinsoodb/execution/operator/join/physical_comparison_join.hpp"
#include "guinsoodb/execution/operator/join/physical_cross_product.hpp"
#include "guinsoodb/execution/operator/join/physical_delim_join.hpp"
#include "guinsoodb/execution/operator/join/physical_hash_join.hpp"
#include "guinsoodb/execution/operator/join/physical_index_join.hpp"
#include "guinsoodb/execution/operator/join/physical_join.hpp"
#include "guinsoodb/execution/operator/join/physical_nested_loop_join.hpp"
#include "guinsoodb/execution/operator/join/physical_piecewise_merge_join.hpp"
#include "guinsoodb/execution/operator/order/physical_order.hpp"
#include "guinsoodb/execution/operator/order/physical_top_n.hpp"
#include "guinsoodb/execution/operator/persistent/buffered_csv_reader.hpp"
#include "guinsoodb/execution/operator/persistent/physical_copy_to_file.hpp"
#include "guinsoodb/execution/operator/persistent/physical_delete.hpp"
#include "guinsoodb/execution/operator/persistent/physical_export.hpp"
#include "guinsoodb/execution/operator/persistent/physical_insert.hpp"
#include "guinsoodb/execution/operator/persistent/physical_update.hpp"
#include "guinsoodb/execution/operator/projection/physical_projection.hpp"
#include "guinsoodb/execution/operator/projection/physical_unnest.hpp"
#include "guinsoodb/execution/operator/scan/physical_chunk_scan.hpp"
#include "guinsoodb/execution/operator/scan/physical_dummy_scan.hpp"
#include "guinsoodb/execution/operator/scan/physical_empty_result.hpp"
#include "guinsoodb/execution/operator/scan/physical_expression_scan.hpp"
#include "guinsoodb/execution/operator/scan/physical_table_scan.hpp"
#include "guinsoodb/execution/operator/schema/physical_alter.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_function.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_index.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_schema.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_sequence.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_table.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_table_as.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_view.hpp"
#include "guinsoodb/execution/operator/schema/physical_drop.hpp"
#include "guinsoodb/execution/operator/set/physical_recursive_cte.hpp"
#include "guinsoodb/execution/operator/set/physical_union.hpp"

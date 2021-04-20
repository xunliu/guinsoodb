#include "guinsoodb/planner/operator/logical_aggregate.hpp"
#include "guinsoodb/planner/operator/logical_any_join.hpp"
#include "guinsoodb/planner/operator/logical_chunk_get.hpp"
#include "guinsoodb/planner/operator/logical_comparison_join.hpp"
#include "guinsoodb/planner/operator/logical_copy_to_file.hpp"
#include "guinsoodb/planner/operator/logical_create.hpp"
#include "guinsoodb/planner/operator/logical_create_index.hpp"
#include "guinsoodb/planner/operator/logical_create_table.hpp"
#include "guinsoodb/planner/operator/logical_cross_product.hpp"
#include "guinsoodb/planner/operator/logical_cteref.hpp"
#include "guinsoodb/planner/operator/logical_delete.hpp"
#include "guinsoodb/planner/operator/logical_delim_get.hpp"
#include "guinsoodb/planner/operator/logical_delim_join.hpp"
#include "guinsoodb/planner/operator/logical_distinct.hpp"
#include "guinsoodb/planner/operator/logical_dummy_scan.hpp"
#include "guinsoodb/planner/operator/logical_empty_result.hpp"
#include "guinsoodb/planner/operator/logical_execute.hpp"
#include "guinsoodb/planner/operator/logical_explain.hpp"
#include "guinsoodb/planner/operator/logical_export.hpp"
#include "guinsoodb/planner/operator/logical_expression_get.hpp"
#include "guinsoodb/planner/operator/logical_filter.hpp"
#include "guinsoodb/planner/operator/logical_get.hpp"
#include "guinsoodb/planner/operator/logical_insert.hpp"
#include "guinsoodb/planner/operator/logical_join.hpp"
#include "guinsoodb/planner/operator/logical_limit.hpp"
#include "guinsoodb/planner/operator/logical_order.hpp"
#include "guinsoodb/planner/operator/logical_pragma.hpp"
#include "guinsoodb/planner/operator/logical_prepare.hpp"
#include "guinsoodb/planner/operator/logical_projection.hpp"
#include "guinsoodb/planner/operator/logical_recursive_cte.hpp"
#include "guinsoodb/planner/operator/logical_sample.hpp"
#include "guinsoodb/planner/operator/logical_set.hpp"
#include "guinsoodb/planner/operator/logical_set_operation.hpp"
#include "guinsoodb/planner/operator/logical_show.hpp"
#include "guinsoodb/planner/operator/logical_simple.hpp"
#include "guinsoodb/planner/operator/logical_top_n.hpp"
#include "guinsoodb/planner/operator/logical_unnest.hpp"
#include "guinsoodb/planner/operator/logical_update.hpp"
#include "guinsoodb/planner/operator/logical_window.hpp"

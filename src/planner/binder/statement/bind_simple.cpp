#include "guinsoodb/parser/statement/alter_statement.hpp"
#include "guinsoodb/parser/statement/transaction_statement.hpp"
#include "guinsoodb/planner/operator/logical_simple.hpp"
#include "guinsoodb/catalog/catalog.hpp"
#include "guinsoodb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "guinsoodb/catalog/catalog_entry/view_catalog_entry.hpp"
#include "guinsoodb/planner/binder.hpp"

//! This file contains the binder definitions for statements that do not need to be bound at all and only require a
//! straightforward conversion

namespace guinsoodb {

BoundStatement Binder::Bind(AlterStatement &stmt) {
	BoundStatement result;
	result.names = {"Success"};
	result.types = {LogicalType::BOOLEAN};
	Catalog &catalog = Catalog::GetCatalog(context);
	auto entry = catalog.GetEntry(context, stmt.info->GetCatalogType(), stmt.info->schema, stmt.info->name, true);
	if (entry && !entry->temporary) {
		// we can only alter temporary tables/views in read-only mode
		this->read_only = false;
	}
	result.plan = make_unique<LogicalSimple>(LogicalOperatorType::LOGICAL_ALTER, move(stmt.info));
	return result;
}

BoundStatement Binder::Bind(TransactionStatement &stmt) {
	// transaction statements do not require a valid transaction
	this->requires_valid_transaction = false;

	BoundStatement result;
	result.names = {"Success"};
	result.types = {LogicalType::BOOLEAN};
	result.plan = make_unique<LogicalSimple>(LogicalOperatorType::LOGICAL_TRANSACTION, move(stmt.info));
	return result;
}

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/checkpoint/table_data_reader.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/storage/checkpoint_manager.hpp"

namespace guinsoodb {
struct BoundCreateTableInfo;

//! The table data reader is responsible for reading the data of a table from the block manager
class TableDataReader {
public:
	TableDataReader(DatabaseInstance &db, MetaBlockReader &reader, BoundCreateTableInfo &info);

	void ReadTableData();

private:
	DatabaseInstance &db;
	MetaBlockReader &reader;
	BoundCreateTableInfo &info;
};

} // namespace guinsoodb

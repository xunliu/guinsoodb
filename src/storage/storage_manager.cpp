#include "guinsoodb/storage/storage_manager.hpp"
#include "guinsoodb/storage/checkpoint_manager.hpp"
#include "guinsoodb/storage/in_memory_block_manager.hpp"
#include "guinsoodb/storage/single_file_block_manager.hpp"
#include "guinsoodb/storage/object_cache.hpp"

#include "guinsoodb/catalog/catalog.hpp"
#include "guinsoodb/common/file_system.hpp"
#include "guinsoodb/main/database.hpp"
#include "guinsoodb/main/connection.hpp"
#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/function/function.hpp"
#include "guinsoodb/parser/parsed_data/create_schema_info.hpp"
#include "guinsoodb/transaction/transaction_manager.hpp"
#include "guinsoodb/common/serializer/buffered_file_reader.hpp"
#include "guinsoodb/storage/checkpoint_manager.hpp"

namespace guinsoodb {

StorageManager::StorageManager(DatabaseInstance &db, string path, bool read_only)
    : db(db), path(move(path)), wal(db), read_only(read_only) {
}

StorageManager::~StorageManager() {
}

StorageManager &StorageManager::GetStorageManager(ClientContext &context) {
	return StorageManager::GetStorageManager(*context.db);
}

BufferManager &BufferManager::GetBufferManager(ClientContext &context) {
	return BufferManager::GetBufferManager(*context.db);
}

ObjectCache &ObjectCache::GetObjectCache(ClientContext &context) {
	return context.db->GetObjectCache();
}

bool ObjectCache::ObjectCacheEnabled(ClientContext &context) {
	return context.db->config.object_cache_enable;
}

bool StorageManager::InMemory() {
	return path.empty() || path == ":memory:";
}

void StorageManager::Initialize() {
	bool in_memory = InMemory();
	if (in_memory && read_only) {
		throw CatalogException("Cannot launch in-memory database in read-only mode!");
	}

	// first initialize the base system catalogs
	// these are never written to the WAL
	Connection con(db);
	con.BeginTransaction();

	auto &catalog = Catalog::GetCatalog(*con.context);

	// create the default schema
	CreateSchemaInfo info;
	info.schema = DEFAULT_SCHEMA;
	info.internal = true;
	catalog.CreateSchema(*con.context, &info);

	// initialize default functions
	BuiltinFunctions builtin(*con.context, catalog);
	builtin.Initialize();

	// commit transactions
	con.Commit();

	if (!in_memory) {
		// create or load the database from disk, if not in-memory mode
		LoadDatabase();
	} else {
		auto &config = DBConfig::GetConfig(*con.context);
		block_manager = make_unique<InMemoryBlockManager>();
		buffer_manager = make_unique<BufferManager>(db, config.temporary_directory, config.maximum_memory);
	}
}

void StorageManager::LoadDatabase() {
	string wal_path = path + ".wal";
	auto &fs = db.GetFileSystem();
	auto &config = db.config;
	bool truncate_wal = false;
	// first check if the database exists
	if (!fs.FileExists(path)) {
		if (read_only) {
			throw CatalogException("Cannot open database \"%s\" in read-only mode: database does not exist", path);
		}
		// check if the WAL exists
		if (fs.FileExists(wal_path)) {
			// WAL file exists but database file does not
			// remove the WAL
			fs.RemoveFile(wal_path);
		}
		// initialize the block manager while creating a new db file
		block_manager = make_unique<SingleFileBlockManager>(db, path, read_only, true, config.use_direct_io);
		buffer_manager = make_unique<BufferManager>(db, config.temporary_directory, config.maximum_memory);
	} else {
		// initialize the block manager while loading the current db file
		auto sf_bm = make_unique<SingleFileBlockManager>(db, path, read_only, false, config.use_direct_io);
		auto sf = sf_bm.get();
		block_manager = move(sf_bm);
		buffer_manager = make_unique<BufferManager>(db, config.temporary_directory, config.maximum_memory);
		sf->LoadFreeList();

		//! Load from storage
		CheckpointManager checkpointer(db);
		checkpointer.LoadFromStorage();
		// check if the WAL file exists
		if (fs.FileExists(wal_path)) {
			// replay the WAL
			truncate_wal = WriteAheadLog::Replay(db, wal_path);
		}
	}
	// initialize the WAL file
	if (!read_only) {
		wal.Initialize(wal_path);
		if (truncate_wal) {
			wal.Truncate(0);
		}
	}
}

void StorageManager::CreateCheckpoint(bool delete_wal, bool force_checkpoint) {
	if (InMemory() || read_only || !wal.initialized) {
		return;
	}
	if (wal.GetWALSize() > 0 || db.config.force_checkpoint || force_checkpoint) {
		// we only need to checkpoint if there is anything in the WAL
		CheckpointManager checkpointer(db);
		checkpointer.CreateCheckpoint();
	}
	if (delete_wal) {
		wal.Delete();
	}
}

} // namespace guinsoodb

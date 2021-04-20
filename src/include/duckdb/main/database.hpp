//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/database.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/mutex.hpp"
#include "guinsoodb/common/winapi.hpp"
#include "guinsoodb/main/config.hpp"
#include "guinsoodb/main/extension.hpp"

namespace guinsoodb {
class StorageManager;
class Catalog;
class TransactionManager;
class ConnectionManager;
class FileSystem;
class TaskScheduler;
class ObjectCache;

class DatabaseInstance : public std::enable_shared_from_this<DatabaseInstance> {
	friend class GuinsooDB;

public:
	GUINSOODB_API DatabaseInstance();
	GUINSOODB_API ~DatabaseInstance();

	DBConfig config;

public:
	StorageManager &GetStorageManager();
	Catalog &GetCatalog();
	FileSystem &GetFileSystem();
	TransactionManager &GetTransactionManager();
	TaskScheduler &GetScheduler();
	ObjectCache &GetObjectCache();
	ConnectionManager &GetConnectionManager();

	idx_t NumberOfThreads();

	static DatabaseInstance &GetDatabase(ClientContext &context);

private:
	void Initialize(const char *path, DBConfig *config);

	void Configure(DBConfig &config);

private:
	unique_ptr<StorageManager> storage;
	unique_ptr<Catalog> catalog;
	unique_ptr<TransactionManager> transaction_manager;
	unique_ptr<TaskScheduler> scheduler;
	unique_ptr<ObjectCache> object_cache;
	unique_ptr<ConnectionManager> connection_manager;
};

//! The database object. This object holds the catalog and all the
//! database-specific meta information.
class GuinsooDB {
public:
	GUINSOODB_API explicit GuinsooDB(const char *path = nullptr, DBConfig *config = nullptr);
	GUINSOODB_API explicit GuinsooDB(const string &path, DBConfig *config = nullptr);
	GUINSOODB_API ~GuinsooDB();

	//! Reference to the actual database instance
	shared_ptr<DatabaseInstance> instance;

public:
	template <class T>
	void LoadExtension() {
		T extension;
		extension.Load(*this);
	}

	GUINSOODB_API FileSystem &GetFileSystem();

	GUINSOODB_API idx_t NumberOfThreads();
	GUINSOODB_API static const char *SourceID();
	GUINSOODB_API static const char *LibraryVersion();
};

} // namespace guinsoodb

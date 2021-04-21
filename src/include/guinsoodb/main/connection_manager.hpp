//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/connection_manager.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/mutex.hpp"
#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/common/vector.hpp"

namespace guinsoodb {
class ClientContext;
class DatabaseInstance;

class ConnectionManager {
public:
	ConnectionManager() {
	}

	void AddConnection(ClientContext &context) {
		lock_guard<mutex> lock(connections_lock);
		connections.push_back(weak_ptr<ClientContext>(context.shared_from_this()));
	}

	vector<shared_ptr<ClientContext>> GetConnectionList() {
		vector<shared_ptr<ClientContext>> result;
		for (size_t i = 0; i < connections.size(); i++) {
			auto connection = connections[i].lock();
			if (!connection) {
				connections.erase(connections.begin() + i);
				i--;
				continue;
			} else {
				result.push_back(move(connection));
			}
		}
		return result;
	}

	static ConnectionManager &Get(DatabaseInstance &db);
	static ConnectionManager &Get(ClientContext &context);

public:
	mutex connections_lock;
	vector<weak_ptr<ClientContext>> connections;
};

} // namespace guinsoodb

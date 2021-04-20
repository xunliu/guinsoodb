#include "guinsoodb_node.hpp"

#define DEFINE_CONSTANT_INTEGER(target, constant, name)                                                                \
	Napi::PropertyDescriptor::Value(#name, Napi::Number::New(env, constant),                                           \
	                                static_cast<napi_property_attributes>(napi_enumerable | napi_configurable)),

namespace {

Napi::Object RegisterModule(Napi::Env env, Napi::Object exports) {
	Napi::HandleScope scope(env);

	node_guinsoodb::Database::Init(env, exports);
	node_guinsoodb::Connection::Init(env, exports);
	node_guinsoodb::Statement::Init(env, exports);

	exports.DefineProperties({
	    DEFINE_CONSTANT_INTEGER(exports, node_guinsoodb::Database::DUCKDB_NODEJS_ERROR, ERROR) DEFINE_CONSTANT_INTEGER(
	        exports, node_guinsoodb::Database::DUCKDB_NODEJS_READONLY, OPEN_READONLY) // same as SQLite
	    DEFINE_CONSTANT_INTEGER(exports, 0, OPEN_READWRITE)                        // ignored
	    DEFINE_CONSTANT_INTEGER(exports, 0, OPEN_CREATE)                           // ignored
	    DEFINE_CONSTANT_INTEGER(exports, 0, OPEN_FULLMUTEX)                        // ignored
	    DEFINE_CONSTANT_INTEGER(exports, 0, OPEN_SHAREDCACHE)                      // ignored
	    DEFINE_CONSTANT_INTEGER(exports, 0, OPEN_PRIVATECACHE)                     // ignored
	});

	return exports;
}

} // namespace

NODE_API_MODULE(node_guinsoodb, RegisterModule);

#pragma once

#include "guinsoodb.hpp"

namespace guinsoodb {

class HTTPFsExtension : public Extension {
public:
	void Load(GuinsooDB &db) override;
};

} // namespace guinsoodb

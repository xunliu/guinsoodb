#pragma once

#include "guinsoodb.hpp"

namespace guinsoodb {

class ParquetExtension : public Extension {
public:
	void Load(GuinsooDB &db) override;
};

} // namespace guinsoodb

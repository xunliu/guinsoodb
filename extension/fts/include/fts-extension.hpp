//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// fts-extension.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb.hpp"

namespace guinsoodb {

class FTSExtension : public Extension {
public:
	void Load(GuinsooDB &db) override;
};

} // namespace guinsoodb

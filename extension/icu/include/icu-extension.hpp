//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// icu-extension.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb.hpp"

namespace guinsoodb {

class ICUExtension : public Extension {
public:
	void Load(GuinsooDB &db) override;
};

} // namespace guinsoodb

#include "guinsoodb/function/scalar/string_functions.hpp"
#include "guinsoodb/function/scalar/blob_functions.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterStringFunctions() {
	Register<ReverseFun>();
	Register<LowerFun>();
	Register<UpperFun>();
	Register<StripAccentsFun>();
	Register<ConcatFun>();
	Register<ContainsFun>();
	Register<LengthFun>();
	Register<LikeFun>();
	Register<LikeEscapeFun>();
	Register<LpadFun>();
	Register<LeftFun>();
	Register<MD5Fun>();
	Register<RightFun>();
	Register<PrintfFun>();
	Register<RegexpFun>();
	Register<SubstringFun>();
	Register<InstrFun>();
	Register<PrefixFun>();
	Register<RepeatFun>();
	Register<ReplaceFun>();
	Register<RpadFun>();
	Register<SuffixFun>();
	Register<TrimFun>();
	Register<UnicodeFun>();
	Register<NFCNormalizeFun>();
	Register<StringSplitFun>();
	Register<ASCII>();
	Register<CHR>();
	Register<MismatchesFun>();
	Register<LevenshteinFun>();
	Register<JaccardFun>();

	// blob functions
	Register<Base64Fun>();
	Register<EncodeFun>();
}

} // namespace guinsoodb

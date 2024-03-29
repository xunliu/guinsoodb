//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/tree_renderer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"
#include "guinsoodb/common/vector.hpp"
#include "guinsoodb/main/query_profiler.hpp"

namespace guinsoodb {
class LogicalOperator;
class PhysicalOperator;

struct RenderTreeNode {
	string name;
	string extra_text;
};

struct RenderTree {
	RenderTree(idx_t width, idx_t height);

	unique_ptr<unique_ptr<RenderTreeNode>[]> nodes;
	idx_t width;
	idx_t height;

public:
	RenderTreeNode *GetNode(idx_t x, idx_t y);
	void SetNode(idx_t x, idx_t y, unique_ptr<RenderTreeNode> node);
	bool HasNode(idx_t x, idx_t y);

	idx_t GetPosition(idx_t x, idx_t y);
};

struct TreeRendererConfig {

	void enable_detailed() {
		MAX_EXTRA_LINES = 1000;
		detailed = true;
	}

	void enable_standard() {
		MAX_EXTRA_LINES = 30;
		detailed = false;
	}

	idx_t MAXIMUM_RENDER_WIDTH = 240;
	idx_t NODE_RENDER_WIDTH = 29;
	idx_t MINIMUM_RENDER_WIDTH = 15;
	idx_t MAX_EXTRA_LINES = 30;
	bool detailed = false;

	const char *LTCORNER = "┌";
	const char *RTCORNER = "┐";
	const char *LDCORNER = "└";
	const char *RDCORNER = "┘";

	const char *MIDDLE = "┼";
	const char *TMIDDLE = "┬";
	const char *LMIDDLE = "├";
	const char *RMIDDLE = "┤";
	const char *DMIDDLE = "┴";

	const char *VERTICAL = "│";
	const char *HORIZONTAL = "─";

	// ASCII version?
	// static constexpr const char* LTCORNER = "<";
	// static constexpr const char* RTCORNER = ">";
	// static constexpr const char* LDCORNER = "<";
	// static constexpr const char* RDCORNER = ">";

	// static constexpr const char* MIDDLE = "+";
	// static constexpr const char* TMIDDLE = "+";
	// static constexpr const char* LMIDDLE = "+";
	// static constexpr const char* RMIDDLE = "+";
	// static constexpr const char* DMIDDLE = "+";

	// static constexpr const char* VERTICAL = "|";
	// static constexpr const char* HORIZONTAL = "-";
};

class TreeRenderer {
public:
	explicit TreeRenderer(TreeRendererConfig config_p = TreeRendererConfig()) : config(move(config_p)) {
	}

	string ToString(const LogicalOperator &op);
	string ToString(const PhysicalOperator &op);
	string ToString(const QueryProfiler::TreeNode &op);

	void Render(const LogicalOperator &op, std::ostream &ss);
	void Render(const PhysicalOperator &op, std::ostream &ss);
	void Render(const QueryProfiler::TreeNode &op, std::ostream &ss);

	void ToStream(RenderTree &root, std::ostream &ss);

	void EnableDetailed() {
		config.enable_detailed();
	}
	void EnableStandard() {
		config.enable_standard();
	}

private:
	unique_ptr<RenderTree> CreateTree(const LogicalOperator &op);
	unique_ptr<RenderTree> CreateTree(const PhysicalOperator &op);
	unique_ptr<RenderTree> CreateTree(const QueryProfiler::TreeNode &op);

	string ExtraInfoSeparator();
	unique_ptr<RenderTreeNode> CreateRenderNode(string name, string extra_info);
	unique_ptr<RenderTreeNode> CreateNode(const LogicalOperator &op);
	unique_ptr<RenderTreeNode> CreateNode(const PhysicalOperator &op);
	unique_ptr<RenderTreeNode> CreateNode(const QueryProfiler::TreeNode &op);

private:
	//! The configuration used for rendering
	TreeRendererConfig config;

private:
	void RenderTopLayer(RenderTree &root, std::ostream &ss, idx_t y);
	void RenderBoxContent(RenderTree &root, std::ostream &ss, idx_t y);
	void RenderBottomLayer(RenderTree &root, std::ostream &ss, idx_t y);

	bool CanSplitOnThisChar(char l);
	bool IsPadding(char l);
	string RemovePadding(string l);
	void SplitUpExtraInfo(const string &extra_info, vector<string> &result);
	void SplitStringBuffer(const string &source, vector<string> &result);

	template <class T>
	idx_t CreateRenderTreeRecursive(RenderTree &result, const T &op, idx_t x, idx_t y);

	template <class T>
	unique_ptr<RenderTree> CreateRenderTree(const T &op);
	string ExtractExpressionsRecursive(ExpressionInformation &states);
};

} // namespace guinsoodb
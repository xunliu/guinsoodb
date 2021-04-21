//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/transformer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"
#include "guinsoodb/common/enums/expression_type.hpp"
#include "guinsoodb/common/types.hpp"
#include "guinsoodb/common/unordered_map.hpp"
#include "guinsoodb/parser/qualified_name.hpp"
#include "guinsoodb/parser/tokens.hpp"

#include "pg_definitions.hpp"
#include "nodes/parsenodes.hpp"

namespace guinsoodb {

class ColumnDefinition;
struct OrderByNode;
struct CopyInfo;
struct CommonTableExpressionInfo;

//! The transformer class is responsible for transforming the internal Postgres
//! parser representation into the GuinsooDB representation
class Transformer {
public:
	explicit Transformer(Transformer *parent = nullptr) : parent(parent) {
	}

	//! Transforms a Postgres parse tree into a set of SQL Statements
	bool TransformParseTree(guinsoodb_libpgquery::PGList *tree, vector<unique_ptr<SQLStatement>> &statements);
	string NodetypeToString(guinsoodb_libpgquery::PGNodeTag type);

	idx_t ParamCount() {
		return parent ? parent->ParamCount() : prepared_statement_parameter_index;
	}

private:
	Transformer *parent;
	//! The current prepared statement parameter index
	idx_t prepared_statement_parameter_index = 0;
	//! Holds window expressions defined by name. We need those when transforming the expressions referring to them.
	unordered_map<string, guinsoodb_libpgquery::PGWindowDef *> window_clauses;

	void SetParamCount(idx_t new_count) {
		if (parent) {
			parent->SetParamCount(new_count);
		} else {
			this->prepared_statement_parameter_index = new_count;
		}
	}

private:
	//! Transforms a Postgres statement into a single SQL statement
	unique_ptr<SQLStatement> TransformStatement(guinsoodb_libpgquery::PGNode *stmt);
	//===--------------------------------------------------------------------===//
	// Statement transformation
	//===--------------------------------------------------------------------===//
	//! Transform a Postgres guinsoodb_libpgquery::T_PGSelectStmt node into a SelectStatement
	unique_ptr<SelectStatement> TransformSelect(guinsoodb_libpgquery::PGNode *node, bool isSelect = true);
	//! Transform a Postgres T_AlterStmt node into a AlterStatement
	unique_ptr<AlterStatement> TransformAlter(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGRenameStmt node into a RenameStatement
	unique_ptr<AlterStatement> TransformRename(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGCreateStmt node into a CreateStatement
	unique_ptr<CreateStatement> TransformCreateTable(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGCreateStmt node into a CreateStatement
	unique_ptr<CreateStatement> TransformCreateTableAs(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres node into a CreateStatement
	unique_ptr<CreateStatement> TransformCreateSchema(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGCreateSeqStmt node into a CreateStatement
	unique_ptr<CreateStatement> TransformCreateSequence(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGViewStmt node into a CreateStatement
	unique_ptr<CreateStatement> TransformCreateView(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGIndexStmt node into CreateStatement
	unique_ptr<CreateStatement> TransformCreateIndex(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGCreateFunctionStmt node into CreateStatement
	unique_ptr<CreateStatement> TransformCreateFunction(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGDropStmt node into a Drop[Table,Schema]Statement
	unique_ptr<SQLStatement> TransformDrop(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGInsertStmt node into a InsertStatement
	unique_ptr<InsertStatement> TransformInsert(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGCopyStmt node into a CopyStatement
	unique_ptr<CopyStatement> TransformCopy(guinsoodb_libpgquery::PGNode *node);
	void TransformCopyOptions(CopyInfo &info, guinsoodb_libpgquery::PGList *options);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGTransactionStmt node into a TransactionStatement
	unique_ptr<TransactionStatement> TransformTransaction(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres T_DeleteStatement node into a DeleteStatement
	unique_ptr<DeleteStatement> TransformDelete(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGUpdateStmt node into a UpdateStatement
	unique_ptr<UpdateStatement> TransformUpdate(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGPragmaStmt node into a PragmaStatement
	unique_ptr<PragmaStatement> TransformPragma(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGExportStmt node into a ExportStatement
	unique_ptr<ExportStatement> TransformExport(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres guinsoodb_libpgquery::T_PGImportStmt node into a PragmaStatement
	unique_ptr<PragmaStatement> TransformImport(guinsoodb_libpgquery::PGNode *node);
	unique_ptr<ExplainStatement> TransformExplain(guinsoodb_libpgquery::PGNode *node);
	unique_ptr<VacuumStatement> TransformVacuum(guinsoodb_libpgquery::PGNode *node);
	unique_ptr<PragmaStatement> TransformShow(guinsoodb_libpgquery::PGNode *node);
	unique_ptr<ShowStatement> TransformShowSelect(guinsoodb_libpgquery::PGNode *node);

	unique_ptr<PrepareStatement> TransformPrepare(guinsoodb_libpgquery::PGNode *node);
	unique_ptr<ExecuteStatement> TransformExecute(guinsoodb_libpgquery::PGNode *node);
	unique_ptr<CallStatement> TransformCall(guinsoodb_libpgquery::PGNode *node);
	unique_ptr<DropStatement> TransformDeallocate(guinsoodb_libpgquery::PGNode *node);
	unique_ptr<SetStatement> TransformSet(guinsoodb_libpgquery::PGNode *node);
	unique_ptr<SQLStatement> TransformCheckpoint(guinsoodb_libpgquery::PGNode *node);
	unique_ptr<LoadStatement> TransformLoad(guinsoodb_libpgquery::PGNode *node);

	//===--------------------------------------------------------------------===//
	// Query Node Transform
	//===--------------------------------------------------------------------===//
	//! Transform a Postgres guinsoodb_libpgquery::T_PGSelectStmt node into a QueryNode
	unique_ptr<QueryNode> TransformSelectNode(guinsoodb_libpgquery::PGSelectStmt *node);

	//===--------------------------------------------------------------------===//
	// Expression Transform
	//===--------------------------------------------------------------------===//
	//! Transform a Postgres boolean expression into an Expression
	unique_ptr<ParsedExpression> TransformBoolExpr(guinsoodb_libpgquery::PGBoolExpr *root);
	//! Transform a Postgres case expression into an Expression
	unique_ptr<ParsedExpression> TransformCase(guinsoodb_libpgquery::PGCaseExpr *root);
	//! Transform a Postgres type cast into an Expression
	unique_ptr<ParsedExpression> TransformTypeCast(guinsoodb_libpgquery::PGTypeCast *root);
	//! Transform a Postgres coalesce into an Expression
	unique_ptr<ParsedExpression> TransformCoalesce(guinsoodb_libpgquery::PGAExpr *root);
	//! Transform a Postgres column reference into an Expression
	unique_ptr<ParsedExpression> TransformColumnRef(guinsoodb_libpgquery::PGColumnRef *root);
	//! Transform a Postgres constant value into an Expression
	unique_ptr<ConstantExpression> TransformValue(guinsoodb_libpgquery::PGValue val);
	//! Transform a Postgres operator into an Expression
	unique_ptr<ParsedExpression> TransformAExpr(guinsoodb_libpgquery::PGAExpr *root);
	//! Transform a Postgres abstract expression into an Expression
	unique_ptr<ParsedExpression> TransformExpression(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres function call into an Expression
	unique_ptr<ParsedExpression> TransformFuncCall(guinsoodb_libpgquery::PGFuncCall *root);
	//! Transform a Postgres boolean expression into an Expression
	unique_ptr<ParsedExpression> TransformInterval(guinsoodb_libpgquery::PGIntervalConstant *root);
	//! Transform a Postgres lambda node [e.g. (x, y) -> x + y] into a lambda expression
	unique_ptr<ParsedExpression> TransformLambda(guinsoodb_libpgquery::PGLambdaFunction *node);
	//! Transform a Postgres array access node (e.g. x[1] or x[1:3])
	unique_ptr<ParsedExpression> TransformArrayAccess(guinsoodb_libpgquery::PGAIndirection *node);
	//! Transform a positional reference (e.g. #1)
	unique_ptr<ParsedExpression> TransformPositionalReference(guinsoodb_libpgquery::PGPositionalReference *node);

	//! Transform a Postgres constant value into an Expression
	unique_ptr<ParsedExpression> TransformConstant(guinsoodb_libpgquery::PGAConst *c);

	unique_ptr<ParsedExpression> TransformResTarget(guinsoodb_libpgquery::PGResTarget *root);
	unique_ptr<ParsedExpression> TransformNullTest(guinsoodb_libpgquery::PGNullTest *root);
	unique_ptr<ParsedExpression> TransformParamRef(guinsoodb_libpgquery::PGParamRef *node);
	unique_ptr<ParsedExpression> TransformNamedArg(guinsoodb_libpgquery::PGNamedArgExpr *root);

	unique_ptr<ParsedExpression> TransformSQLValueFunction(guinsoodb_libpgquery::PGSQLValueFunction *node);

	unique_ptr<ParsedExpression> TransformSubquery(guinsoodb_libpgquery::PGSubLink *root);
	//===--------------------------------------------------------------------===//
	// Constraints transform
	//===--------------------------------------------------------------------===//
	unique_ptr<Constraint> TransformConstraint(guinsoodb_libpgquery::PGListCell *cell);

	unique_ptr<Constraint> TransformConstraint(guinsoodb_libpgquery::PGListCell *cell, ColumnDefinition &column,
	                                           idx_t index);

	//===--------------------------------------------------------------------===//
	// Collation transform
	//===--------------------------------------------------------------------===//
	unique_ptr<ParsedExpression> TransformCollateExpr(guinsoodb_libpgquery::PGCollateClause *collate);

	string TransformCollation(guinsoodb_libpgquery::PGCollateClause *collate);

	ColumnDefinition TransformColumnDefinition(guinsoodb_libpgquery::PGColumnDef *cdef);
	//===--------------------------------------------------------------------===//
	// Helpers
	//===--------------------------------------------------------------------===//
	string TransformAlias(guinsoodb_libpgquery::PGAlias *root, vector<string> &column_name_alias);
	void TransformCTE(guinsoodb_libpgquery::PGWithClause *de_with_clause, QueryNode &select);
	unique_ptr<SelectStatement> TransformRecursiveCTE(guinsoodb_libpgquery::PGCommonTableExpr *node,
	                                                  CommonTableExpressionInfo &info);
	// Operator String to ExpressionType (e.g. + => OPERATOR_ADD)
	ExpressionType OperatorToExpressionType(const string &op);

	unique_ptr<ParsedExpression> TransformUnaryOperator(const string &op, unique_ptr<ParsedExpression> child);
	unique_ptr<ParsedExpression> TransformBinaryOperator(const string &op, unique_ptr<ParsedExpression> left,
	                                                     unique_ptr<ParsedExpression> right);
	//===--------------------------------------------------------------------===//
	// TableRef transform
	//===--------------------------------------------------------------------===//
	//! Transform a Postgres node into a TableRef
	unique_ptr<TableRef> TransformTableRefNode(guinsoodb_libpgquery::PGNode *node);
	//! Transform a Postgres FROM clause into a TableRef
	unique_ptr<TableRef> TransformFrom(guinsoodb_libpgquery::PGList *root);
	//! Transform a Postgres table reference into a TableRef
	unique_ptr<TableRef> TransformRangeVar(guinsoodb_libpgquery::PGRangeVar *root);
	//! Transform a Postgres table-producing function into a TableRef
	unique_ptr<TableRef> TransformRangeFunction(guinsoodb_libpgquery::PGRangeFunction *root);
	//! Transform a Postgres join node into a TableRef
	unique_ptr<TableRef> TransformJoin(guinsoodb_libpgquery::PGJoinExpr *root);
	//! Transform a table producing subquery into a TableRef
	unique_ptr<TableRef> TransformRangeSubselect(guinsoodb_libpgquery::PGRangeSubselect *root);
	//! Transform a VALUES list into a set of expressions
	unique_ptr<TableRef> TransformValuesList(guinsoodb_libpgquery::PGList *list);

	//! Transform a range var into a (schema) qualified name
	QualifiedName TransformQualifiedName(guinsoodb_libpgquery::PGRangeVar *root);

	//! Transform a Postgres TypeName string into a LogicalType
	LogicalType TransformTypeName(guinsoodb_libpgquery::PGTypeName *name);

	//! Transform a Postgres GROUP BY expression into a list of Expression
	bool TransformGroupBy(guinsoodb_libpgquery::PGList *group, vector<unique_ptr<ParsedExpression>> &result);
	//! Transform a Postgres ORDER BY expression into an OrderByDescription
	bool TransformOrderBy(guinsoodb_libpgquery::PGList *order, vector<OrderByNode> &result);

	//! Transform a Postgres SELECT clause into a list of Expressions
	bool TransformExpressionList(guinsoodb_libpgquery::PGList *list, vector<unique_ptr<ParsedExpression>> &result);

	//! Transform a Postgres PARTITION BY/ORDER BY specification into lists of expressions
	void TransformWindowDef(guinsoodb_libpgquery::PGWindowDef *window_spec, WindowExpression *expr);
	//! Transform a Postgres window frame specification into frame expressions
	void TransformWindowFrame(guinsoodb_libpgquery::PGWindowDef *window_spec, WindowExpression *expr);

	unique_ptr<SampleOptions> TransformSampleOptions(guinsoodb_libpgquery::PGNode *options);
};

} // namespace guinsoodb

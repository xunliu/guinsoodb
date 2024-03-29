#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

std::string Transformer::NodetypeToString(guinsoodb_libpgquery::PGNodeTag type) {
	switch (type) {
	case guinsoodb_libpgquery::T_PGInvalid:
		return "T_Invalid";
	case guinsoodb_libpgquery::T_PGIndexInfo:
		return "T_IndexInfo";
	case guinsoodb_libpgquery::T_PGExprContext:
		return "T_ExprContext";
	case guinsoodb_libpgquery::T_PGProjectionInfo:
		return "T_ProjectionInfo";
	case guinsoodb_libpgquery::T_PGJunkFilter:
		return "T_JunkFilter";
	case guinsoodb_libpgquery::T_PGResultRelInfo:
		return "T_ResultRelInfo";
	case guinsoodb_libpgquery::T_PGEState:
		return "T_EState";
	case guinsoodb_libpgquery::T_PGTupleTableSlot:
		return "T_TupleTableSlot";
	case guinsoodb_libpgquery::T_PGPlan:
		return "T_Plan";
	case guinsoodb_libpgquery::T_PGResult:
		return "T_Result";
	case guinsoodb_libpgquery::T_PGProjectSet:
		return "T_ProjectSet";
	case guinsoodb_libpgquery::T_PGModifyTable:
		return "T_ModifyTable";
	case guinsoodb_libpgquery::T_PGAppend:
		return "T_Append";
	case guinsoodb_libpgquery::T_PGMergeAppend:
		return "T_MergeAppend";
	case guinsoodb_libpgquery::T_PGRecursiveUnion:
		return "T_RecursiveUnion";
	case guinsoodb_libpgquery::T_PGBitmapAnd:
		return "T_BitmapAnd";
	case guinsoodb_libpgquery::T_PGBitmapOr:
		return "T_BitmapOr";
	case guinsoodb_libpgquery::T_PGScan:
		return "T_Scan";
	case guinsoodb_libpgquery::T_PGSeqScan:
		return "T_SeqScan";
	case guinsoodb_libpgquery::T_PGSampleScan:
		return "T_SampleScan";
	case guinsoodb_libpgquery::T_PGIndexScan:
		return "T_IndexScan";
	case guinsoodb_libpgquery::T_PGIndexOnlyScan:
		return "T_IndexOnlyScan";
	case guinsoodb_libpgquery::T_PGBitmapIndexScan:
		return "T_BitmapIndexScan";
	case guinsoodb_libpgquery::T_PGBitmapHeapScan:
		return "T_BitmapHeapScan";
	case guinsoodb_libpgquery::T_PGTidScan:
		return "T_TidScan";
	case guinsoodb_libpgquery::T_PGSubqueryScan:
		return "T_SubqueryScan";
	case guinsoodb_libpgquery::T_PGFunctionScan:
		return "T_FunctionScan";
	case guinsoodb_libpgquery::T_PGValuesScan:
		return "T_ValuesScan";
	case guinsoodb_libpgquery::T_PGTableFuncScan:
		return "T_TableFuncScan";
	case guinsoodb_libpgquery::T_PGCteScan:
		return "T_CteScan";
	case guinsoodb_libpgquery::T_PGNamedTuplestoreScan:
		return "T_NamedTuplestoreScan";
	case guinsoodb_libpgquery::T_PGWorkTableScan:
		return "T_WorkTableScan";
	case guinsoodb_libpgquery::T_PGForeignScan:
		return "T_ForeignScan";
	case guinsoodb_libpgquery::T_PGCustomScan:
		return "T_CustomScan";
	case guinsoodb_libpgquery::T_PGJoin:
		return "T_Join";
	case guinsoodb_libpgquery::T_PGNestLoop:
		return "T_NestLoop";
	case guinsoodb_libpgquery::T_PGMergeJoin:
		return "T_MergeJoin";
	case guinsoodb_libpgquery::T_PGHashJoin:
		return "T_HashJoin";
	case guinsoodb_libpgquery::T_PGMaterial:
		return "T_Material";
	case guinsoodb_libpgquery::T_PGSort:
		return "T_Sort";
	case guinsoodb_libpgquery::T_PGGroup:
		return "T_Group";
	case guinsoodb_libpgquery::T_PGAgg:
		return "T_Agg";
	case guinsoodb_libpgquery::T_PGWindowAgg:
		return "T_WindowAgg";
	case guinsoodb_libpgquery::T_PGUnique:
		return "T_Unique";
	case guinsoodb_libpgquery::T_PGGather:
		return "T_Gather";
	case guinsoodb_libpgquery::T_PGGatherMerge:
		return "T_GatherMerge";
	case guinsoodb_libpgquery::T_PGHash:
		return "T_Hash";
	case guinsoodb_libpgquery::T_PGSetOp:
		return "T_SetOp";
	case guinsoodb_libpgquery::T_PGLockRows:
		return "T_LockRows";
	case guinsoodb_libpgquery::T_PGLimit:
		return "T_Limit";
	case guinsoodb_libpgquery::T_PGNestLoopParam:
		return "T_NestLoopParam";
	case guinsoodb_libpgquery::T_PGPlanRowMark:
		return "T_PlanRowMark";
	case guinsoodb_libpgquery::T_PGPlanInvalItem:
		return "T_PlanInvalItem";
	case guinsoodb_libpgquery::T_PGPlanState:
		return "T_PlanState";
	case guinsoodb_libpgquery::T_PGResultState:
		return "T_ResultState";
	case guinsoodb_libpgquery::T_PGProjectSetState:
		return "T_ProjectSetState";
	case guinsoodb_libpgquery::T_PGModifyTableState:
		return "T_ModifyTableState";
	case guinsoodb_libpgquery::T_PGAppendState:
		return "T_AppendState";
	case guinsoodb_libpgquery::T_PGMergeAppendState:
		return "T_MergeAppendState";
	case guinsoodb_libpgquery::T_PGRecursiveUnionState:
		return "T_RecursiveUnionState";
	case guinsoodb_libpgquery::T_PGBitmapAndState:
		return "T_BitmapAndState";
	case guinsoodb_libpgquery::T_PGBitmapOrState:
		return "T_BitmapOrState";
	case guinsoodb_libpgquery::T_PGScanState:
		return "T_ScanState";
	case guinsoodb_libpgquery::T_PGSeqScanState:
		return "T_SeqScanState";
	case guinsoodb_libpgquery::T_PGSampleScanState:
		return "T_SampleScanState";
	case guinsoodb_libpgquery::T_PGIndexScanState:
		return "T_IndexScanState";
	case guinsoodb_libpgquery::T_PGIndexOnlyScanState:
		return "T_IndexOnlyScanState";
	case guinsoodb_libpgquery::T_PGBitmapIndexScanState:
		return "T_BitmapIndexScanState";
	case guinsoodb_libpgquery::T_PGBitmapHeapScanState:
		return "T_BitmapHeapScanState";
	case guinsoodb_libpgquery::T_PGTidScanState:
		return "T_TidScanState";
	case guinsoodb_libpgquery::T_PGSubqueryScanState:
		return "T_SubqueryScanState";
	case guinsoodb_libpgquery::T_PGFunctionScanState:
		return "T_FunctionScanState";
	case guinsoodb_libpgquery::T_PGTableFuncScanState:
		return "T_TableFuncScanState";
	case guinsoodb_libpgquery::T_PGValuesScanState:
		return "T_ValuesScanState";
	case guinsoodb_libpgquery::T_PGCteScanState:
		return "T_CteScanState";
	case guinsoodb_libpgquery::T_PGNamedTuplestoreScanState:
		return "T_NamedTuplestoreScanState";
	case guinsoodb_libpgquery::T_PGWorkTableScanState:
		return "T_WorkTableScanState";
	case guinsoodb_libpgquery::T_PGForeignScanState:
		return "T_ForeignScanState";
	case guinsoodb_libpgquery::T_PGCustomScanState:
		return "T_CustomScanState";
	case guinsoodb_libpgquery::T_PGJoinState:
		return "T_JoinState";
	case guinsoodb_libpgquery::T_PGNestLoopState:
		return "T_NestLoopState";
	case guinsoodb_libpgquery::T_PGMergeJoinState:
		return "T_MergeJoinState";
	case guinsoodb_libpgquery::T_PGHashJoinState:
		return "T_HashJoinState";
	case guinsoodb_libpgquery::T_PGMaterialState:
		return "T_MaterialState";
	case guinsoodb_libpgquery::T_PGSortState:
		return "T_SortState";
	case guinsoodb_libpgquery::T_PGGroupState:
		return "T_GroupState";
	case guinsoodb_libpgquery::T_PGAggState:
		return "T_AggState";
	case guinsoodb_libpgquery::T_PGWindowAggState:
		return "T_WindowAggState";
	case guinsoodb_libpgquery::T_PGUniqueState:
		return "T_UniqueState";
	case guinsoodb_libpgquery::T_PGGatherState:
		return "T_GatherState";
	case guinsoodb_libpgquery::T_PGGatherMergeState:
		return "T_GatherMergeState";
	case guinsoodb_libpgquery::T_PGHashState:
		return "T_HashState";
	case guinsoodb_libpgquery::T_PGSetOpState:
		return "T_SetOpState";
	case guinsoodb_libpgquery::T_PGLockRowsState:
		return "T_LockRowsState";
	case guinsoodb_libpgquery::T_PGLimitState:
		return "T_LimitState";
	case guinsoodb_libpgquery::T_PGAlias:
		return "T_Alias";
	case guinsoodb_libpgquery::T_PGRangeVar:
		return "T_RangeVar";
	case guinsoodb_libpgquery::T_PGTableFunc:
		return "T_TableFunc";
	case guinsoodb_libpgquery::T_PGExpr:
		return "T_Expr";
	case guinsoodb_libpgquery::T_PGVar:
		return "T_Var";
	case guinsoodb_libpgquery::T_PGConst:
		return "T_Const";
	case guinsoodb_libpgquery::T_PGParam:
		return "T_Param";
	case guinsoodb_libpgquery::T_PGAggref:
		return "T_Aggref";
	case guinsoodb_libpgquery::T_PGGroupingFunc:
		return "T_GroupingFunc";
	case guinsoodb_libpgquery::T_PGWindowFunc:
		return "T_WindowFunc";
	case guinsoodb_libpgquery::T_PGArrayRef:
		return "T_ArrayRef";
	case guinsoodb_libpgquery::T_PGFuncExpr:
		return "T_FuncExpr";
	case guinsoodb_libpgquery::T_PGNamedArgExpr:
		return "T_NamedArgExpr";
	case guinsoodb_libpgquery::T_PGOpExpr:
		return "T_OpExpr";
	case guinsoodb_libpgquery::T_PGDistinctExpr:
		return "T_DistinctExpr";
	case guinsoodb_libpgquery::T_PGNullIfExpr:
		return "T_NullIfExpr";
	case guinsoodb_libpgquery::T_PGScalarArrayOpExpr:
		return "T_ScalarArrayOpExpr";
	case guinsoodb_libpgquery::T_PGBoolExpr:
		return "T_BoolExpr";
	case guinsoodb_libpgquery::T_PGSubLink:
		return "T_SubLink";
	case guinsoodb_libpgquery::T_PGSubPlan:
		return "T_SubPlan";
	case guinsoodb_libpgquery::T_PGAlternativeSubPlan:
		return "T_AlternativeSubPlan";
	case guinsoodb_libpgquery::T_PGFieldSelect:
		return "T_FieldSelect";
	case guinsoodb_libpgquery::T_PGFieldStore:
		return "T_FieldStore";
	case guinsoodb_libpgquery::T_PGRelabelType:
		return "T_RelabelType";
	case guinsoodb_libpgquery::T_PGCoerceViaIO:
		return "T_CoerceViaIO";
	case guinsoodb_libpgquery::T_PGArrayCoerceExpr:
		return "T_ArrayCoerceExpr";
	case guinsoodb_libpgquery::T_PGConvertRowtypeExpr:
		return "T_ConvertRowtypeExpr";
	case guinsoodb_libpgquery::T_PGCollateExpr:
		return "T_CollateExpr";
	case guinsoodb_libpgquery::T_PGCaseExpr:
		return "T_CaseExpr";
	case guinsoodb_libpgquery::T_PGCaseWhen:
		return "T_CaseWhen";
	case guinsoodb_libpgquery::T_PGCaseTestExpr:
		return "T_CaseTestExpr";
	case guinsoodb_libpgquery::T_PGArrayExpr:
		return "T_ArrayExpr";
	case guinsoodb_libpgquery::T_PGRowExpr:
		return "T_RowExpr";
	case guinsoodb_libpgquery::T_PGRowCompareExpr:
		return "T_RowCompareExpr";
	case guinsoodb_libpgquery::T_PGCoalesceExpr:
		return "T_CoalesceExpr";
	case guinsoodb_libpgquery::T_PGMinMaxExpr:
		return "T_MinMaxExpr";
	case guinsoodb_libpgquery::T_PGSQLValueFunction:
		return "T_SQLValueFunction";
	case guinsoodb_libpgquery::T_PGXmlExpr:
		return "T_XmlExpr";
	case guinsoodb_libpgquery::T_PGNullTest:
		return "T_NullTest";
	case guinsoodb_libpgquery::T_PGBooleanTest:
		return "T_BooleanTest";
	case guinsoodb_libpgquery::T_PGCoerceToDomain:
		return "T_CoerceToDomain";
	case guinsoodb_libpgquery::T_PGCoerceToDomainValue:
		return "T_CoerceToDomainValue";
	case guinsoodb_libpgquery::T_PGSetToDefault:
		return "T_SetToDefault";
	case guinsoodb_libpgquery::T_PGCurrentOfExpr:
		return "T_CurrentOfExpr";
	case guinsoodb_libpgquery::T_PGNextValueExpr:
		return "T_NextValueExpr";
	case guinsoodb_libpgquery::T_PGInferenceElem:
		return "T_InferenceElem";
	case guinsoodb_libpgquery::T_PGTargetEntry:
		return "T_TargetEntry";
	case guinsoodb_libpgquery::T_PGRangeTblRef:
		return "T_RangeTblRef";
	case guinsoodb_libpgquery::T_PGJoinExpr:
		return "T_JoinExpr";
	case guinsoodb_libpgquery::T_PGFromExpr:
		return "T_FromExpr";
	case guinsoodb_libpgquery::T_PGOnConflictExpr:
		return "T_OnConflictExpr";
	case guinsoodb_libpgquery::T_PGIntoClause:
		return "T_IntoClause";
	case guinsoodb_libpgquery::T_PGExprState:
		return "T_ExprState";
	case guinsoodb_libpgquery::T_PGAggrefExprState:
		return "T_AggrefExprState";
	case guinsoodb_libpgquery::T_PGWindowFuncExprState:
		return "T_WindowFuncExprState";
	case guinsoodb_libpgquery::T_PGSetExprState:
		return "T_SetExprState";
	case guinsoodb_libpgquery::T_PGSubPlanState:
		return "T_SubPlanState";
	case guinsoodb_libpgquery::T_PGAlternativeSubPlanState:
		return "T_AlternativeSubPlanState";
	case guinsoodb_libpgquery::T_PGDomainConstraintState:
		return "T_DomainConstraintState";
	case guinsoodb_libpgquery::T_PGPlannerInfo:
		return "T_PlannerInfo";
	case guinsoodb_libpgquery::T_PGPlannerGlobal:
		return "T_PlannerGlobal";
	case guinsoodb_libpgquery::T_PGRelOptInfo:
		return "T_RelOptInfo";
	case guinsoodb_libpgquery::T_PGIndexOptInfo:
		return "T_IndexOptInfo";
	case guinsoodb_libpgquery::T_PGForeignKeyOptInfo:
		return "T_ForeignKeyOptInfo";
	case guinsoodb_libpgquery::T_PGParamPathInfo:
		return "T_ParamPathInfo";
	case guinsoodb_libpgquery::T_PGPath:
		return "T_Path";
	case guinsoodb_libpgquery::T_PGIndexPath:
		return "T_IndexPath";
	case guinsoodb_libpgquery::T_PGBitmapHeapPath:
		return "T_BitmapHeapPath";
	case guinsoodb_libpgquery::T_PGBitmapAndPath:
		return "T_BitmapAndPath";
	case guinsoodb_libpgquery::T_PGBitmapOrPath:
		return "T_BitmapOrPath";
	case guinsoodb_libpgquery::T_PGTidPath:
		return "T_TidPath";
	case guinsoodb_libpgquery::T_PGSubqueryScanPath:
		return "T_SubqueryScanPath";
	case guinsoodb_libpgquery::T_PGForeignPath:
		return "T_ForeignPath";
	case guinsoodb_libpgquery::T_PGCustomPath:
		return "T_CustomPath";
	case guinsoodb_libpgquery::T_PGNestPath:
		return "T_NestPath";
	case guinsoodb_libpgquery::T_PGMergePath:
		return "T_MergePath";
	case guinsoodb_libpgquery::T_PGHashPath:
		return "T_HashPath";
	case guinsoodb_libpgquery::T_PGAppendPath:
		return "T_AppendPath";
	case guinsoodb_libpgquery::T_PGMergeAppendPath:
		return "T_MergeAppendPath";
	case guinsoodb_libpgquery::T_PGResultPath:
		return "T_ResultPath";
	case guinsoodb_libpgquery::T_PGMaterialPath:
		return "T_MaterialPath";
	case guinsoodb_libpgquery::T_PGUniquePath:
		return "T_UniquePath";
	case guinsoodb_libpgquery::T_PGGatherPath:
		return "T_GatherPath";
	case guinsoodb_libpgquery::T_PGGatherMergePath:
		return "T_GatherMergePath";
	case guinsoodb_libpgquery::T_PGProjectionPath:
		return "T_ProjectionPath";
	case guinsoodb_libpgquery::T_PGProjectSetPath:
		return "T_ProjectSetPath";
	case guinsoodb_libpgquery::T_PGSortPath:
		return "T_SortPath";
	case guinsoodb_libpgquery::T_PGGroupPath:
		return "T_GroupPath";
	case guinsoodb_libpgquery::T_PGUpperUniquePath:
		return "T_UpperUniquePath";
	case guinsoodb_libpgquery::T_PGAggPath:
		return "T_AggPath";
	case guinsoodb_libpgquery::T_PGGroupingSetsPath:
		return "T_GroupingSetsPath";
	case guinsoodb_libpgquery::T_PGMinMaxAggPath:
		return "T_MinMaxAggPath";
	case guinsoodb_libpgquery::T_PGWindowAggPath:
		return "T_WindowAggPath";
	case guinsoodb_libpgquery::T_PGSetOpPath:
		return "T_SetOpPath";
	case guinsoodb_libpgquery::T_PGRecursiveUnionPath:
		return "T_RecursiveUnionPath";
	case guinsoodb_libpgquery::T_PGLockRowsPath:
		return "T_LockRowsPath";
	case guinsoodb_libpgquery::T_PGModifyTablePath:
		return "T_ModifyTablePath";
	case guinsoodb_libpgquery::T_PGLimitPath:
		return "T_LimitPath";
	case guinsoodb_libpgquery::T_PGEquivalenceClass:
		return "T_EquivalenceClass";
	case guinsoodb_libpgquery::T_PGEquivalenceMember:
		return "T_EquivalenceMember";
	case guinsoodb_libpgquery::T_PGPathKey:
		return "T_PathKey";
	case guinsoodb_libpgquery::T_PGPathTarget:
		return "T_PathTarget";
	case guinsoodb_libpgquery::T_PGRestrictInfo:
		return "T_RestrictInfo";
	case guinsoodb_libpgquery::T_PGPlaceHolderVar:
		return "T_PlaceHolderVar";
	case guinsoodb_libpgquery::T_PGSpecialJoinInfo:
		return "T_SpecialJoinInfo";
	case guinsoodb_libpgquery::T_PGAppendRelInfo:
		return "T_AppendRelInfo";
	case guinsoodb_libpgquery::T_PGPartitionedChildRelInfo:
		return "T_PartitionedChildRelInfo";
	case guinsoodb_libpgquery::T_PGPlaceHolderInfo:
		return "T_PlaceHolderInfo";
	case guinsoodb_libpgquery::T_PGMinMaxAggInfo:
		return "T_MinMaxAggInfo";
	case guinsoodb_libpgquery::T_PGPlannerParamItem:
		return "T_PlannerParamItem";
	case guinsoodb_libpgquery::T_PGRollupData:
		return "T_RollupData";
	case guinsoodb_libpgquery::T_PGGroupingSetData:
		return "T_GroupingSetData";
	case guinsoodb_libpgquery::T_PGStatisticExtInfo:
		return "T_StatisticExtInfo";
	case guinsoodb_libpgquery::T_PGMemoryContext:
		return "T_MemoryContext";
	case guinsoodb_libpgquery::T_PGAllocSetContext:
		return "T_AllocSetContext";
	case guinsoodb_libpgquery::T_PGSlabContext:
		return "T_SlabContext";
	case guinsoodb_libpgquery::T_PGValue:
		return "T_Value";
	case guinsoodb_libpgquery::T_PGInteger:
		return "T_Integer";
	case guinsoodb_libpgquery::T_PGFloat:
		return "T_Float";
	case guinsoodb_libpgquery::T_PGString:
		return "T_String";
	case guinsoodb_libpgquery::T_PGBitString:
		return "T_BitString";
	case guinsoodb_libpgquery::T_PGNull:
		return "T_Null";
	case guinsoodb_libpgquery::T_PGList:
		return "T_List";
	case guinsoodb_libpgquery::T_PGIntList:
		return "T_IntList";
	case guinsoodb_libpgquery::T_PGOidList:
		return "T_OidList";
	case guinsoodb_libpgquery::T_PGExtensibleNode:
		return "T_ExtensibleNode";
	case guinsoodb_libpgquery::T_PGRawStmt:
		return "T_RawStmt";
	case guinsoodb_libpgquery::T_PGQuery:
		return "T_Query";
	case guinsoodb_libpgquery::T_PGPlannedStmt:
		return "T_PlannedStmt";
	case guinsoodb_libpgquery::T_PGInsertStmt:
		return "T_InsertStmt";
	case guinsoodb_libpgquery::T_PGDeleteStmt:
		return "T_DeleteStmt";
	case guinsoodb_libpgquery::T_PGUpdateStmt:
		return "T_UpdateStmt";
	case guinsoodb_libpgquery::T_PGSelectStmt:
		return "T_SelectStmt";
	case guinsoodb_libpgquery::T_PGAlterTableStmt:
		return "T_AlterTableStmt";
	case guinsoodb_libpgquery::T_PGAlterTableCmd:
		return "T_AlterTableCmd";
	case guinsoodb_libpgquery::T_PGAlterDomainStmt:
		return "T_AlterDomainStmt";
	case guinsoodb_libpgquery::T_PGSetOperationStmt:
		return "T_SetOperationStmt";
	case guinsoodb_libpgquery::T_PGGrantStmt:
		return "T_GrantStmt";
	case guinsoodb_libpgquery::T_PGGrantRoleStmt:
		return "T_GrantRoleStmt";
	case guinsoodb_libpgquery::T_PGAlterDefaultPrivilegesStmt:
		return "T_AlterDefaultPrivilegesStmt";
	case guinsoodb_libpgquery::T_PGClosePortalStmt:
		return "T_ClosePortalStmt";
	case guinsoodb_libpgquery::T_PGClusterStmt:
		return "T_ClusterStmt";
	case guinsoodb_libpgquery::T_PGCopyStmt:
		return "T_CopyStmt";
	case guinsoodb_libpgquery::T_PGCreateStmt:
		return "T_CreateStmt";
	case guinsoodb_libpgquery::T_PGDefineStmt:
		return "T_DefineStmt";
	case guinsoodb_libpgquery::T_PGDropStmt:
		return "T_DropStmt";
	case guinsoodb_libpgquery::T_PGTruncateStmt:
		return "T_TruncateStmt";
	case guinsoodb_libpgquery::T_PGCommentStmt:
		return "T_CommentStmt";
	case guinsoodb_libpgquery::T_PGFetchStmt:
		return "T_FetchStmt";
	case guinsoodb_libpgquery::T_PGIndexStmt:
		return "T_IndexStmt";
	case guinsoodb_libpgquery::T_PGCreateFunctionStmt:
		return "T_CreateFunctionStmt";
	case guinsoodb_libpgquery::T_PGAlterFunctionStmt:
		return "T_AlterFunctionStmt";
	case guinsoodb_libpgquery::T_PGDoStmt:
		return "T_DoStmt";
	case guinsoodb_libpgquery::T_PGRenameStmt:
		return "T_RenameStmt";
	case guinsoodb_libpgquery::T_PGRuleStmt:
		return "T_RuleStmt";
	case guinsoodb_libpgquery::T_PGNotifyStmt:
		return "T_NotifyStmt";
	case guinsoodb_libpgquery::T_PGListenStmt:
		return "T_ListenStmt";
	case guinsoodb_libpgquery::T_PGUnlistenStmt:
		return "T_UnlistenStmt";
	case guinsoodb_libpgquery::T_PGTransactionStmt:
		return "T_TransactionStmt";
	case guinsoodb_libpgquery::T_PGViewStmt:
		return "T_ViewStmt";
	case guinsoodb_libpgquery::T_PGLoadStmt:
		return "T_LoadStmt";
	case guinsoodb_libpgquery::T_PGCreateDomainStmt:
		return "T_CreateDomainStmt";
	case guinsoodb_libpgquery::T_PGCreatedbStmt:
		return "T_CreatedbStmt";
	case guinsoodb_libpgquery::T_PGDropdbStmt:
		return "T_DropdbStmt";
	case guinsoodb_libpgquery::T_PGVacuumStmt:
		return "T_VacuumStmt";
	case guinsoodb_libpgquery::T_PGExplainStmt:
		return "T_ExplainStmt";
	case guinsoodb_libpgquery::T_PGCreateTableAsStmt:
		return "T_CreateTableAsStmt";
	case guinsoodb_libpgquery::T_PGCreateSeqStmt:
		return "T_CreateSeqStmt";
	case guinsoodb_libpgquery::T_PGAlterSeqStmt:
		return "T_AlterSeqStmt";
	case guinsoodb_libpgquery::T_PGVariableSetStmt:
		return "T_VariableSetStmt";
	case guinsoodb_libpgquery::T_PGVariableShowStmt:
		return "T_VariableShowStmt";
	case guinsoodb_libpgquery::T_PGVariableShowSelectStmt:
		return "T_VariableShowSelectStmt";
	case guinsoodb_libpgquery::T_PGDiscardStmt:
		return "T_DiscardStmt";
	case guinsoodb_libpgquery::T_PGCreateTrigStmt:
		return "T_CreateTrigStmt";
	case guinsoodb_libpgquery::T_PGCreatePLangStmt:
		return "T_CreatePLangStmt";
	case guinsoodb_libpgquery::T_PGCreateRoleStmt:
		return "T_CreateRoleStmt";
	case guinsoodb_libpgquery::T_PGAlterRoleStmt:
		return "T_AlterRoleStmt";
	case guinsoodb_libpgquery::T_PGDropRoleStmt:
		return "T_DropRoleStmt";
	case guinsoodb_libpgquery::T_PGLockStmt:
		return "T_LockStmt";
	case guinsoodb_libpgquery::T_PGConstraintsSetStmt:
		return "T_ConstraintsSetStmt";
	case guinsoodb_libpgquery::T_PGReindexStmt:
		return "T_ReindexStmt";
	case guinsoodb_libpgquery::T_PGCheckPointStmt:
		return "T_CheckPointStmt";
	case guinsoodb_libpgquery::T_PGCreateSchemaStmt:
		return "T_CreateSchemaStmt";
	case guinsoodb_libpgquery::T_PGAlterDatabaseStmt:
		return "T_AlterDatabaseStmt";
	case guinsoodb_libpgquery::T_PGAlterDatabaseSetStmt:
		return "T_AlterDatabaseSetStmt";
	case guinsoodb_libpgquery::T_PGAlterRoleSetStmt:
		return "T_AlterRoleSetStmt";
	case guinsoodb_libpgquery::T_PGCreateConversionStmt:
		return "T_CreateConversionStmt";
	case guinsoodb_libpgquery::T_PGCreateCastStmt:
		return "T_CreateCastStmt";
	case guinsoodb_libpgquery::T_PGCreateOpClassStmt:
		return "T_CreateOpClassStmt";
	case guinsoodb_libpgquery::T_PGCreateOpFamilyStmt:
		return "T_CreateOpFamilyStmt";
	case guinsoodb_libpgquery::T_PGAlterOpFamilyStmt:
		return "T_AlterOpFamilyStmt";
	case guinsoodb_libpgquery::T_PGPrepareStmt:
		return "T_PrepareStmt";
	case guinsoodb_libpgquery::T_PGExecuteStmt:
		return "T_ExecuteStmt";
	case guinsoodb_libpgquery::T_PGCallStmt:
		return "T_CallStmt";
	case guinsoodb_libpgquery::T_PGDeallocateStmt:
		return "T_DeallocateStmt";
	case guinsoodb_libpgquery::T_PGDeclareCursorStmt:
		return "T_DeclareCursorStmt";
	case guinsoodb_libpgquery::T_PGCreateTableSpaceStmt:
		return "T_CreateTableSpaceStmt";
	case guinsoodb_libpgquery::T_PGDropTableSpaceStmt:
		return "T_DropTableSpaceStmt";
	case guinsoodb_libpgquery::T_PGAlterObjectDependsStmt:
		return "T_AlterObjectDependsStmt";
	case guinsoodb_libpgquery::T_PGAlterObjectSchemaStmt:
		return "T_AlterObjectSchemaStmt";
	case guinsoodb_libpgquery::T_PGAlterOwnerStmt:
		return "T_AlterOwnerStmt";
	case guinsoodb_libpgquery::T_PGAlterOperatorStmt:
		return "T_AlterOperatorStmt";
	case guinsoodb_libpgquery::T_PGDropOwnedStmt:
		return "T_DropOwnedStmt";
	case guinsoodb_libpgquery::T_PGReassignOwnedStmt:
		return "T_ReassignOwnedStmt";
	case guinsoodb_libpgquery::T_PGCompositeTypeStmt:
		return "T_CompositeTypeStmt";
	case guinsoodb_libpgquery::T_PGCreateEnumStmt:
		return "T_CreateEnumStmt";
	case guinsoodb_libpgquery::T_PGCreateRangeStmt:
		return "T_CreateRangeStmt";
	case guinsoodb_libpgquery::T_PGAlterEnumStmt:
		return "T_AlterEnumStmt";
	case guinsoodb_libpgquery::T_PGAlterTSDictionaryStmt:
		return "T_AlterTSDictionaryStmt";
	case guinsoodb_libpgquery::T_PGAlterTSConfigurationStmt:
		return "T_AlterTSConfigurationStmt";
	case guinsoodb_libpgquery::T_PGCreateFdwStmt:
		return "T_CreateFdwStmt";
	case guinsoodb_libpgquery::T_PGAlterFdwStmt:
		return "T_AlterFdwStmt";
	case guinsoodb_libpgquery::T_PGCreateForeignServerStmt:
		return "T_CreateForeignServerStmt";
	case guinsoodb_libpgquery::T_PGAlterForeignServerStmt:
		return "T_AlterForeignServerStmt";
	case guinsoodb_libpgquery::T_PGCreateUserMappingStmt:
		return "T_CreateUserMappingStmt";
	case guinsoodb_libpgquery::T_PGAlterUserMappingStmt:
		return "T_AlterUserMappingStmt";
	case guinsoodb_libpgquery::T_PGDropUserMappingStmt:
		return "T_DropUserMappingStmt";
	case guinsoodb_libpgquery::T_PGAlterTableSpaceOptionsStmt:
		return "T_AlterTableSpaceOptionsStmt";
	case guinsoodb_libpgquery::T_PGAlterTableMoveAllStmt:
		return "T_AlterTableMoveAllStmt";
	case guinsoodb_libpgquery::T_PGSecLabelStmt:
		return "T_SecLabelStmt";
	case guinsoodb_libpgquery::T_PGCreateForeignTableStmt:
		return "T_CreateForeignTableStmt";
	case guinsoodb_libpgquery::T_PGImportForeignSchemaStmt:
		return "T_ImportForeignSchemaStmt";
	case guinsoodb_libpgquery::T_PGCreateExtensionStmt:
		return "T_CreateExtensionStmt";
	case guinsoodb_libpgquery::T_PGAlterExtensionStmt:
		return "T_AlterExtensionStmt";
	case guinsoodb_libpgquery::T_PGAlterExtensionContentsStmt:
		return "T_AlterExtensionContentsStmt";
	case guinsoodb_libpgquery::T_PGCreateEventTrigStmt:
		return "T_CreateEventTrigStmt";
	case guinsoodb_libpgquery::T_PGAlterEventTrigStmt:
		return "T_AlterEventTrigStmt";
	case guinsoodb_libpgquery::T_PGRefreshMatViewStmt:
		return "T_RefreshMatViewStmt";
	case guinsoodb_libpgquery::T_PGReplicaIdentityStmt:
		return "T_ReplicaIdentityStmt";
	case guinsoodb_libpgquery::T_PGAlterSystemStmt:
		return "T_AlterSystemStmt";
	case guinsoodb_libpgquery::T_PGCreatePolicyStmt:
		return "T_CreatePolicyStmt";
	case guinsoodb_libpgquery::T_PGAlterPolicyStmt:
		return "T_AlterPolicyStmt";
	case guinsoodb_libpgquery::T_PGCreateTransformStmt:
		return "T_CreateTransformStmt";
	case guinsoodb_libpgquery::T_PGCreateAmStmt:
		return "T_CreateAmStmt";
	case guinsoodb_libpgquery::T_PGCreatePublicationStmt:
		return "T_CreatePublicationStmt";
	case guinsoodb_libpgquery::T_PGAlterPublicationStmt:
		return "T_AlterPublicationStmt";
	case guinsoodb_libpgquery::T_PGCreateSubscriptionStmt:
		return "T_CreateSubscriptionStmt";
	case guinsoodb_libpgquery::T_PGAlterSubscriptionStmt:
		return "T_AlterSubscriptionStmt";
	case guinsoodb_libpgquery::T_PGDropSubscriptionStmt:
		return "T_DropSubscriptionStmt";
	case guinsoodb_libpgquery::T_PGCreateStatsStmt:
		return "T_CreateStatsStmt";
	case guinsoodb_libpgquery::T_PGAlterCollationStmt:
		return "T_AlterCollationStmt";
	case guinsoodb_libpgquery::T_PGAExpr:
		return "TAExpr";
	case guinsoodb_libpgquery::T_PGColumnRef:
		return "T_ColumnRef";
	case guinsoodb_libpgquery::T_PGParamRef:
		return "T_ParamRef";
	case guinsoodb_libpgquery::T_PGAConst:
		return "TAConst";
	case guinsoodb_libpgquery::T_PGFuncCall:
		return "T_FuncCall";
	case guinsoodb_libpgquery::T_PGAStar:
		return "TAStar";
	case guinsoodb_libpgquery::T_PGAIndices:
		return "TAIndices";
	case guinsoodb_libpgquery::T_PGAIndirection:
		return "TAIndirection";
	case guinsoodb_libpgquery::T_PGAArrayExpr:
		return "TAArrayExpr";
	case guinsoodb_libpgquery::T_PGResTarget:
		return "T_ResTarget";
	case guinsoodb_libpgquery::T_PGMultiAssignRef:
		return "T_MultiAssignRef";
	case guinsoodb_libpgquery::T_PGTypeCast:
		return "T_TypeCast";
	case guinsoodb_libpgquery::T_PGCollateClause:
		return "T_CollateClause";
	case guinsoodb_libpgquery::T_PGSortBy:
		return "T_SortBy";
	case guinsoodb_libpgquery::T_PGWindowDef:
		return "T_WindowDef";
	case guinsoodb_libpgquery::T_PGRangeSubselect:
		return "T_RangeSubselect";
	case guinsoodb_libpgquery::T_PGRangeFunction:
		return "T_RangeFunction";
	case guinsoodb_libpgquery::T_PGRangeTableSample:
		return "T_RangeTableSample";
	case guinsoodb_libpgquery::T_PGRangeTableFunc:
		return "T_RangeTableFunc";
	case guinsoodb_libpgquery::T_PGRangeTableFuncCol:
		return "T_RangeTableFuncCol";
	case guinsoodb_libpgquery::T_PGTypeName:
		return "T_TypeName";
	case guinsoodb_libpgquery::T_PGColumnDef:
		return "T_ColumnDef";
	case guinsoodb_libpgquery::T_PGIndexElem:
		return "T_IndexElem";
	case guinsoodb_libpgquery::T_PGConstraint:
		return "T_Constraint";
	case guinsoodb_libpgquery::T_PGDefElem:
		return "T_DefElem";
	case guinsoodb_libpgquery::T_PGRangeTblEntry:
		return "T_RangeTblEntry";
	case guinsoodb_libpgquery::T_PGRangeTblFunction:
		return "T_RangeTblFunction";
	case guinsoodb_libpgquery::T_PGTableSampleClause:
		return "T_TableSampleClause";
	case guinsoodb_libpgquery::T_PGWithCheckOption:
		return "T_WithCheckOption";
	case guinsoodb_libpgquery::T_PGSortGroupClause:
		return "T_SortGroupClause";
	case guinsoodb_libpgquery::T_PGGroupingSet:
		return "T_GroupingSet";
	case guinsoodb_libpgquery::T_PGWindowClause:
		return "T_WindowClause";
	case guinsoodb_libpgquery::T_PGObjectWithArgs:
		return "T_ObjectWithArgs";
	case guinsoodb_libpgquery::T_PGAccessPriv:
		return "T_AccessPriv";
	case guinsoodb_libpgquery::T_PGCreateOpClassItem:
		return "T_CreateOpClassItem";
	case guinsoodb_libpgquery::T_PGTableLikeClause:
		return "T_TableLikeClause";
	case guinsoodb_libpgquery::T_PGFunctionParameter:
		return "T_FunctionParameter";
	case guinsoodb_libpgquery::T_PGLockingClause:
		return "T_LockingClause";
	case guinsoodb_libpgquery::T_PGRowMarkClause:
		return "T_RowMarkClause";
	case guinsoodb_libpgquery::T_PGXmlSerialize:
		return "T_XmlSerialize";
	case guinsoodb_libpgquery::T_PGWithClause:
		return "T_WithClause";
	case guinsoodb_libpgquery::T_PGInferClause:
		return "T_InferClause";
	case guinsoodb_libpgquery::T_PGOnConflictClause:
		return "T_OnConflictClause";
	case guinsoodb_libpgquery::T_PGCommonTableExpr:
		return "T_CommonTableExpr";
	case guinsoodb_libpgquery::T_PGRoleSpec:
		return "T_RoleSpec";
	case guinsoodb_libpgquery::T_PGTriggerTransition:
		return "T_TriggerTransition";
	case guinsoodb_libpgquery::T_PGPartitionElem:
		return "T_PartitionElem";
	case guinsoodb_libpgquery::T_PGPartitionSpec:
		return "T_PartitionSpec";
	case guinsoodb_libpgquery::T_PGPartitionBoundSpec:
		return "T_PartitionBoundSpec";
	case guinsoodb_libpgquery::T_PGPartitionRangeDatum:
		return "T_PartitionRangeDatum";
	case guinsoodb_libpgquery::T_PGPartitionCmd:
		return "T_PartitionCmd";
	case guinsoodb_libpgquery::T_PGIdentifySystemCmd:
		return "T_IdentifySystemCmd";
	case guinsoodb_libpgquery::T_PGBaseBackupCmd:
		return "T_BaseBackupCmd";
	case guinsoodb_libpgquery::T_PGCreateReplicationSlotCmd:
		return "T_CreateReplicationSlotCmd";
	case guinsoodb_libpgquery::T_PGDropReplicationSlotCmd:
		return "T_DropReplicationSlotCmd";
	case guinsoodb_libpgquery::T_PGStartReplicationCmd:
		return "T_StartReplicationCmd";
	case guinsoodb_libpgquery::T_PGTimeLineHistoryCmd:
		return "T_TimeLineHistoryCmd";
	case guinsoodb_libpgquery::T_PGSQLCmd:
		return "T_SQLCmd";
	case guinsoodb_libpgquery::T_PGTriggerData:
		return "T_TriggerData";
	case guinsoodb_libpgquery::T_PGEventTriggerData:
		return "T_EventTriggerData";
	case guinsoodb_libpgquery::T_PGReturnSetInfo:
		return "T_ReturnSetInfo";
	case guinsoodb_libpgquery::T_PGWindowObjectData:
		return "T_WindowObjectData";
	case guinsoodb_libpgquery::T_PGTIDBitmap:
		return "T_TIDBitmap";
	case guinsoodb_libpgquery::T_PGInlineCodeBlock:
		return "T_InlineCodeBlock";
	case guinsoodb_libpgquery::T_PGFdwRoutine:
		return "T_FdwRoutine";
	case guinsoodb_libpgquery::T_PGIndexAmRoutine:
		return "T_IndexAmRoutine";
	case guinsoodb_libpgquery::T_PGTsmRoutine:
		return "T_TsmRoutine";
	case guinsoodb_libpgquery::T_PGForeignKeyCacheInfo:
		return "T_ForeignKeyCacheInfo";
	default:
		D_ASSERT(0);
		return "";
	}
}

} // namespace guinsoodb

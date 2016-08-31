/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

// Generated from C.g4 by ANTLR 4.2
package amulet.translator;
import org.antlr.v4.runtime.misc.NotNull;
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link CParser}.
 */
public interface CListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link CParser#expression}.
	 * @param ctx the parse tree
	 */
	void enterExpression(@NotNull CParser.ExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#expression}.
	 * @param ctx the parse tree
	 */
	void exitExpression(@NotNull CParser.ExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#declarator}.
	 * @param ctx the parse tree
	 */
	void enterDeclarator(@NotNull CParser.DeclaratorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#declarator}.
	 * @param ctx the parse tree
	 */
	void exitDeclarator(@NotNull CParser.DeclaratorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#leftExpression}.
	 * @param ctx the parse tree
	 */
	void enterLeftExpression(@NotNull CParser.LeftExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#leftExpression}.
	 * @param ctx the parse tree
	 */
	void exitLeftExpression(@NotNull CParser.LeftExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#expressionStatement}.
	 * @param ctx the parse tree
	 */
	void enterExpressionStatement(@NotNull CParser.ExpressionStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#expressionStatement}.
	 * @param ctx the parse tree
	 */
	void exitExpressionStatement(@NotNull CParser.ExpressionStatementContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#rightExpression}.
	 * @param ctx the parse tree
	 */
	void enterRightExpression(@NotNull CParser.RightExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#rightExpression}.
	 * @param ctx the parse tree
	 */
	void exitRightExpression(@NotNull CParser.RightExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#unaryExpression}.
	 * @param ctx the parse tree
	 */
	void enterUnaryExpression(@NotNull CParser.UnaryExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#unaryExpression}.
	 * @param ctx the parse tree
	 */
	void exitUnaryExpression(@NotNull CParser.UnaryExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#designation}.
	 * @param ctx the parse tree
	 */
	void enterDesignation(@NotNull CParser.DesignationContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#designation}.
	 * @param ctx the parse tree
	 */
	void exitDesignation(@NotNull CParser.DesignationContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#parameterDeclaration}.
	 * @param ctx the parse tree
	 */
	void enterParameterDeclaration(@NotNull CParser.ParameterDeclarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#parameterDeclaration}.
	 * @param ctx the parse tree
	 */
	void exitParameterDeclaration(@NotNull CParser.ParameterDeclarationContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#nestedParenthesesBlock}.
	 * @param ctx the parse tree
	 */
	void enterNestedParenthesesBlock(@NotNull CParser.NestedParenthesesBlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#nestedParenthesesBlock}.
	 * @param ctx the parse tree
	 */
	void exitNestedParenthesesBlock(@NotNull CParser.NestedParenthesesBlockContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#parameterTypeList}.
	 * @param ctx the parse tree
	 */
	void enterParameterTypeList(@NotNull CParser.ParameterTypeListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#parameterTypeList}.
	 * @param ctx the parse tree
	 */
	void exitParameterTypeList(@NotNull CParser.ParameterTypeListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#designator}.
	 * @param ctx the parse tree
	 */
	void enterDesignator(@NotNull CParser.DesignatorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#designator}.
	 * @param ctx the parse tree
	 */
	void exitDesignator(@NotNull CParser.DesignatorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#primaryExpression}.
	 * @param ctx the parse tree
	 */
	void enterPrimaryExpression(@NotNull CParser.PrimaryExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#primaryExpression}.
	 * @param ctx the parse tree
	 */
	void exitPrimaryExpression(@NotNull CParser.PrimaryExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#structOrUnion}.
	 * @param ctx the parse tree
	 */
	void enterStructOrUnion(@NotNull CParser.StructOrUnionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#structOrUnion}.
	 * @param ctx the parse tree
	 */
	void exitStructOrUnion(@NotNull CParser.StructOrUnionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#initDeclarator}.
	 * @param ctx the parse tree
	 */
	void enterInitDeclarator(@NotNull CParser.InitDeclaratorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#initDeclarator}.
	 * @param ctx the parse tree
	 */
	void exitInitDeclarator(@NotNull CParser.InitDeclaratorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#storageClassSpecifier}.
	 * @param ctx the parse tree
	 */
	void enterStorageClassSpecifier(@NotNull CParser.StorageClassSpecifierContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#storageClassSpecifier}.
	 * @param ctx the parse tree
	 */
	void exitStorageClassSpecifier(@NotNull CParser.StorageClassSpecifierContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#typeQualifierList}.
	 * @param ctx the parse tree
	 */
	void enterTypeQualifierList(@NotNull CParser.TypeQualifierListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#typeQualifierList}.
	 * @param ctx the parse tree
	 */
	void exitTypeQualifierList(@NotNull CParser.TypeQualifierListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#functionCall}.
	 * @param ctx the parse tree
	 */
	void enterFunctionCall(@NotNull CParser.FunctionCallContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#functionCall}.
	 * @param ctx the parse tree
	 */
	void exitFunctionCall(@NotNull CParser.FunctionCallContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#structDeclarator}.
	 * @param ctx the parse tree
	 */
	void enterStructDeclarator(@NotNull CParser.StructDeclaratorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#structDeclarator}.
	 * @param ctx the parse tree
	 */
	void exitStructDeclarator(@NotNull CParser.StructDeclaratorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#parameterList}.
	 * @param ctx the parse tree
	 */
	void enterParameterList(@NotNull CParser.ParameterListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#parameterList}.
	 * @param ctx the parse tree
	 */
	void exitParameterList(@NotNull CParser.ParameterListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#enumerator}.
	 * @param ctx the parse tree
	 */
	void enterEnumerator(@NotNull CParser.EnumeratorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#enumerator}.
	 * @param ctx the parse tree
	 */
	void exitEnumerator(@NotNull CParser.EnumeratorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#declarationList}.
	 * @param ctx the parse tree
	 */
	void enterDeclarationList(@NotNull CParser.DeclarationListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#declarationList}.
	 * @param ctx the parse tree
	 */
	void exitDeclarationList(@NotNull CParser.DeclarationListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#shiftExpression}.
	 * @param ctx the parse tree
	 */
	void enterShiftExpression(@NotNull CParser.ShiftExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#shiftExpression}.
	 * @param ctx the parse tree
	 */
	void exitShiftExpression(@NotNull CParser.ShiftExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#blockItemList}.
	 * @param ctx the parse tree
	 */
	void enterBlockItemList(@NotNull CParser.BlockItemListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#blockItemList}.
	 * @param ctx the parse tree
	 */
	void exitBlockItemList(@NotNull CParser.BlockItemListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#typedefName}.
	 * @param ctx the parse tree
	 */
	void enterTypedefName(@NotNull CParser.TypedefNameContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#typedefName}.
	 * @param ctx the parse tree
	 */
	void exitTypedefName(@NotNull CParser.TypedefNameContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#declaration}.
	 * @param ctx the parse tree
	 */
	void enterDeclaration(@NotNull CParser.DeclarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#declaration}.
	 * @param ctx the parse tree
	 */
	void exitDeclaration(@NotNull CParser.DeclarationContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#designatorList}.
	 * @param ctx the parse tree
	 */
	void enterDesignatorList(@NotNull CParser.DesignatorListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#designatorList}.
	 * @param ctx the parse tree
	 */
	void exitDesignatorList(@NotNull CParser.DesignatorListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#assignmentExpression}.
	 * @param ctx the parse tree
	 */
	void enterAssignmentExpression(@NotNull CParser.AssignmentExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#assignmentExpression}.
	 * @param ctx the parse tree
	 */
	void exitAssignmentExpression(@NotNull CParser.AssignmentExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#selectionStatement}.
	 * @param ctx the parse tree
	 */
	void enterSelectionStatement(@NotNull CParser.SelectionStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#selectionStatement}.
	 * @param ctx the parse tree
	 */
	void exitSelectionStatement(@NotNull CParser.SelectionStatementContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#argumentExpressionList}.
	 * @param ctx the parse tree
	 */
	void enterArgumentExpressionList(@NotNull CParser.ArgumentExpressionListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#argumentExpressionList}.
	 * @param ctx the parse tree
	 */
	void exitArgumentExpressionList(@NotNull CParser.ArgumentExpressionListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#additiveExpression}.
	 * @param ctx the parse tree
	 */
	void enterAdditiveExpression(@NotNull CParser.AdditiveExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#additiveExpression}.
	 * @param ctx the parse tree
	 */
	void exitAdditiveExpression(@NotNull CParser.AdditiveExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#declarationSpecifier}.
	 * @param ctx the parse tree
	 */
	void enterDeclarationSpecifier(@NotNull CParser.DeclarationSpecifierContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#declarationSpecifier}.
	 * @param ctx the parse tree
	 */
	void exitDeclarationSpecifier(@NotNull CParser.DeclarationSpecifierContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#postfixExpression}.
	 * @param ctx the parse tree
	 */
	void enterPostfixExpression(@NotNull CParser.PostfixExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#postfixExpression}.
	 * @param ctx the parse tree
	 */
	void exitPostfixExpression(@NotNull CParser.PostfixExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#alignmentSpecifier}.
	 * @param ctx the parse tree
	 */
	void enterAlignmentSpecifier(@NotNull CParser.AlignmentSpecifierContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#alignmentSpecifier}.
	 * @param ctx the parse tree
	 */
	void exitAlignmentSpecifier(@NotNull CParser.AlignmentSpecifierContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#statement}.
	 * @param ctx the parse tree
	 */
	void enterStatement(@NotNull CParser.StatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#statement}.
	 * @param ctx the parse tree
	 */
	void exitStatement(@NotNull CParser.StatementContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#exclusiveOrExpression}.
	 * @param ctx the parse tree
	 */
	void enterExclusiveOrExpression(@NotNull CParser.ExclusiveOrExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#exclusiveOrExpression}.
	 * @param ctx the parse tree
	 */
	void exitExclusiveOrExpression(@NotNull CParser.ExclusiveOrExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#unaryOperator}.
	 * @param ctx the parse tree
	 */
	void enterUnaryOperator(@NotNull CParser.UnaryOperatorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#unaryOperator}.
	 * @param ctx the parse tree
	 */
	void exitUnaryOperator(@NotNull CParser.UnaryOperatorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#functionDefinition}.
	 * @param ctx the parse tree
	 */
	void enterFunctionDefinition(@NotNull CParser.FunctionDefinitionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#functionDefinition}.
	 * @param ctx the parse tree
	 */
	void exitFunctionDefinition(@NotNull CParser.FunctionDefinitionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#unaryAddrOperator}.
	 * @param ctx the parse tree
	 */
	void enterUnaryAddrOperator(@NotNull CParser.UnaryAddrOperatorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#unaryAddrOperator}.
	 * @param ctx the parse tree
	 */
	void exitUnaryAddrOperator(@NotNull CParser.UnaryAddrOperatorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#constantExpression}.
	 * @param ctx the parse tree
	 */
	void enterConstantExpression(@NotNull CParser.ConstantExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#constantExpression}.
	 * @param ctx the parse tree
	 */
	void exitConstantExpression(@NotNull CParser.ConstantExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#structDeclarationList}.
	 * @param ctx the parse tree
	 */
	void enterStructDeclarationList(@NotNull CParser.StructDeclarationListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#structDeclarationList}.
	 * @param ctx the parse tree
	 */
	void exitStructDeclarationList(@NotNull CParser.StructDeclarationListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#initializerList}.
	 * @param ctx the parse tree
	 */
	void enterInitializerList(@NotNull CParser.InitializerListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#initializerList}.
	 * @param ctx the parse tree
	 */
	void exitInitializerList(@NotNull CParser.InitializerListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#pointer}.
	 * @param ctx the parse tree
	 */
	void enterPointer(@NotNull CParser.PointerContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#pointer}.
	 * @param ctx the parse tree
	 */
	void exitPointer(@NotNull CParser.PointerContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#declarationSpecifiers}.
	 * @param ctx the parse tree
	 */
	void enterDeclarationSpecifiers(@NotNull CParser.DeclarationSpecifiersContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#declarationSpecifiers}.
	 * @param ctx the parse tree
	 */
	void exitDeclarationSpecifiers(@NotNull CParser.DeclarationSpecifiersContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#structDeclaration}.
	 * @param ctx the parse tree
	 */
	void enterStructDeclaration(@NotNull CParser.StructDeclarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#structDeclaration}.
	 * @param ctx the parse tree
	 */
	void exitStructDeclaration(@NotNull CParser.StructDeclarationContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#enumSpecifier}.
	 * @param ctx the parse tree
	 */
	void enterEnumSpecifier(@NotNull CParser.EnumSpecifierContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#enumSpecifier}.
	 * @param ctx the parse tree
	 */
	void exitEnumSpecifier(@NotNull CParser.EnumSpecifierContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#multiplicativeExpression}.
	 * @param ctx the parse tree
	 */
	void enterMultiplicativeExpression(@NotNull CParser.MultiplicativeExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#multiplicativeExpression}.
	 * @param ctx the parse tree
	 */
	void exitMultiplicativeExpression(@NotNull CParser.MultiplicativeExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#assignmentOperator}.
	 * @param ctx the parse tree
	 */
	void enterAssignmentOperator(@NotNull CParser.AssignmentOperatorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#assignmentOperator}.
	 * @param ctx the parse tree
	 */
	void exitAssignmentOperator(@NotNull CParser.AssignmentOperatorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#declarationSpecifiers2}.
	 * @param ctx the parse tree
	 */
	void enterDeclarationSpecifiers2(@NotNull CParser.DeclarationSpecifiers2Context ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#declarationSpecifiers2}.
	 * @param ctx the parse tree
	 */
	void exitDeclarationSpecifiers2(@NotNull CParser.DeclarationSpecifiers2Context ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#compilationUnit}.
	 * @param ctx the parse tree
	 */
	void enterCompilationUnit(@NotNull CParser.CompilationUnitContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#compilationUnit}.
	 * @param ctx the parse tree
	 */
	void exitCompilationUnit(@NotNull CParser.CompilationUnitContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#directDeclarator}.
	 * @param ctx the parse tree
	 */
	void enterDirectDeclarator(@NotNull CParser.DirectDeclaratorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#directDeclarator}.
	 * @param ctx the parse tree
	 */
	void exitDirectDeclarator(@NotNull CParser.DirectDeclaratorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#directAbstractDeclarator}.
	 * @param ctx the parse tree
	 */
	void enterDirectAbstractDeclarator(@NotNull CParser.DirectAbstractDeclaratorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#directAbstractDeclarator}.
	 * @param ctx the parse tree
	 */
	void exitDirectAbstractDeclarator(@NotNull CParser.DirectAbstractDeclaratorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#identifierList}.
	 * @param ctx the parse tree
	 */
	void enterIdentifierList(@NotNull CParser.IdentifierListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#identifierList}.
	 * @param ctx the parse tree
	 */
	void exitIdentifierList(@NotNull CParser.IdentifierListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#typeSpecifier}.
	 * @param ctx the parse tree
	 */
	void enterTypeSpecifier(@NotNull CParser.TypeSpecifierContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#typeSpecifier}.
	 * @param ctx the parse tree
	 */
	void exitTypeSpecifier(@NotNull CParser.TypeSpecifierContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#conditionalExpression}.
	 * @param ctx the parse tree
	 */
	void enterConditionalExpression(@NotNull CParser.ConditionalExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#conditionalExpression}.
	 * @param ctx the parse tree
	 */
	void exitConditionalExpression(@NotNull CParser.ConditionalExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#translationUnit}.
	 * @param ctx the parse tree
	 */
	void enterTranslationUnit(@NotNull CParser.TranslationUnitContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#translationUnit}.
	 * @param ctx the parse tree
	 */
	void exitTranslationUnit(@NotNull CParser.TranslationUnitContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#andExpression}.
	 * @param ctx the parse tree
	 */
	void enterAndExpression(@NotNull CParser.AndExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#andExpression}.
	 * @param ctx the parse tree
	 */
	void exitAndExpression(@NotNull CParser.AndExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#structOrUnionSpecifier}.
	 * @param ctx the parse tree
	 */
	void enterStructOrUnionSpecifier(@NotNull CParser.StructOrUnionSpecifierContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#structOrUnionSpecifier}.
	 * @param ctx the parse tree
	 */
	void exitStructOrUnionSpecifier(@NotNull CParser.StructOrUnionSpecifierContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#labeledStatement}.
	 * @param ctx the parse tree
	 */
	void enterLabeledStatement(@NotNull CParser.LabeledStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#labeledStatement}.
	 * @param ctx the parse tree
	 */
	void exitLabeledStatement(@NotNull CParser.LabeledStatementContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#relationalExpression}.
	 * @param ctx the parse tree
	 */
	void enterRelationalExpression(@NotNull CParser.RelationalExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#relationalExpression}.
	 * @param ctx the parse tree
	 */
	void exitRelationalExpression(@NotNull CParser.RelationalExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#enumerationConstant}.
	 * @param ctx the parse tree
	 */
	void enterEnumerationConstant(@NotNull CParser.EnumerationConstantContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#enumerationConstant}.
	 * @param ctx the parse tree
	 */
	void exitEnumerationConstant(@NotNull CParser.EnumerationConstantContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#iterationStatement}.
	 * @param ctx the parse tree
	 */
	void enterIterationStatement(@NotNull CParser.IterationStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#iterationStatement}.
	 * @param ctx the parse tree
	 */
	void exitIterationStatement(@NotNull CParser.IterationStatementContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#typeQualifier}.
	 * @param ctx the parse tree
	 */
	void enterTypeQualifier(@NotNull CParser.TypeQualifierContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#typeQualifier}.
	 * @param ctx the parse tree
	 */
	void exitTypeQualifier(@NotNull CParser.TypeQualifierContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#enumeratorList}.
	 * @param ctx the parse tree
	 */
	void enterEnumeratorList(@NotNull CParser.EnumeratorListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#enumeratorList}.
	 * @param ctx the parse tree
	 */
	void exitEnumeratorList(@NotNull CParser.EnumeratorListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#arrayDeclarator}.
	 * @param ctx the parse tree
	 */
	void enterArrayDeclarator(@NotNull CParser.ArrayDeclaratorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#arrayDeclarator}.
	 * @param ctx the parse tree
	 */
	void exitArrayDeclarator(@NotNull CParser.ArrayDeclaratorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#compoundStatement}.
	 * @param ctx the parse tree
	 */
	void enterCompoundStatement(@NotNull CParser.CompoundStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#compoundStatement}.
	 * @param ctx the parse tree
	 */
	void exitCompoundStatement(@NotNull CParser.CompoundStatementContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#jumpStatement}.
	 * @param ctx the parse tree
	 */
	void enterJumpStatement(@NotNull CParser.JumpStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#jumpStatement}.
	 * @param ctx the parse tree
	 */
	void exitJumpStatement(@NotNull CParser.JumpStatementContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#blockItem}.
	 * @param ctx the parse tree
	 */
	void enterBlockItem(@NotNull CParser.BlockItemContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#blockItem}.
	 * @param ctx the parse tree
	 */
	void exitBlockItem(@NotNull CParser.BlockItemContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#logicalAndExpression}.
	 * @param ctx the parse tree
	 */
	void enterLogicalAndExpression(@NotNull CParser.LogicalAndExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#logicalAndExpression}.
	 * @param ctx the parse tree
	 */
	void exitLogicalAndExpression(@NotNull CParser.LogicalAndExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#abstractDeclarator}.
	 * @param ctx the parse tree
	 */
	void enterAbstractDeclarator(@NotNull CParser.AbstractDeclaratorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#abstractDeclarator}.
	 * @param ctx the parse tree
	 */
	void exitAbstractDeclarator(@NotNull CParser.AbstractDeclaratorContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#typeName}.
	 * @param ctx the parse tree
	 */
	void enterTypeName(@NotNull CParser.TypeNameContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#typeName}.
	 * @param ctx the parse tree
	 */
	void exitTypeName(@NotNull CParser.TypeNameContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#logicalOrExpression}.
	 * @param ctx the parse tree
	 */
	void enterLogicalOrExpression(@NotNull CParser.LogicalOrExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#logicalOrExpression}.
	 * @param ctx the parse tree
	 */
	void exitLogicalOrExpression(@NotNull CParser.LogicalOrExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#inclusiveOrExpression}.
	 * @param ctx the parse tree
	 */
	void enterInclusiveOrExpression(@NotNull CParser.InclusiveOrExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#inclusiveOrExpression}.
	 * @param ctx the parse tree
	 */
	void exitInclusiveOrExpression(@NotNull CParser.InclusiveOrExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#justUnion}.
	 * @param ctx the parse tree
	 */
	void enterJustUnion(@NotNull CParser.JustUnionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#justUnion}.
	 * @param ctx the parse tree
	 */
	void exitJustUnion(@NotNull CParser.JustUnionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#equalityExpression}.
	 * @param ctx the parse tree
	 */
	void enterEqualityExpression(@NotNull CParser.EqualityExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#equalityExpression}.
	 * @param ctx the parse tree
	 */
	void exitEqualityExpression(@NotNull CParser.EqualityExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#structDeclaratorList}.
	 * @param ctx the parse tree
	 */
	void enterStructDeclaratorList(@NotNull CParser.StructDeclaratorListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#structDeclaratorList}.
	 * @param ctx the parse tree
	 */
	void exitStructDeclaratorList(@NotNull CParser.StructDeclaratorListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#castExpression}.
	 * @param ctx the parse tree
	 */
	void enterCastExpression(@NotNull CParser.CastExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#castExpression}.
	 * @param ctx the parse tree
	 */
	void exitCastExpression(@NotNull CParser.CastExpressionContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#specifierQualifierList}.
	 * @param ctx the parse tree
	 */
	void enterSpecifierQualifierList(@NotNull CParser.SpecifierQualifierListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#specifierQualifierList}.
	 * @param ctx the parse tree
	 */
	void exitSpecifierQualifierList(@NotNull CParser.SpecifierQualifierListContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#externalDeclaration}.
	 * @param ctx the parse tree
	 */
	void enterExternalDeclaration(@NotNull CParser.ExternalDeclarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#externalDeclaration}.
	 * @param ctx the parse tree
	 */
	void exitExternalDeclaration(@NotNull CParser.ExternalDeclarationContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#initializer}.
	 * @param ctx the parse tree
	 */
	void enterInitializer(@NotNull CParser.InitializerContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#initializer}.
	 * @param ctx the parse tree
	 */
	void exitInitializer(@NotNull CParser.InitializerContext ctx);

	/**
	 * Enter a parse tree produced by {@link CParser#initDeclaratorList}.
	 * @param ctx the parse tree
	 */
	void enterInitDeclaratorList(@NotNull CParser.InitDeclaratorListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CParser#initDeclaratorList}.
	 * @param ctx the parse tree
	 */
	void exitInitDeclaratorList(@NotNull CParser.InitDeclaratorListContext ctx);
}
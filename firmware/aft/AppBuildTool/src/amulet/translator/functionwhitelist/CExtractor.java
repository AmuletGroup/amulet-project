/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.translator.functionwhitelist;

import org.antlr.v4.runtime.*;
import java.util.*;

import amulet.translator.CParser;
import amulet.translator.CBaseListener;

public class CExtractor extends CBaseListener {
    TokenStreamRewriter rewriter;
    TokenStream tokens;
    CParser parser;
	
	String className;
	Vector<String> functionDefinitions;
	Vector<String> functionCalls;
	Vector<String> operationNames;
	
    public CExtractor(TokenStream tokens, CParser parser, String className, Vector<String> operationNames) {
		rewriter = new TokenStreamRewriter(tokens);
		this.className = className;
		this.parser = parser;
		this.tokens = tokens;
		this.operationNames = operationNames;

		functionDefinitions = new Vector<String>();
		functionCalls = new Vector<String>();
    }
	
	//getters and setters for retrieving messages after translation
	public List<String> getFunctionDefinitions() {
		return functionDefinitions;
	}
	
	public List<String> getFunctionCalls() {
		return functionCalls; 
	}	
	
	////Listener methods for translating the AST during the walk of the parse tree.

	public void enterFunctionDefinition(CParser.FunctionDefinitionContext ctx) 
	{ 
		String func_name = ctx.Identifier().getText();
		functionDefinitions.add(func_name);
	}

	public void enterFunctionCall(CParser.FunctionCallContext ctx)
	{
		boolean isApiCall = false;
		String func_name = ctx.Identifier().getText();
		isApiCall = func_name.startsWith("Amulet");
		if (!isApiCall) {
			functionCalls.add(func_name);
		}
		if (operationNames.contains(func_name)) {
			String replaceText = className + "_" + ctx.Identifier().getText() + "(me";
			if (ctx.argumentExpressionList() != null) {
				replaceText += ", " + ctx.argumentExpressionList().getText();
			}
			replaceText += ");";
			rewriter.replace(ctx.getStart(), ctx.getStop(), replaceText);
		}
	}
}
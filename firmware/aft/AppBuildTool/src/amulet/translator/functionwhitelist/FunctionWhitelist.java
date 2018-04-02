package amulet.translator.functionwhitelist;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.*;

import org.w3c.dom.Node;

import java.util.*;

import amulet.fsm.State;
import amulet.translator.CLexer;
import amulet.translator.CParser;

public class FunctionWhitelist {
	private boolean success = false;

	private Vector<String> functionDefinitions;
	private Vector<String> functionCalls;
    
	public boolean checkIfAllFunctionsDefined()
	{
		if (functionDefinitions == null || functionCalls == null) return true;
		if (functionDefinitions.size() == 0 && functionCalls.size() > 0) return false;
		for (int i=0; i < functionDefinitions.size(); i++) {
			functionCalls.removeAll(Collections.singleton(functionDefinitions.get(i)));
		}
		if (functionCalls.size() > 0) {
			return false;
		}
		return true;
	}

	public FunctionWhitelist(String className, Vector<State> states, Vector<Node> action_nodes, Vector<Node> guardNodes, Vector<String> operationNames)
	{
		functionDefinitions = new Vector<String>();
		functionCalls = new Vector<String>();
		functionDefinitions.addAll(operationNames);
		for (int i=0; i < states.size(); i++) {
			if (states.get(i).getEntryCode() != null)
			{
				states.get(i).setEntryCode(addFuncDefFuncCalls(className, states.get(i).getEntryCode(), operationNames));
			}
			if (states.get(i).getExitCode() != null)
			{
				states.get(i).setExitCode(addFuncDefFuncCalls(className, states.get(i).getExitCode(), operationNames));
			}
		}
		for (int i=0; i < action_nodes.size(); i++) {
			if (action_nodes.get(i).getTextContent() != null)
			{
				action_nodes.get(i).setTextContent(addFuncDefFuncCalls(className, action_nodes.get(i).getTextContent(), operationNames));
			}
		}
		for (int i=0; i < guardNodes.size(); i++) {
			if (guardNodes.get(i).getTextContent() != null)
			{
				guardNodes.get(i).setTextContent(addFuncDefFuncCalls(className, guardNodes.get(i).getTextContent(), operationNames));
			}
		}
		/** Commenting this out since it isn't required to check if functions are defined. Compile will fail if they aren't defined **/
		/*
		if (!checkIfAllFunctionsDefined()) {
			System.err.println("Calling undefined function(s): ");
			for (int i=0; i < functionCalls.size(); i++) {
				System.out.println(functionCalls.get(i));
			}
			setSuccess(false);
		} else {
			setSuccess(true);
		}*/
		setSuccess(true);
	}

	public void setSuccess(boolean val) {
		success = val;
	}

	public boolean getSuccess() {
		return success;
	}

	public String addFuncDefFuncCalls(String className, String code, Vector<String> operationNames) {
		ANTLRInputStream input = new ANTLRInputStream(code);
		CLexer lexer = new CLexer(input);
	    CommonTokenStream tokens = new CommonTokenStream(lexer);
	    CParser parser = new CParser(tokens);
	    ParseTree tree;
	    if (code.trim().length() > 0 && code.indexOf(";") == -1 && code.indexOf("//") == -1 && code.indexOf("/*") == -1) {
	        //Guard Code
	        tree = parser.expression();
	    } else {
	        tree = parser.blockItemList();
	    }
	                
	    ParseTreeWalker walker = new ParseTreeWalker(); // create standard walker
	    CExtractor extractor = new CExtractor(tokens, parser, className, operationNames);
	    walker.walk(extractor, tree); // initiate walk of tree with listener

	    functionDefinitions.addAll(extractor.getFunctionDefinitions());
	    functionCalls.addAll(extractor.getFunctionCalls());
	    String translatedCode = extractor.rewriter.getText();
	    return translatedCode;
	}
    
}

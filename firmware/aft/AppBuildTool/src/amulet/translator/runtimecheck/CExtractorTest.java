/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.translator.runtimecheck;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

import amulet.resourceprofiler.Resource;
import amulet.resourceprofiler.ResourceProfiler;
import amulet.resourceprofiler.Resource.ComputationType;
import amulet.resourceprofiler.Resource.ResourceType;
import amulet.translator.CBaseListener;
import amulet.translator.CLexer;
import amulet.translator.CParser;

public class CExtractorTest extends CBaseListener {

	/*
	 * TEST SETUP
	 */
	
	// Test scenarios for jacob to play with...
	public static boolean testEnterFunctionCalls = false;
	public static boolean testEnterIterationStatement = false;
	
    
	// Fake QMApp/QMClass info for test.
	public static String fakeRequestorName = "MyFakeAppName";
	public static String fakeStateName = "MyFakeState";	
	
	// Minimal CExtractor/AFT state.
    String requestor;
    TokenStream tokens;
    CParser parser;
    ResourceProfiler resourceProfiler;
    
    public CExtractorTest(String requestor, TokenStream tokens, CParser parser, ResourceProfiler resourceProfiler) {
		this.requestor = requestor;
		this.tokens = tokens;
		this.parser = parser;
		this.resourceProfiler = resourceProfiler;
    }
	
	/*
	 * LISTENER METHODS USED DURING WALK OF THE PARSE TREE.
	 */

	public void enterFunctionCall(CParser.FunctionCallContext ctx) {	
//		System.out.println("DEBUG func enterFunctionCall()::"+ctx.getText());
		
		if (testEnterFunctionCalls) {
			if (ctx == null || ctx.Identifier() == null)
				return;		
			String func_name = ctx.Identifier().getText();
	
			// Add some resource record to the profiler. 
			if (resourceProfiler != null && testEnterFunctionCalls) {
				Resource record = new Resource(func_name, requestor, ResourceType.NON_AMULET_API_FUNCTION_CALL, 0.0);
				resourceProfiler.add( record );
			}
		}
	}

	@Override 
	public void enterIterationStatement(CParser.IterationStatementContext ctx) {
//		System.out.println("DEBUG loop enterIterationStatement()::"+ctx.getText());
		
		if (testEnterIterationStatement) {
			// Strip out strings for the bounds of the corresponding for-loop.
			String start =  ctx.declaration().initDeclaratorList().stop.getText();
			String end = ctx.expression(0).assignmentExpression().conditionalExpression().logicalOrExpression().logicalAndExpression().inclusiveOrExpression().exclusiveOrExpression().andExpression().equalityExpression().relationalExpression().shiftExpression().additiveExpression().multiplicativeExpression().castExpression().getText();
			String update = "1";
			
			/*
			 * Enter the "for-loop context". All records added to the Resource Profiler will 
			 * be annotated as being within the for-loop for more accurate accounting. 
			 */
			// Inform the ResourceProfiler that we've entered a for-loop.
			resourceProfiler.enterForLoopContext(start, end, update);
		}
	}
	
	public void exitIterationStatement(CParser.IterationStatementContext ctx) { 
	    // Add a general for-loop (COMPUTATION) resource.
		if (resourceProfiler != null) {
			Resource record = new Resource(ComputationType.FOR_LOOP.text(), requestor, ResourceType.COMPUTATION, 0.0);
			resourceProfiler.add( record );
		}

		// Inform the ResourceProfiler that we've exited the for-loop.
	    resourceProfiler.exitForLoopContext();
	}
	
	/* 
	 * TEST FOR JACOB.
	 * 
	 * JACOB: you will have to update the path specified in resProfiler.add( ... ). Hardcoding it allowed me 
	 * to put this test together without modifying any of the existing AFT itself. 
	 */
	
	public static void main(String[] args) {
		// TODO: JACOB CHANGE ME TO TRUE/FALSE TO TRY OUT DIFFERENT COMBOS!
		testEnterFunctionCalls = true;
		testEnterIterationStatement = true;
		
		// Fake Resource Profiler with some fake QMClass info for test.
		ResourceProfiler resProfiler = new ResourceProfiler();
		resProfiler.add("/Users/traviswp/projects/project-amulet/src/applications/bite_counter/bite_counter.qm"); // TODO: JACOB CHANGE ME!
		resProfiler.setStateContext(fakeStateName);

		// Simple code to test.
		String code = "";
		code += "myFunc1();\n";
		code += "myFunc2();\n";
		code += "int sum = 0;\n";
		code += "for (uint8_t j = 0; j < 10; j++) {\n";
		code += "	sum = i + j;\n";
		code += "	myFunc3();\n";
		code += "}\n";

		// Setup for parsing...
		ANTLRInputStream input = new ANTLRInputStream(code);
		CLexer lexer = new CLexer(input);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        CParser parser = new CParser(tokens);
        ParseTree tree = parser.blockItemList();
        
        // Do parse/walk...
        ParseTreeWalker walker = new ParseTreeWalker(); // create standard walker
        CExtractorTest extractor = new CExtractorTest(fakeRequestorName, tokens, parser, resProfiler);
        walker.walk(extractor, tree); // initiate walk of tree with listener
        
        // Display the contents of the profiler.
        System.out.println(resProfiler.toString());
	}
}

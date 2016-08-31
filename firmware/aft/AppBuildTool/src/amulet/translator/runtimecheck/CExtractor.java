/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.translator.runtimecheck;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import org.stringtemplate.v4.*;

import java.util.*;
import java.io.InputStream;
import java.io.ByteArrayInputStream;

import amulet.qm.QMClass;
import amulet.resourceprofiler.Resource;
import amulet.resourceprofiler.Resource.ComputationType;
import amulet.resourceprofiler.ResourceProfiler;
import amulet.resourceprofiler.Resource.ResourceType;
import amulet.translator.CParser;
import amulet.translator.CLexer;
import amulet.translator.CBaseListener;
import amulet.translator.CParser.DeclarationContext;
import amulet.translator.CParser.ExpressionContext;

public class CExtractor extends CBaseListener {
    TokenStreamRewriter rewriter;
    STGroup templateGroup;
    QMClass qmclass; 
    TokenStream tokens;
    CParser parser;
    String appId;
    String appLogId;
    String togglePinCode = "P1OUT |= BIT2; // on\n{volatile uint8_t i=0;\nfor(i;i<200;i++);}\nP1OUT &= (~BIT2); // off";
    boolean togglePins = false;
    Vector<String> arrays;
    Vector<String> global_arrays;
    ResourceProfiler resourceProfiler;
	
    public CExtractor(QMClass qmclass, TokenStream tokens, CParser parser, boolean toggle, Vector<String> global_arrays, ResourceProfiler resourceProfiler) {
		rewriter = new TokenStreamRewriter(tokens);
		this.templateGroup = new STGroupFile("./amulet/translator/runtimecheck/CTemplates.stg");
		this.qmclass = qmclass;
		this.tokens = tokens;
		this.parser = parser;
		this.appId = qmclass.getName().toUpperCase()+"_APP";
		this.appLogId = appId+"_LOG";
		this.togglePins = toggle;
		this.arrays = global_arrays;
		this.global_arrays = global_arrays;
		this.resourceProfiler = resourceProfiler;
    }

	////Listener methods for translating the AST during the walk of the parse tree.

	public void enterFunctionCall(CParser.FunctionCallContext ctx) {	
		if (ctx == null || ctx.Identifier() == null)
			return;
		
		String func_name = ctx.Identifier().getText();

		// Determine if function call corresponds to custom Amulet API or not.
		boolean isApiCall = func_name.startsWith("Amulet");
		
		// Track the "type" of resource which is being used in this function call.
		ResourceType observedResourceType = null;
		
		if(isApiCall) {
			/* 
			 * Construct the API Call object which may contain:
			 * - apiDef (required)
			 * - appLogId (optional; required for Log operations though)
			 * - arguments (optional)
			 * - appId (required)
			 *
			 * If there is a need to add a new handler for checking/modifying 
			 * function calls during translation, add a method structured as 
			 * below (prefixed with "handler_" followed by a descriptive name 
			 * for what the handler is checking/handling). The "isOtherCall" 
			 * methods should be the last two "catch-all" handlers to handle 
			 * function calls in Amulet apps generally. 
			 *
			 * See: CTemplates.stg for more details relating to existing templates. 
			 */
			ST apiCall = null;

			/*
			 * Custom handlers. Please put any new handlers among this "group"
			 * of handlers. 
			 */
			if ( (apiCall = handler_isAmuletStateTransitionCall(ctx)) != null ) {
				// handled
				observedResourceType = ResourceType.AMULET_API_FUNCTION_CALL;
			} else if ( (apiCall = handler_isAmuletLogCall(ctx)) != null ) {
				// handled
				observedResourceType = ResourceType.AMULET_API_FUNCTION_CALL;
			} else if ( (apiCall = handler_isAmuletSubscribeCall(ctx)) != null ) {
				// handled
				observedResourceType = ResourceType.AMULET_API_FUNCTION_CALL;
			} else if ( (apiCall = handler_isAmuletTimerCall(ctx)) != null ) {
				// handled
				observedResourceType = ResourceType.AMULET_API_FUNCTION_CALL;
			} else if ( (apiCall = handler_isAmuletRectCall(ctx)) != null ) {
				// handled -- intentionally leave empty so observedResourceType == null
				// and we won't double-add the resource below (it was already added 
				// in the handler).
			}			
			/*
			 * Catch-all handlers. Please put new handlers ABOVE (i.e., before calling
			 * handler_isOtherCallWithArguments() and handler_isOtherCallWithNoArguments())
			 */
			else if ( (apiCall = handler_isOtherCallWithArguments(ctx)) != null ) {
				// handled
				observedResourceType = ResourceType.AMULET_API_FUNCTION_CALL;
			} else {
				apiCall = handler_isOtherCallWithNoArguments(ctx);
				observedResourceType = ResourceType.AMULET_API_FUNCTION_CALL;
			}

			// API CALL: add function caller ID (App ID) to the API call.
			apiCall.add("appId",this.appId);

			// API CALL: PIN handling (before)
			if (togglePins) {
				rewriter.insertBefore(ctx.getStart(), "\n"+togglePinCode+"\n");
			}

			// API CALL: re-write the API call
			rewriter.replace(ctx.getStart(),ctx.getStop(),apiCall.render());

			// API CALL: PIN handling (after)
			if (togglePins) {
				rewriter.insertAfter(ctx.getStop(), ";\n"+togglePinCode);
			}
		} else {
			CParser.ArgumentExpressionListContext argCtx;
			CParser.AssignmentExpressionContext assignCtx;

			// Function call is *not* an API call. Add .values to all array arguments
			observedResourceType = ResourceType.NON_AMULET_API_FUNCTION_CALL;
			argCtx = ctx.argumentExpressionList();
			if (argCtx != null) {
				while (argCtx != null && argCtx.assignmentExpression() != null) {
					assignCtx = argCtx.assignmentExpression();
					if (arrays.contains(assignCtx.getText())) {
						rewriter.insertAfter(assignCtx.getStop(), ".values");
					}
					argCtx = argCtx.argumentExpressionList();
				}
			}
		}
		
		// Add any observed resources (e.g., Non-Amulet API/Amulet API function call, Sensor Subscription).
//		System.out.println("DEBUG enterFunctionCall()::"+ ctx.getText() + " [func=" + func_name + ", isApiCall=" + isApiCall + ", observerdResType=" + observedResourceType + "]");
		if ((resourceProfiler != null) && (observedResourceType != null)) {
			Resource record = new Resource(func_name, qmclass.getName(), observedResourceType, 0.0);
			resourceProfiler.add( record );
		}
	}

	///////////////////////  FUNCTION CALL HANDLERS  ///////////////////////

	/** 
	 * API CALL HANDLER: Given a call to use the AmuletStateTransition, 
	 * insert QP specific information (e.g., the Active Object) and Amulet audit 
	 * information (e.g., the App's ID) automatically. Also, verify that the signal 
	 * the app invokes is valid -- if it is, translate it in accordance with 
	 * our rules (i.e., prefix the signal with the name of the app).  
	 */
	private ST handler_isAmuletStateTransitionCall(CParser.FunctionCallContext ctx) {
		String func_name = ctx.Identifier().getText();

		if (func_name.equals("AmuletStateTransition")) {
			ST apiCall = templateGroup.getInstanceOf("apiCallStateTransition");
			apiCall.add("apiDef", ctx.Identifier().getText());
			apiCall.add("appName", qmclass.getName());

			// Verify that there is exactly one signal specified and that the 
			// caller has the necessary permission/scope to invoke this signal. 
			int nArgs = 0;
			if (ctx.argumentExpressionList() != null)
				nArgs = ctx.argumentExpressionList().getText().split(",").length;
			
			if (nArgs < 1) {
				apiCall.add("sig", "SIG_ERROR_TOO_FEW_ARGS");
			} else if (nArgs > 1) {
				apiCall.add("sig", "SIG_ERROR_TOO_MANY_ARGS");
			} else { // ctx.argumentExpressionList().size() == 1
				
				/*
				 * NOTE: AFT assumes that signal call has "_SIG" suffix.
				 */

				// Extract the "raw" signal argument.
				String sig = ctx.argumentExpressionList().getText();
				
				// Trim off suffix "_SIG" for upcoming comparison.
				String cmpSig = sig.substring(0, sig.lastIndexOf('_')); 

				// Check that this signal is within this app's "scope"
				if (qmclass.getSignals().contains(cmpSig)) {
					apiCall.add("sig", sig);
				} else {
					apiCall.add("sig", "SIG_ERROR_INVALID_ARG_"+sig);
				}
			}

			// Signify to the caller that this case was handled.
			return apiCall;
		}

		return null;
	}

	/** 
	 * API CALL HANDLER: Given a call to use the Amulet Log, insert the app's
	 * App ID and Log ID automatically. 
	 */
	private ST handler_isAmuletLogCall(CParser.FunctionCallContext ctx) {
		String func_name = ctx.Identifier().getText();

		if ((ctx.argumentExpressionList() != null) && func_name.contains("Log")) {
			// API Log Call w/ Args
			ST apiCall = templateGroup.getInstanceOf("apiLogCallWithArgs");
			apiCall.add("apiDef",ctx.Identifier().getText());
			apiCall.add("appLogId",this.appLogId);
			apiCall.add("arguments",ctx.argumentExpressionList().getText());

			// Signify to the caller that this case was handled.
			return apiCall;
		}

		return null;
	}

	/**
	 * API CALL HANDLER: Record the resources that are requested by applications 
	 * in order to identify which resources are needed and which ones are not 
	 * when we go to produce the resulting firmware image. Knowing this information 
	 * can allow us to only include necessary code in the firmware image and 
	 * can greatly reduce the size of the image in the event that certain 
	 * resources are not needed for a particular build.
	 */
	private ST handler_isAmuletTimerCall(CParser.FunctionCallContext ctx) {
		String func_name = ctx.Identifier().getText();

		if ((ctx.argumentExpressionList() != null) && func_name.contains("AmuletTimer")) {
			ST apiCall = templateGroup.getInstanceOf("apiCallWithArgs");
			apiCall.add("apiDef",ctx.Identifier().getText());
			apiCall.add("arguments",ctx.argumentExpressionList().getText());

			String timerDelayInSeconds = ctx.argumentExpressionList().getText().split(",")[0];
			Resource record = new Resource(func_name, qmclass.getName(), ResourceType.TIMER_SUBSCRIPTION, 0.0);
			record.addExtra(Resource.EXTRA_TIMER_DELAY_IN_SECONDS, timerDelayInSeconds);

			// Add the detected resource request to the list of requested resources.
			if (resourceProfiler != null) {
				resourceProfiler.add( record );
			}

			// Signify to the caller that this case was handled.
			return apiCall;
		}

		return null;
	}

	/**
	 * API CALL HANDLER: Record the resources that are requested by applications 
	 * in order to identify which resources are needed and which ones are not 
	 * when we go to produce the resulting firmware image. Knowing this information 
	 * can allow us to only include necessary code in the firmware image and 
	 * can greatly reduce the size of the image in the event that certain 
	 * resources are not needed for a particular build.
	 */
	private ST handler_isAmuletSubscribeCall(CParser.FunctionCallContext ctx) {
		String func_name = ctx.Identifier().getText();

		if ((ctx.argumentExpressionList() != null) && func_name.contains("AmuletSubscribe")) {
			ST apiCall = templateGroup.getInstanceOf("apiCallWithArgs");
			apiCall.add("apiDef",ctx.Identifier().getText());
			apiCall.add("arguments",ctx.argumentExpressionList().getText());

			String resource = ctx.argumentExpressionList().getText().split(",")[0];
			Resource record = new Resource(resource, qmclass.getName(), ResourceType.SENSOR_SUBSCRIPTION, 0.0);

			// Add the detected resource request to the list of requested resources.
			if (resourceProfiler != null) {
				resourceProfiler.add( record );
//				resourceList.add( record );
			}

			// Signify to the caller that this case was handled.
			return apiCall;
		}

		return null;
	}

	/**
	 * API CALL HANDLER: Special case: we need to be able to scale the 
	 * cost of rectangle-fillinf related calls because they are more expensive 
	 * as they handle larger rectangles. 
	 *
	 * This could also be applied to drawing lines if we want more precision
	 */
	private ST handler_isAmuletRectCall(CParser.FunctionCallContext ctx) {  // TODO: JOSIAH: ADD A NEW FUNCTION LIKE THIS FOR OTHER HANDLERS (AS NEEDED)
		String func_name = ctx.Identifier().getText();

		if ((ctx.argumentExpressionList() != null) && (func_name.contains("ClearRect") || func_name.contains("FillRect"))) { 
			// Setup the ST for the caller -- we won't tweak args or anything here
			// so this is pretty trivial. 
			ST apiCall = templateGroup.getInstanceOf("apiCallWithArgs");
			apiCall.add("apiDef",ctx.Identifier().getText());
			apiCall.add("arguments",ctx.argumentExpressionList().getText());

			// Parse out relevant args passed to this function call. 
			String[] args = ctx.argumentExpressionList().getText().split(",");
			String scaleFactor = args[0];
			String x = args[0];
			String y = args[1];
			String w = args[2];
			String h = args[3];

    		String[] t = new String[]{"BOLD_FONT_SIZE", "MEDIUM_FONT_SIZE", "REGULAR_FONT_SIZE", "LCD_HORIZONTAL_MAX", "LCD_VERTICAL_MAX"};
    		int[] vals = new int[]{22, 16, 12, 127, 127};
    		for(int i = 0;i<t.length;i++) {
    			w = w.replace(t[i], Integer.toString(vals[i]));
    			h = h.replace(t[i], Integer.toString(vals[i]));
    		}

    		// JAVA is stupid
    		try { 
		        Integer.parseInt(w); 
		    } catch(Exception e) { 		w = "127";   	 }
    		try { 
		        Integer.parseInt(h); 
		    } catch(Exception e) { 		h = "40";   	 }		    
    			double rectsize = Integer.parseInt(w) * Integer.parseInt(h);
    		
			// NOTE: IF you set the cost to be non-zero, the ARP will use *this value* to 
			//       scale the actual energy cost computed from the energy params from the device profile. 
			Resource record = new Resource(func_name, qmclass.getName(), ResourceType.AMULET_API_FUNCTION_CALL, 1.0);
			record.addExtra("width", w);
			record.addExtra("height", h);
			// Add the detected resource request to the list of requested resources.
			if (resourceProfiler != null) {
				resourceProfiler.add( record );
			}

			// Signify to the caller that this case was handled.
			return apiCall;
		}
		return null;
	}
	
	/**
	 * API CALL HANDLER: Generic handler for doing basic translation of app
	 * calls to the Amulet API. This handler specifically works for calls to 
	 * functions defined in the Amulet API that -have- arguments.
	 *
	 * Currently, this handler exists to automatically insert the App ID of the 
	 * caller as the final parameter to the function call.
	 */
	private ST handler_isOtherCallWithArguments(CParser.FunctionCallContext ctx) {
		if (ctx.argumentExpressionList() != null) {
			ST apiCall = templateGroup.getInstanceOf("apiCallWithArgs");
			apiCall.add("apiDef",ctx.Identifier().getText());
			apiCall.add("arguments",ctx.argumentExpressionList().getText());
			return apiCall;
		}

		return null;
	}

	/**
	 * API CALL HANDLER: Generic handler for doing basic translation of app
	 * calls to the Amulet API. This handler specifically works for calls to 
	 * functions defined in the Amulet API that -don't have- arguments.
	 *
	 * Currently, this handler exists to automatically insert the App ID of the 
	 * caller as the final parameter to the function call.
	 */
	private ST handler_isOtherCallWithNoArguments(CParser.FunctionCallContext ctx) {
		if (ctx.argumentExpressionList() == null) {
			ST apiCall = templateGroup.getInstanceOf("apiCallWithoutArgs");
			apiCall.add("apiDef",ctx.Identifier().getText());
			return apiCall;
		}

		return null;
	}

	////////////////////////////////////////////////////////////////////////

	@Override
	public void enterParameterDeclaration(CParser.ParameterDeclarationContext ctx)  {
		CParser.AbstractDeclaratorContext abstractCtx = ctx.abstractDeclarator();
		System.out.println("DEBUG enterParameterDeclaration():: " + abstractCtx.getText());
		CParser.DeclaratorContext declCtx = ctx.declarator();
		CParser.DeclarationSpecifiers2Context declSpec2Ctx;
		CParser.DeclarationSpecifiersContext declSpecCtx;
		List<CParser.DeclarationSpecifierContext> declSpecListCtx = null;
		CParser.TypeSpecifierContext typeSpecCtx = null;
		TerminalNode arrayDeclNode = null;
		//Check if array declaration or not
		if (abstractCtx != null && abstractCtx.getText().indexOf("[") != -1 && abstractCtx.getText().indexOf("]") != -1) {
			declSpec2Ctx = ctx.declarationSpecifiers2();
			arrayDeclNode = abstractCtx.directAbstractDeclarator().arrayDeclarator().Identifier();
			declSpecListCtx = declSpec2Ctx.declarationSpecifier();
			//rewriter.replace(ctx.getStart(),ctx.getStop(),"__"+declSpec2Ctx.getText()+"_array "+arrayDeclNode.getText());
		} else if (declCtx != null && declCtx.getText().indexOf("[") != -1 && declCtx.getText().indexOf("]") != -1) {
			declSpecCtx = ctx.declarationSpecifiers();
			arrayDeclNode = declCtx.directDeclarator().directDeclarator().Identifier();
			declSpecListCtx = declSpecCtx.declarationSpecifier();
			//rewriter.replace(ctx.getStart(),ctx.getStop(),"__"+declSpecCtx.getText()+"_array "+arrayDeclNode.getText());
		}
		if (declSpecListCtx != null) {
			for (CParser.DeclarationSpecifierContext declSpCtx : declSpecListCtx) {
				if (declSpCtx.typeSpecifier() != null) {
					typeSpecCtx = declSpCtx.typeSpecifier();
					break;
				}
			}
		}
		if (typeSpecCtx != null && arrayDeclNode != null) {
			rewriter.replace(ctx.getStart(),ctx.getStop(),"__"+typeSpecCtx.getText()+"_array "+arrayDeclNode.getText());
		}
	}

	@Override
	public void enterExpressionStatement(CParser.ExpressionStatementContext exprStctx)  {
		/*
		 * Update "basic block" count in For-loop Context.
		 *  + Rule: any expression without an amulet API function call.
		 *  
		 * NOTE: Right now we are trying to guard against double counting Amulet API function 
		 * calls so we do some primitive checking of the line of code for indicators that 
		 * this expression includes a call to such a function. 
		 * 
		 * TODO: we should make a better way of checking if there really is a call to an 
		 * amulet API function...
		 */
//		System.out.println("DEBUG>>>>>>>>>>enterExpressionStatement: " + exprStctx.getText() + " (" + resourceProfiler.loopContext.isForLoopContext() + ")");
		if (resourceProfiler != null && resourceProfiler.loopContext.isForLoopContext()) {
			if (!exprStctx.getText().contains("Amulet") || (!exprStctx.getText().contains("(") && !exprStctx.getText().contains(")"))) {
				resourceProfiler.incNumberStatementsInLoop();
//				System.out.println("     LOOP INCLUDE!");
			}
		}
		/*
		 * Update "basic block" count in QM Context (e.g., state, transition).
		 *  + Rule: any expression without an amulet API function call.
		 */
		else if (resourceProfiler != null && !resourceProfiler.loopContext.isForLoopContext()) {
			if (!exprStctx.getText().contains("Amulet") || (!exprStctx.getText().contains("(") && !exprStctx.getText().contains(")"))) {
				resourceProfiler.incNumLinesOfCode();
//				System.out.println("     QM INCLUDE!");
			}
		}
		
		if (exprStctx.expression().assignmentExpression().leftExpression() != null || exprStctx.expression().assignmentExpression().rightExpression() != null) {
			//Enter only if the expression statement is assignment expression with left and right hand sides
			CParser.AssignmentExpressionContext ctx = exprStctx.expression().assignmentExpression();
			CParser.LeftExpressionContext ctxLeft = ctx.leftExpression();
			CParser.RightExpressionContext ctxRight = ctx.rightExpression();
			CParser.AssignmentOperatorContext ctxAssignOp = ctx.assignmentOperator();
			CParser.CastExpressionContext ctxLeftCast = ctxLeft.conditionalExpression().logicalOrExpression().logicalAndExpression().inclusiveOrExpression().exclusiveOrExpression().andExpression().equalityExpression().relationalExpression().shiftExpression().additiveExpression().multiplicativeExpression().castExpression();
//			CParser.CastExpressionContext ctxRightCast = ctxRight.conditionalExpression().logicalOrExpression().logicalAndExpression().inclusiveOrExpression().exclusiveOrExpression().andExpression().equalityExpression().relationalExpression().shiftExpression().additiveExpression().multiplicativeExpression().castExpression();

			//////////////////////////////////////////////////////////////////////////
//			System.out.println("            + left::"+ctxLeft.getText());
//			System.out.println("            + op::"+ctxAssignOp.getText());
//			System.out.println("            + right::"+ctxRight.getText());
			//////////////////////////////////////////////////////////////////////////
			
			if (ctxLeft.getText().indexOf("[") != -1 && ctxLeft.getText().indexOf("]") != -1) {
				//Array assignment statement (i.e., assign INTO array index)...
				ST arrayAssignment = templateGroup.getInstanceOf("arrayAssignment");
				arrayAssignment.add("arrayName",ctxLeftCast.unaryExpression().postfixExpression().postfixExpression().getText());
				arrayAssignment.add("arrayIndex",ctxLeftCast.unaryExpression().postfixExpression().expression().getText());
				arrayAssignment.add("assignmentOp",ctxAssignOp.getText());
				if (ctxRight.getText().indexOf("[") != -1 && ctxRight.getText().indexOf("]") != -1) {
					//... array assignment statement with AND array access
					
					/*ST arrayAssignmentWithArrayAccess = templateGroup.getInstanceOf("arrayAssignmentWithArrayAccess");
					arrayAssignmentWithArrayAccess.add("arrayName",ctxLeftCast.unaryExpression().postfixExpression().postfixExpression().getText());
					arrayAssignmentWithArrayAccess.add("arrayIndex",ctxLeftCast.unaryExpression().postfixExpression().expression().getText());
					arrayAssignmentWithArrayAccess.add("assignmentOp",ctxAssignOp.getText());
					arrayAssignmentWithArrayAccess.add("arrayToBeRead",ctxRightCast.unaryExpression().postfixExpression().postfixExpression().getText());
					arrayAssignmentWithArrayAccess.add("arrayReadIndex",ctxRightCast.unaryExpression().postfixExpression().expression().getText());
					rewriter.insertAfter(exprStctx.getStop(),arrayAssignmentWithArrayAccess.render());*/

					InputStream is = new ByteArrayInputStream(ctxRight.getText().getBytes());
					ANTLRInputStream input;
					try {
						input = new ANTLRInputStream(is);
					} catch (Exception e) {
						return;
					}
					CLexer lexer = new CLexer(input);
		            CommonTokenStream tokens = new CommonTokenStream(lexer);
		            CParser parser = new CParser(tokens);
		            ParseTree tree = parser.expression(); // parse
		                
		            ParseTreeWalker walker = new ParseTreeWalker(); // create standard walker
		            CExtractor extractor = new CExtractor(qmclass, tokens, parser, togglePins, global_arrays, resourceProfiler);
		            walker.walk(extractor, tree); // initiate walk of tree with listener

		            arrayAssignment.add("value",extractor.rewriter.getText());
				} else {
					//... only array assignment statement (and NO array access)
					
					/*ST arrayAssignment = templateGroup.getInstanceOf("arrayAssignment");
					arrayAssignment.add("arrayName",ctxLeftCast.unaryExpression().postfixExpression().postfixExpression().getText());
					arrayAssignment.add("arrayIndex",ctxLeftCast.unaryExpression().postfixExpression().expression().getText());*/
					arrayAssignment.add("value",ctxRight.getText());
					/*arrayAssignment.add("assignmentOp",ctxAssignOp.getText());
					rewriter.insertAfter(exprStctx.getStop(),arrayAssignment.render());*/
				}
				rewriter.insertAfter(exprStctx.getStop(),arrayAssignment.render());
			} else if (ctxRight.getText().indexOf("[") != -1 && ctxRight.getText().indexOf("]") != -1) {
				//Assignment statement from array accessed index.

				/*ST arrayAccess = templateGroup.getInstanceOf("arrayAccess");
				arrayAccess.add("arrayName",ctxRightCast.unaryExpression().postfixExpression().postfixExpression().getText());
				arrayAccess.add("arrayIndex",ctxRightCast.unaryExpression().postfixExpression().expression().getText());
				rewriter.insertAfter(exprStctx.getStop(),ctxLeft.getText()+ctxAssignOp.getText()+arrayAccess.render()+";");*/
			} else {
				// Other Assignment (i.e., assign to non-array variable AND NO array access).
				
				// Add (*ONLY*) assignment statements that assign int values to a variable to the ResourceProfiler's intVarResolver.
				try {
					Integer.parseInt(ctxRight.getText());
					resourceProfiler.setResolverIntVar(ctxLeft.getText(), ctxRight.getText());
				} catch (NumberFormatException nfe) {
					//pass
//					System.out.println("WARNING: right val not an int value::" + ctxRight.getText());
				}
			}
		}
	}

	@Override
	public void exitExpressionStatement(CParser.ExpressionStatementContext exprStctx)  {
		if (exprStctx.expression().assignmentExpression().leftExpression() != null || exprStctx.expression().assignmentExpression().rightExpression() != null) {
			//Enter only if he expression statement is assignment expression with left and right hand sides
			CParser.AssignmentExpressionContext ctx = exprStctx.expression().assignmentExpression();
			CParser.LeftExpressionContext ctxLeft = ctx.leftExpression();
			CParser.RightExpressionContext ctxRight = ctx.rightExpression();
//			CParser.AssignmentOperatorContext ctxAssignOp = ctx.assignmentOperator();
			if (ctxLeft.getText().indexOf("[") != -1 && ctxLeft.getText().indexOf("]") != -1) {
				//Array assignment is being done
				if (ctxRight.getText().indexOf("[") != -1 && ctxRight.getText().indexOf("]") != -1) {
					//Array assignment and array access
					rewriter.delete(exprStctx.getStart(), exprStctx.getStop());
				} else {
					//Just array assignment
					rewriter.delete(exprStctx.getStart(), exprStctx.getStop());
				}
			} else if (ctxRight.getText().indexOf("[") != -1 && ctxRight.getText().indexOf("]") != -1) {
				//Just array access
				//rewriter.delete(exprStctx.getStart(), exprStctx.getStop());
			}
		}
	}

	@Override
	public void enterDeclaration(CParser.DeclarationContext ctx)  {
		/*
		 * Update "basic block" count in For-loop Context.
		 *  + Rule: any declaration without an amulet API function call.
		 *  
		 * NOTE: Right now we are trying to guard against double counting Amulet API function 
		 * calls so we do some primitive checking of the line of code for indicators that 
		 * this expression includes a call to such a function. 
		 * 
		 * TODO: we should make a better way of checking if there really is a call to an 
		 * amulet API function...
		 */
//		System.out.println("DEBUG>>>>>>>>>>enterDeclaration: " + ctx.getText() + " (" + resourceProfiler.loopContext.isForLoopContext() + ")");
		if (resourceProfiler != null && resourceProfiler.loopContext.isForLoopContext()) {
			if (!ctx.getText().contains("Amulet") || (!ctx.getText().contains("(") && !ctx.getText().contains(")"))) {
				if (resourceProfiler.loopContext.isForLoopContext()) {
					resourceProfiler.incNumberStatementsInLoop();
//					System.out.println("     LOOP INCLUDE!");
				}
			}
		}
		/*
		 * Update "basic block" count in QM Context (e.g., state, transition).
		 *  + Rule: any declaration without a function call.
		 */
		else if (resourceProfiler != null && !resourceProfiler.loopContext.isForLoopContext()) {
			if (!ctx.getText().contains("Amulet") || (!ctx.getText().contains("(") && !ctx.getText().contains(")"))) {
				// Rule: any expression with an assignment statement.
				resourceProfiler.incNumLinesOfCode();
//				System.out.println("     QM INCLUDE!");
			}
		}
		
		if (ctx.declarationSpecifiers().getText().indexOf("free") != -1 && ctx.initDeclaratorList() != null) {
			CParser.DeclaratorContext declaratorCtx = ctx.initDeclaratorList().initDeclarator().declarator();
			if (declaratorCtx != null && declaratorCtx.getText().indexOf("[") != -1 && declaratorCtx.getText().indexOf("]") != -1) {
				//Enter only if it has array declaration
				declaratorCtx = declaratorCtx.directDeclarator().declarator();
				rewriter.replace(ctx.getStart(),ctx.getStop(),"free("+declaratorCtx.directDeclarator().directDeclarator().getText()+".values["+declaratorCtx.directDeclarator().assignmentExpression().getText()+"]);");
			}

		} else if (ctx.initDeclaratorList() != null) {
			CParser.DeclaratorContext declaratorCtx = ctx.initDeclaratorList().initDeclarator().declarator();
			//Enter only if it has array declaration
			if (declaratorCtx != null && declaratorCtx.getText().indexOf("[") != -1 && declaratorCtx.getText().indexOf("]") != -1) {
				Vector<CParser.InitDeclaratorContext> arrayContexts = new Vector<CParser.InitDeclaratorContext>();
				String variables = "";
				CParser.InitDeclaratorContext initDeclCtx;
				CParser.InitDeclaratorListContext initDeclLstctx = ctx.initDeclaratorList();
				//Loop through all initDeclaratorList and add arrays into vector and variables into string
				while (initDeclLstctx.initDeclaratorList() != null) {
					if (initDeclLstctx.initDeclarator().declarator().getText().indexOf("[") != -1 && initDeclLstctx.initDeclarator().declarator().getText().indexOf("]") != -1) {
						initDeclCtx = initDeclLstctx.initDeclarator();
						arrayContexts.add(initDeclCtx);
					} else {
						variables += initDeclLstctx.initDeclarator().getText()+',';
					}
					initDeclLstctx = initDeclLstctx.initDeclaratorList();
				}
				if (initDeclLstctx.initDeclarator().getText().indexOf("[") != -1 && initDeclLstctx.initDeclarator().getText().indexOf("]") != -1) {
					initDeclCtx = initDeclLstctx.initDeclarator();
					arrayContexts.add(initDeclCtx);
				} else {
					variables += initDeclLstctx.initDeclarator().getText()+',';
				}
				String arrayDecl = "";
//				String arrayInitializer = "";
				CParser.DirectDeclaratorContext directDeclCtx;
				ST arrayDeclaration = templateGroup.getInstanceOf("arrayDeclaration");
				for (CParser.InitDeclaratorContext declCtx : arrayContexts) {
					arrayDeclaration = templateGroup.getInstanceOf("arrayDeclaration");
					directDeclCtx = declCtx.declarator().directDeclarator();
					String arrayName = directDeclCtx.directDeclarator().getText();
					String arraySize = directDeclCtx.assignmentExpression().getText();
					String arrayType = ctx.declarationSpecifiers().getText();
					//TODO: What if initializer has an array read. The previous version was failing to do that too.
					if (declCtx.initializer() != null) {
						arrayDeclaration = templateGroup.getInstanceOf("arrayDeclarationWithInit");
						arrayDeclaration.add("arrayInit", declCtx.initializer().getText());
					}

					arrays.add(arrayName);

					// Add memory resource to profiler (array declaration).
					resourceProfiler.addMemoryResource(qmclass.getName(), arrayType, arrayName, arraySize, ResourceType.MEMORY);

					arrayDeclaration.add("arrayType", ctx.declarationSpecifiers().getText());
					arrayDeclaration.add("arrayName", arrayName);
					arrayDeclaration.add("arraySize", arraySize);
					arrayDecl += arrayDeclaration.render()+"\n";
					arrayDeclaration.remove("arrayType");
					arrayDeclaration.remove("arrayName");
					arrayDeclaration.remove("arraySize");
				}
				
				//Insert variables in the beginning
				if (variables.length() > 0) {
					rewriter.replace(ctx.getStart(), ctx.getStop(), ctx.declarationSpecifiers().getText()+" "+variables.substring(0, variables.length()-1)+";");
					if (arrayContexts.size() > 0) {
						rewriter.insertAfter(ctx.getStop(), arrayDecl);
					}
				} else {
					if (arrayContexts.size() > 0) {
						rewriter.replace(ctx.getStart(), ctx.getStop(), arrayDecl);
					}
				}
			} else {
				// Add memory resource to profiler (not an array but still a declaration so we need to count for resource profiler).
				resourceProfiler.addMemoryResource(qmclass.getName(), ctx.declarationSpecifiers().getText() , declaratorCtx.getText(), "1", ResourceType.MEMORY);
			}
		}
	}

	@Override
	public void enterUnaryExpression(CParser.UnaryExpressionContext ctx)  {
		if (ctx.getText().indexOf("[") != -1 && ctx.getText().indexOf("]") != -1 && ctx.postfixExpression() != null) {
			if (ctx.postfixExpression().postfixExpression() != null) {
				CParser.PostfixExpressionContext postfixCtx = ctx.postfixExpression();
				ST arrayAccess = templateGroup.getInstanceOf("arrayAccess");
				arrayAccess.add("arrayName",postfixCtx.postfixExpression().getText());
				arrayAccess.add("arrayIndex",postfixCtx.expression().getText());
				rewriter.replace(ctx.getStart(),ctx.getStop(),arrayAccess.render());
			}
		} else if (ctx.getText().indexOf("sizeof") != -1 && ctx.unaryExpression() != null) {
			if (ctx.unaryExpression().postfixExpression() != null) {
				if (ctx.unaryExpression().postfixExpression().primaryExpression() != null) {
					CParser.ExpressionContext exprCtx = ctx.unaryExpression().postfixExpression().primaryExpression().expression();
					if (exprCtx != null) {
						if (arrays.contains(exprCtx.getText())) {
							rewriter.insertAfter(exprCtx.getStop(), ".values");
						}
					}
				}
			}
		}
	}
	
	@Override 
	public void enterIterationStatement(CParser.IterationStatementContext ctx) { 
		/*
		 * This only supports very basic for-loops right now! (e.g., well-defined for(...;...;...) loops; 
		 * start at MIN and go up to, but not including, MAX; increment by 1).
		 * 
		 * TODO: Support while loops
		 * TODO: Support nested loop structures. 
		 * TODO: 'update expression' parsing/handling: parse "++" as "1".
		 * TODO: 'update expression' parsing/handling: parse "+= N" as "N".
		 * TODO: 'update expression' parsing/handling: parse "--" as "-1".
		 * TODO: 'update expression' parsing/handling: parse "-= N" as "-N".
		 */
		String loopType = ctx.start.getText();

		if (loopType.equals("for")) {
			// Java objects for for-loop definition.
			DeclarationContext declarationCtx = ctx.declaration();
			ExpressionContext conditionCtx    = ctx.expression(0);
//			ExpressionContext updateCtx       = ctx.expression(1);

			/*
			 * Parse/resolve strings for the loop bounds to count iterations. Then, 
			 * enter the "for-loop context". All records added to the Resource Profiler will 
			 * be annotated as being within the for-loop for more accurate accounting. 
			 */
			
			// Update for-loop bounds based on (a VERY simple) ResourceProfiler int variable resolver...
			String start = declarationCtx.initDeclaratorList().stop.getText();
			if (resourceProfiler.getResolverIntVar(start) != null) {
				start = resourceProfiler.getResolverIntVar(start);
			}
			
			String end = conditionCtx.assignmentExpression().conditionalExpression().logicalOrExpression().logicalAndExpression().inclusiveOrExpression().exclusiveOrExpression().andExpression().equalityExpression().relationalExpression().shiftExpression().additiveExpression().multiplicativeExpression().castExpression().getText();
			if (resourceProfiler.getResolverIntVar(end) != null) {
				end = resourceProfiler.getResolverIntVar(end);
			}
			
			String update = "1";
//			if (resourceProfiler.getResolverIntVar(update) != null) {
//				update = resourceProfiler.getResolverIntVar(update);
//			}

			// Inform the ResourceProfiler that we've entered a for-loop.
			resourceProfiler.enterForLoopContext(start, end, update);
		} else {
			System.err.println("\nCExtractor.java:: ***LOOP-TYPE '" + loopType + "' NOT SUPPORTED UNDER RESOURCE PARSING***\n");
		}
	}

	@Override 
	public void exitIterationStatement(CParser.IterationStatementContext ctx) { 
	    // Add a general for-loop (COMPUTATION) resource for recording "basic blocks".
		if (resourceProfiler != null) {
			Resource record = new Resource(ComputationType.FOR_LOOP.text(), qmclass.getName(), ResourceType.COMPUTATION, 0.0);
			resourceProfiler.add( record );
		}

		// Inform the ResourceProfiler that we've exited the for-loop.
	    resourceProfiler.exitForLoopContext();
	}
}

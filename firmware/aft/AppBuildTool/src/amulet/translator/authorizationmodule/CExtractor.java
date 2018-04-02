package amulet.translator.authorizationmodule;

import org.antlr.v4.runtime.*;
import java.util.*;

import amulet.translator.CParser;
import amulet.translator.ErrorMsg;
import amulet.translator.CBaseListener;

public class CExtractor extends CBaseListener {
    TokenStreamRewriter rewriter;
    TokenStream tokens;
    CParser parser;
	
	Vector<ErrorMsg> m_errors;
	Vector<ErrorMsg> m_warnings;

	Vector<String> authorizedApiList;
	HashMap<String, List<String>> firstArgumentMap;	
	
    public CExtractor(TokenStream tokens, CParser parser, Vector<String> apiList, HashMap<String, List<String> > argumentMap) {
		rewriter = new TokenStreamRewriter(tokens);
		this.tokens = tokens;
		this.parser = parser;

		m_errors = new Vector<ErrorMsg>();
		m_warnings = new Vector<ErrorMsg>();
		authorizedApiList = apiList;
		firstArgumentMap = argumentMap;
    }
	
	//getters and setters for retrieving messages after translation
	public List<ErrorMsg> getErrors() { return m_errors;}
	
	public List<ErrorMsg> getWarnings() { return m_warnings; }
	
	public void enterFunctionCall(CParser.FunctionCallContext ctx) {
		boolean isApiCall = false;
		String func_name = ctx.Identifier().getText();
		
		/*
		 * Verify that, if the function call is an Amulet API Calls, that it is included 
		 * in the list of API calls this application is authorized to make.
		 */
		isApiCall = (func_name.startsWith("Amulet"));
		if(isApiCall && !authorizedApiList.contains(func_name)) {
			// ERROR: this function is *NOT* authorized for this application!
			m_errors.add(new ErrorMsg("This app is not authorized to make "+func_name+" API Call",ctx.getStart().getLine()));
			
		} else if (isApiCall && ctx.argumentExpressionList() != null) {
			/*
			 * TODO: Our old models/assumptions only required checking the first argument -- we should, 
			 * in the future, support checking multiple arguments provided in the API call. 
			 */
			
			// Verify that this API call has passed an acceptable (first) parameter. 
			String[] arguments = ctx.argumentExpressionList().getText().split(",");
			String firstArg = arguments[0].trim();
			if (firstArgumentMap.get(func_name) != null && !firstArgumentMap.get(func_name).contains(firstArg)) {
				System.out.println(ctx.argumentExpressionList().getText());
				m_errors.add(new ErrorMsg("Amulet does not allow "+func_name+" without one of "+firstArgumentMap.get(func_name)+" as first argument",ctx.getStart().getLine()));
			}
		}
	}
	
}
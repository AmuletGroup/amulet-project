package amulet.translator.compatibilitychecker;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import java.util.*;
import java.util.logging.*;

import amulet.translator.CParser;
import amulet.translator.ErrorMsg;
import amulet.translator.CBaseListener;

public class CExtractor extends CBaseListener {
    TokenStreamRewriter rewriter;
    TokenStream tokens;
    CParser parser;
    String className;
	
	//call graph (func_a -> func_b) //hashmap -> set
	//error list
	//warning list
	Stack<ParserRuleContext> m_scope;
	HashMap<String, Set<String>> m_callgraph;
	Vector<ErrorMsg> m_errors;
	Vector<ErrorMsg> m_warnings;
	Vector<ErrorMsg> m_messages;
	
	
	private final static Level logLevel = Level.OFF;
	private final static Logger LOGGER = Logger.getLogger(CExtractor.class.getName());
	
	
	
	
    public CExtractor(TokenStream tokens, CParser parser, String className) {
		rewriter = new TokenStreamRewriter(tokens);
		this.parser = parser;
		this.tokens = tokens;
		this.className = className;

		m_errors = new Vector<ErrorMsg>();
		m_warnings = new Vector<ErrorMsg>();
		m_messages = new Vector<ErrorMsg>();
		m_scope = new Stack<ParserRuleContext>();
		m_callgraph = new HashMap<String, Set<String>>();
		
		//set log level -- seems like it should be simpler than this.
		Handler consoleHandler = new ConsoleHandler();
		consoleHandler.setLevel(logLevel);
		LOGGER.addHandler(consoleHandler);
		LOGGER.setLevel(logLevel);
    }
	
	//getters and setters for retrieving messages after translation
	public List<ErrorMsg> getErrors()
		{ return m_errors;}
	
	public List<ErrorMsg> getWarnings()
		{ return m_warnings; }
	
	//check for recursive functions, recursively, of course. :)
	public void doRecursionCheck()
	{
		for (String func : m_callgraph.keySet())
		{
			Stack<String> calls = new Stack<String>();
			calls.add(func);
			if (isRecursive(func,calls))
			{
				m_errors.add(new ErrorMsg("Amulet does not support recursion (function \""+func+"\" is recursive or contains a recursive call)", -1));
			}
		}
	}
	
	public boolean isRecursive(String func_name, Stack<String> calls_so_far)
	{
		Set<String> next_calls = m_callgraph.get(func_name);
		if (next_calls == null) return false;
		
		/*for (int i=0; i < calls_so_far.size(); i++)
		{
			System.out.print("\t");
		}
		System.out.print("iR "+func_name +" : ");*/
		for (String call : next_calls)
		{
			//System.out.print(call+" -> ");
			if (calls_so_far.contains(call)) return true;
			calls_so_far.push(call);
			if (isRecursive(call, calls_so_far)) return true;
			calls_so_far.pop();
		}
		return false;
	}
	
	
	
	////Listener methods for translating the AST during the walk of the parse tree.

	public void enterFunctionDefinition(CParser.FunctionDefinitionContext ctx) 
	{ 
		LOGGER.fine("FUNC -> "+ctx.getText()+" "+ctx.children.size()+" children");
		m_scope.push(ctx);
		String[] apiPrefixes = {"AmuletGetAccelValue", "AmuletGetStdDev", "AmuletSubscribeInternalSensor", "AmuletGetHR", "AmuletTimer", "AmuletActuate", "AmuletDisplayMessage", "AmuletDisplayClr", "AmuletVarWriteUint32", "AmuletLogAppend", "AmuletLogRead", "AmuletPutData", "AmuletStateTransition" , "UARTprintf"};
		List<String> apiPrefixList = Arrays.asList(apiPrefixes);
		String func_name = ctx.Identifier().getText();
		if (apiPrefixList.contains(func_name)) {
			m_errors.add(new ErrorMsg("Amulet does not allow "+func_name+" as defined function name",ctx.getStart().getLine()));
		}
	}
	
	public void exitFunctionDefinition(CParser.FunctionDefinitionContext ctx) 
	{ 
		LOGGER.fine("FUNC <- "+ctx.getText()+" "+ctx.children.size()+" children");
		rewriter.insertAfter(ctx.getStop(), "\n");
		
		ParserRuleContext prc = m_scope.pop();
		if (ctx != prc) 
		{
			throw new RuntimeException("BUG! AAAAH. Scope problems.");
		}
//		String func_name = ctx.Identifier().getText();
//		Set<String> calls = m_callgraph.get(func_name);
	}
	
	public void enterDeclarator(CParser.DeclaratorContext ctx) { 
		LOGGER.fine("DECL -> "+ctx.getText());
	}

	public void enterDirectDeclarator(CParser.DirectDeclaratorContext ctx) { 
		CParser.AssignmentExpressionContext assExpCtx = ctx.assignmentExpression();
		if (assExpCtx != null && ctx.getText().indexOf("[") != -1 && ctx.getText().indexOf("]") != -1) {
			//Declarator of an array detected. Check if length is an integer or not
			String arrLen = assExpCtx.getText();
			try {
		        int len = Integer.parseInt(arrLen);
		        if (len <= 0) {
					m_errors.add(new ErrorMsg("Amulet does not allow zero or negative sized arrays like "+ctx.getText(),ctx.getStart().getLine()));
		        }
		    } catch(NumberFormatException e) {
		        // Array length isn't an integer. We allow only static sized arrays
		        m_errors.add(new ErrorMsg("Amulet does not allow dynamic sized arrays like "+ctx.getText(),ctx.getStart().getLine()));
		    }

		}
	}
	
	public void enterJumpStatement(CParser.JumpStatementContext ctx) 
	{ 
		ParseTree firstchild = ctx.getChild(0);
		LOGGER.info("JMP -> "+firstchild);
		if (firstchild.getText().equals("goto"))
		{
			LOGGER.severe("Found a goto on line "+ctx.getStart().getLine()+"!");
			m_errors.add(new ErrorMsg("Amulet does not allow \"goto\" statements",ctx.getStart().getLine()));
		}
	}
	
	public void enterJustUnion(CParser.JustUnionContext ctx) 
	{ 
		LOGGER.severe("Found a union on line "+ctx.getStart().getLine()+"!");
		m_errors.add(new ErrorMsg("Amulet does not allow unions! Please remove them",ctx.getStart().getLine()));
	}
	
	public void enterPointer(CParser.PointerContext ctx) 
	{
		LOGGER.severe("Found a pointer on line "+ctx.getStart().getLine()+"!");
		m_errors.add(new ErrorMsg("Amulet does not allow pointers! Please remove them",ctx.getStart().getLine()));
	 }
	 
	public void enterUnaryAddrOperator(CParser.UnaryAddrOperatorContext ctx) 
	{
		LOGGER.severe("Found a unary address operator on line "+ctx.getStart().getLine()+"!");
		m_errors.add(new ErrorMsg("Amulet does not allow pointers! This includes unary address operators. Please remove them",ctx.getStart().getLine()));
	}
}
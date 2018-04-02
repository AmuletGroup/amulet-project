package amulet.translator.compatibilitychecker;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.*;

import java.util.*;

import amulet.translator.CLexer;
import amulet.translator.CParser;
import amulet.translator.ErrorMsg;

public class CompatChecker {
	private boolean success = false;
    
	public void printMsgs(String hdr, List<ErrorMsg> messages)
	{
		if (messages == null || messages.size() <= 0) return;
		System.err.println(hdr);
		//there were error messages
		for (ErrorMsg e : messages)
		{
			System.err.println("\t"+e);
		}
	}

	public CompatChecker(String code, String className)
	{
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
        CExtractor extractor = new CExtractor(tokens, parser, className);
        walker.walk(extractor, tree); // initiate walk of tree with listener
		extractor.doRecursionCheck();

		printMsgs("WARNINGS",extractor.getWarnings());
		printMsgs("ERRORS",extractor.getErrors());
		
		if (extractor.getErrors().size() > 0) {
			System.err.println("Incompatible code found.");
			setSuccess(false);
		} else {
			setSuccess(true);
		}
	}

	public void setSuccess(boolean val) {
		success = val;
	}

	public boolean getSuccess() {
		return success;
	}
    
}

package amulet.translator.authorizationmodule;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.*;

import java.util.*;

import amulet.manifestreader.ManifestReader;
import amulet.translator.CLexer;
import amulet.translator.CParser;
import amulet.translator.ErrorMsg;

public class AuthorizationModule {
	Vector<String> authorizedApiList;
	HashMap<String, List<String> > firstArgumentMap;
	private boolean success;
    
	public AuthorizationModule(ManifestReader manifestReader) {
		authorizedApiList = manifestReader.getAuthorizedApiList();
		firstArgumentMap = manifestReader.getAuthorizedApiArgumentMap();
	}
	
	public void printMsgs(String hdr, List<ErrorMsg> messages) {
		if (messages == null || messages.size() <= 0) return;
		System.err.println(hdr);
		
		//there were error messages
		for (ErrorMsg e : messages) {
			System.err.println("\t"+e);
		}
	}

	public boolean checkApiAuthorization(String code) {
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
        CExtractor extractor = new CExtractor(tokens, parser, authorizedApiList, firstArgumentMap);
        walker.walk(extractor, tree); // initiate walk of tree with listener

		printMsgs("WARNINGS",extractor.getWarnings());
		printMsgs("ERRORS",extractor.getErrors());
		
		if (extractor.getErrors().size() > 0) {
			System.err.println("Incompatible code found.");
			setSuccess(false);
			return false;
		} else {
			setSuccess(true);
			return true;
		}
	}

	public void setSuccess(boolean val) {
		success = val;
	}

	public boolean getSuccess() {
		return success;
	}
    
}

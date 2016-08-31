/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.translator.attributemapper;

import java.util.Vector;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

import amulet.translator.CLexer;
import amulet.translator.CParser;
import amulet.translator.attributemapper.CExtractor;

public class AttributeMapper {
	CExtractor m_extractor = null;
	
	public AttributeMapper(String code, Vector<String> attributes)
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
        m_extractor = new CExtractor(tokens, parser, attributes);
        walker.walk(m_extractor, tree); // initiate walk of tree with listener
	}

	public String getMappedCode() {
		return m_extractor.getText();
	}
}

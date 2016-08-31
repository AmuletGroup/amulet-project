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
import org.antlr.v4.runtime.tree.*;

import java.util.*;

import amulet.qm.QMClass;
import amulet.resourceprofiler.ResourceProfiler;
import amulet.translator.CLexer;
import amulet.translator.CParser;

public class RuntimeCheck {
	String checkedCode = "";
	String togglePinCode = "P1OUT |= BIT2; // on\n{volatile uint8_t i=0;\nfor(i;i<200;i++);}\nP1OUT &= (~BIT2); // off";

	public RuntimeCheck(QMClass qmclass, String code, boolean toggle, Vector<String> global_arrays, ResourceProfiler resourceProfiler)
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
        CExtractor extractor = new CExtractor(qmclass, tokens, parser, toggle, global_arrays, resourceProfiler);
        walker.walk(extractor, tree); // initiate walk of tree with listener
        if (toggle) {
        	checkedCode = togglePinCode+"\n";
        }
		checkedCode += extractor.rewriter.getText();
		if (toggle) {
			checkedCode += "\n"+togglePinCode;
		}
	}

	public String getCheckedCode() {
		return checkedCode;
	}
    
}

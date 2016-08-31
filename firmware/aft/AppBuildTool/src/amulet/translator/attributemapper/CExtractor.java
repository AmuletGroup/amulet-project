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

import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.TokenStreamRewriter;
import org.antlr.v4.runtime.misc.NotNull;

import amulet.translator.CBaseListener;
import amulet.translator.CParser;

public class CExtractor extends CBaseListener {
	TokenStreamRewriter rewriter;
    TokenStream tokens;
    CParser parser;
    Vector<String> m_attributes;
	
	Vector<String> functionDefinitions;
	Vector<String> functionCalls;
	
    public CExtractor(TokenStream tokens, CParser parser, Vector<String> attributes) {
		rewriter = new TokenStreamRewriter(tokens);
		this.parser = parser;
		this.tokens = tokens;
		m_attributes = attributes;
		
    }
    
    public String getText()
	{
		return rewriter.getText();
	}


    ////Listener method for translating the AST during the walk of the parse tree.
    public void enterPostfixExpression(@NotNull CParser.PostfixExpressionContext ctx) 
    { 
    	for (String attrib: m_attributes)
    	{
    		if (ctx.getText().equals(attrib))
    		{
    			rewriter.insertBefore(ctx.getStart(), "me->");
    			return;
    		}
    	}
    	
    }
	
	

}

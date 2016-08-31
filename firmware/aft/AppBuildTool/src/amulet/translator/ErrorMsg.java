/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.translator;

public class ErrorMsg
{
	String msg;
	int line;
	
	public ErrorMsg(String m, int l)
	{
		msg = m;
		line = l;
	}
	
	public String toString()
	{
		return "ERROR (line "+line+"): "+msg+".";
	}
}
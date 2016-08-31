/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.fsm;

import java.util.Vector;

public interface State {
	Vector<Transition> getTransitions();
	String getName();
	String getEntryCode();
	void setEntryCode(String code);
	String getExitCode();
	void setExitCode(String code);
}

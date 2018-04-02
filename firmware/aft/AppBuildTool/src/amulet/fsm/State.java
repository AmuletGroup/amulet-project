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

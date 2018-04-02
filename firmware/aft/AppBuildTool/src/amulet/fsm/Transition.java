package amulet.fsm;

public interface Transition {
	String getTrigger();
	State getTarget();
	State getSource();
}

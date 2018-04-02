package amulet.qm;

import java.util.HashMap;
import java.util.Vector;

import javax.xml.xpath.XPathExpressionException;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import amulet.fsm.*;

public class QMState implements amulet.fsm.State{
	private static HashMap<Node,State> objCache;
	
	private Node m_node;
	
	public static State getState(Node xmlnode)
	{
		if (objCache == null) { objCache = new HashMap<Node,State>(); }
		
		//lookup state first, if not there, then create it
		if (objCache.containsKey(xmlnode)) {
			return objCache.get(xmlnode);
		} else {
			try {
				QMState newstate = new QMState(xmlnode, QMXPath.getXPathNode("..", xmlnode));
				objCache.put(xmlnode, newstate);
				return newstate;
			} catch (XPathExpressionException e) {
				e.printStackTrace();
				return null;
			}
		}
	}
	
	private QMState(Node xmlnode, Node statechart) 
	{
		m_node = xmlnode;

		//add verification info here.
		verify();
	}
	
	private boolean verify()
	{
		if (m_node == null) return false;
		if (getName() == null || getName().equals("")) return false;
		return true;
	}

	public String getName()
	{
		return m_node.getAttributes().getNamedItem("name").getNodeValue();
	}
	
	public Vector<Transition> getTransitions() 
	{
		Vector<Transition> transitions = new Vector<Transition>();
		
		// Regular transition nodes.
		NodeList tnodes;
		try {
			tnodes = QMXPath.getXPathList("./tran", m_node);
		} catch (XPathExpressionException e) {
			e.printStackTrace();
			return null;
		}
		
		for (int i=0; i < tnodes.getLength(); i++) {
			transitions.add(new QMTransition(this, tnodes.item(i)));
		}
		
		return transitions;
	}
	
	@Override
	public String getEntryCode() {
		try {
			Node n = QMXPath.getXPathNode("./entry", m_node);
			return n.getTextContent();
		} catch (XPathExpressionException e) {
			e.printStackTrace();
			return null;
		} catch (NullPointerException e) {
			return null;
		}
	}

	@Override
	public void setEntryCode(String code) {
		try {
			Node n = QMXPath.getXPathNode("./entry", m_node);
			n.setTextContent(code);
		} catch (XPathExpressionException e) {
			e.printStackTrace();
		} catch (NullPointerException e) {
		}
	}

	@Override
	public String getExitCode() {
		try {
			Node n = QMXPath.getXPathNode("./exit", m_node);
			return n.getTextContent();
		} catch (XPathExpressionException e) {
			e.printStackTrace();
			return null;
		} catch (NullPointerException e) {
			return null;
		}
	}

	@Override
	public void setExitCode(String code) {
		try {
			Node n = QMXPath.getXPathNode("./exit", m_node);
			n.setTextContent(code);
		} catch (XPathExpressionException e) {
			e.printStackTrace();
		} catch (NullPointerException e) {
		}
	}
	
	public String toString()
	{
		String result = "QMState: " + getName() + "\n";

		 result += "\nTransitions\n";
		 Vector<Transition> ts = getTransitions();
		 for (Transition tran: ts)
		 {
		 	result = result+"\t"+tran.toString()+"\n";
		 }
		 String entry = getEntryCode();
		 if (entry != null)
		 {
		 	result += "Entry: "+entry+"\n";
		 }
		 String exit = getExitCode();
		 if (exit != null)
		 {
		 	result += "Exit: "+exit+"\n";
		 }
		return result;
	}

	
}

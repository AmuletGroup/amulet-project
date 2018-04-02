package amulet.qm;

import javax.xml.xpath.XPathExpressionException;

import org.w3c.dom.Node;

import amulet.fsm.State;
import amulet.fsm.Transition;

public class QMTransition implements Transition {
	private Node m_node;
	private State m_source;
	
	public QMTransition(State src, Node xmlnode)
	{
		m_node = xmlnode;
		m_source = src;
	}

	@Override
	public String getTrigger() {
		return m_node.getAttributes().getNamedItem("trig").getNodeValue();
	}

	@Override
	public State getTarget() {
		String target_name = m_node.getAttributes().getNamedItem("target").getNodeValue();
		Node target_node;
		try {
			target_node = QMXPath.getXPathNode(convertTargetName(target_name), m_node);
		} catch (XPathExpressionException e) {
			e.printStackTrace();
			return null;
		}
		return QMState.getState(target_node);
	}

	private String convertTargetName(String target_name) {
		String[] tokens = target_name.split("/");
		String result = "";
		for (String t : tokens)
		{
			String newtok = t;
			try{
				int index = Integer.parseInt(t);
				newtok = "state["+index+"]";
			} catch (NumberFormatException e) {
				
			}
			if (!result.equals(""))
			{
				result += "/";
			}
			result += newtok;
		}
		return result;
	}

	@Override
	public State getSource() {
		return m_source;
	}
	
	public String toString()
	{
		return "QMTransition: "+getSource().getName()+" --["+getTrigger()+"]--> "+getTarget().getName();
	}

}

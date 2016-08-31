/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.qm;

import java.util.Vector;
import java.util.HashMap;

import javax.xml.xpath.XPathExpressionException;

import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import amulet.fsm.State;
import amulet.qm.QMState;

public class QMClass {
	static int NEXT_CLASS_ID = 100;        // starting app value.
	static int NEXT_CLASS_LOG_ID = 65;     // starting app log value. NOTE: 65 == 'A' which is an arbitrary string used to name log files. 
	
	private Node m_node; 
	private Node m_statechart;
	private Node m_opNode;
	private Node m_attribNode;
	private Vector<Node> m_action_nodes;
	private Vector<Node> m_guard_nodes;
	private Vector<State> m_states;
	private Vector<String> m_array_attribs;
	private HashMap<String, String[]> m_class_attribs_detail;
	public HashMap<Integer, String> qmStateId2qmStateNameMap; 
	private int m_id = -1;
	private int log_id = -1;
		
	public QMClass(Node n, Node opNode, Node attribNode) throws XPathExpressionException {
		/*
		 * Cache the Node objects extracted elsewhere, internally in this QMClass.
		 * m_node       : class node
		 * m_opNode     : operation node
		 * m_attribNode : attribute node
		 * 
		 * TODO: See QMFile and how it calls this -- I don't think it is correct but I need to investigate further....
		 */
		m_node = n;
		m_opNode = opNode;
		m_attribNode = attribNode;
		
		// Load the complete state chart. 
		m_statechart = QMXPath.getXPathNode("./statechart", m_node);

        /*
         * A mapping of QM "IDs" that map to specific QM States. 
         * 
		 *    NOTE: This is needed since QM, at compile/generation time, creates "action" nodes 
         *    that specify what the "target" state is that a particular "trigger" leads to; 
         *    The states, however, are not referred to by name, but rather an ID that corresponds 
         *    to the order in which the states are listed in the APPNAME.qm file. 
         *  
         *    Example: In the "bite_counter.qm" file, QM State "nodes" are listed in the following 
         *    order, and thus, are referred to by action nodes with the index values indicated in parenthesis:
         *       <state name="LookForLift">   (1)
         *       <state name="Wait2">         (2)
         *       <state name="Wait8">         (3)
         *       <state name="LookForDrop">   (4)
         *       <state name="Monitor">       (5)
         */
		qmStateId2qmStateNameMap = new HashMap<Integer, String>();

		m_states = new Vector<State>();
        NodeList states = QMXPath.getXPathList("./state", m_statechart);
        for (int i=0; i < states.getLength(); i++) {
        	// Create a QM State from an XML node and add to list of States. 
    		State newstate = QMState.getState(states.item(i));
    		m_states.add(newstate);
    		
    		// Update the qmStateId2qmStateNameMap with the new state. 
    		addQmSateId2NameMapEntry( i+1, newstate.getName() );
        }
        
        m_action_nodes = new Vector<Node>();
        NodeList actions = ((Element)m_statechart).getElementsByTagName("action");
        for (int i=0; i < actions.getLength(); i++) {
        	// Add newly encountered XML node to list of Nodes. 
        	m_action_nodes.add(actions.item(i));
        }        
        
        m_guard_nodes = new Vector<Node>();
        NodeList guards = ((Element)m_statechart).getElementsByTagName("guard");
        for (int i=0; i < guards.getLength(); i++) {
        	// Add newly encountered XML node to list of Nodes. 
        	m_guard_nodes.add(guards.item(i));
        }
        
		m_array_attribs = new Vector<String>();
		m_class_attribs_detail = new HashMap<String, String[]>();
        updateAttributes();
	}
	
	public NodeList getInitialTransition() {
		try {
			NodeList initNodeList = QMXPath.getXPathList("./initial", m_statechart);
			return initNodeList;
		} catch (XPathExpressionException xPathExp) {
			return null;
		}
	}

	public Vector<State> getStates() {
		return m_states;
	}

	public Node getAttribNode() {
		return m_attribNode;
	}

	public Node getOpNode() {
		return m_opNode;
	}

	public Vector<Node> getActionNodes() {
		return m_action_nodes;
	}

	public Vector<Node> getGuardNodes() {
		return m_guard_nodes;
	}
	
	public Vector<String> getAttributes() {
		Vector<String> result = new Vector<String>();
		
		try {
			NodeList attributes = QMXPath.getXPathList("./attribute", m_node);
			for (int i=0; i < attributes.getLength(); i++)
			{
				String attrib = attributes.item(i).getAttributes().getNamedItem("name").getNodeValue();
				if (attrib.indexOf("[") > -1 && attrib.indexOf("]") > -1) {
					attrib = attrib.substring(0, attrib.indexOf("["));
				}
				result.add(attrib);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

	public HashMap<String, String[]> getAttributesWithDetails() {
		return m_class_attribs_detail;
	}
	
	public void updateAttributes() {
		try {
			NodeList attributes = QMXPath.getXPathList("./attribute", m_node);
			for (int i=0; i < attributes.getLength(); i++) {
				NamedNodeMap attr = attributes.item(i).getAttributes();
				Node typeNode = attr.getNamedItem("type");
				Node nameNode = attr.getNamedItem("name");
				Node propNode = attr.getNamedItem("properties");
				
				String attrib = nameNode.getNodeValue();
				String[] ss = new String[2];
				ss[0] = typeNode.getNodeValue();
				if(ss[0].contains("_array")) {
					// Already exists as an array so put value in from properties of attribute
					ss[1] = propNode.getNodeValue();
					// Now clear the properties attribtue before it is parsed, THIS IS A HACK BECAUSE 
					// QM IS STUPID AND SO IS OUR METHOD!
					// We have no way to persist the array length from a global array, through to the 
					// Generaed QM file since we rewrite array definitions (how obscure is that)
					// One fix? Use attributes in temp file to set array size, then delete those attributes
					// That is still a hack though
					// OR add a arraySize node to the attribtue
					propNode.setNodeValue("0x00");
				} else {
					ss[1] = "1";
				}
				if (attrib.indexOf("[") > -1 && attrib.indexOf("]") > -1) {
					ss[1] = attrib.substring(attrib.indexOf("[")+1, attrib.indexOf("]"));
					propNode.setNodeValue(ss[1]);
					attrib = attrib.substring(0, attrib.indexOf("["));
					nameNode.setNodeValue(attrib);
					m_array_attribs.add(attrib);
					typeNode.setNodeValue("__"+typeNode.getNodeValue()+"_array");
				}

				m_class_attribs_detail.put(attrib, ss);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public Vector<String> getSignals() {
		Vector<String> result = new Vector<String>();
		
		try {
			NodeList signals = QMXPath.getXPathList("./state/tran", m_statechart);
			for (int i=0; i < signals.getLength(); i++)
			{
				String signal = signals.item(i).getAttributes().getNamedItem("trig").getNodeValue();
				result.add(signal);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

	public void updateSignals() {
		try {
			NodeList signals = QMXPath.getXPathList("./state/tran", m_statechart);
			for (int i=0; i < signals.getLength(); i++) {
				String signal = signals.item(i).getAttributes().getNamedItem("trig").getNodeValue();
				if (!signal.startsWith("AMULET")) {
					signals.item(i).getAttributes().getNamedItem("trig").setNodeValue(this.getName()+"_"+signal);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public Vector<String> getArrayAttributes() {
		return m_array_attribs;
	}
	
	public Vector<String> getOperations() {
		Vector<String> result = new Vector<String>();
		
		try {
			NodeList attributes = QMXPath.getXPathList("./operation", m_node);
			for (int i=0; i < attributes.getLength(); i++)
			{
				String attrib = attributes.item(i).getAttributes().getNamedItem("name").getNodeValue();
				result.add(attrib);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

	public Vector<Node> getOperationNodes() {
		Vector<Node> result = new Vector<Node>();
		
		try {
			NodeList attributes = QMXPath.getXPathList("./operation/code", m_node);
			for (int i=0; i < attributes.getLength(); i++)
			{
				Node attrib = attributes.item(i);
				result.add(attrib);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

	public Node getNode() {
		return m_node;
	}

	public String getName() {
		return m_node.getAttributes().getNamedItem("name").getNodeValue();
	}

	public int getID() {
		if (m_id > -1) return m_id;
		
		try {
			String id_str = m_node.getAttributes().getNamedItem("appid").getNodeValue();
			m_id = Integer.parseInt(id_str);
			return m_id;
		}  catch (Exception e) {
			m_id = NEXT_CLASS_ID++;
			return m_id;
		}
	}
	
	public int getLogID() {
		if (log_id > -1)
			return log_id;

		log_id = NEXT_CLASS_LOG_ID++;
		return log_id;
	}

	public void addQmSateId2NameMapEntry(Integer idx, String name) {
		qmStateId2qmStateNameMap.put(idx, name);
	}
	
	/**
	 * Get the name of the corresponding state when given a string representation 
	 * of the target state. 
	 * @param targetName A string representation of the QM State ID. 
	 * @return
	 */
	public String getQmSateId2NameMapEntry(String currentState, String targetNameById) {
		String targetStateName = "";
		if (targetNameById.equals("..")) {
			// Special Case: ".." implies a trigger leading back to the current state. 
			targetStateName = currentState;
		} else {
			// Extract QM State ID from input string.
			String id = targetNameById.substring(targetNameById.lastIndexOf("/")+1, targetNameById.length());
			
			// Convert to integer and look-up in hash-map.
			try {
				int intId = Integer.parseInt(id);
				targetStateName = qmStateId2qmStateNameMap.get( intId );
			} catch (NumberFormatException nfe) {
				targetStateName = "GET ERROR!";
			}
		}
		
		return targetStateName;
	}
	
	public HashMap<Integer, String> getQmStateId2NameMap() {
		return qmStateId2qmStateNameMap;
	}	
	
	public String toString() {
		String str = "QM Class: " + getName() + "\n";

		str += "\t*States*\n";
		for (State qmstate : getStates()) {
			str += "\t" + qmstate + "\n";
		}
		return str;
	}
	
	/*
	 * UTILITY FUNCTIONS.
	 */
	
	public static String getCurrentQMStateFromActionNode(Node action_node) {
		String currentState = null;
		Node parent1 = action_node.getParentNode().getParentNode();
		Node parent2 = parent1.getParentNode();
		Node parent3 = parent2.getParentNode();
		
		// DEBUG
		//System.out.println("getCurrentQMStateFromActionNode():: ACTION_NODE:"+action_node.getNodeName() + ", P1:"+parent1.getNodeName() + ", P2:"+parent2.getNodeName() + ", P3:"+parent3.getNodeName());
		
		/*
		 * Action nodes, for transitions, are typically found within the first 
		 * few layers down from the "initial" node or any given "state" node.
		 */
		if (parent1.getNodeName().equals("initial") || parent1.getNodeName().equals("statechart")) {
			// This is an initial transition and does not belong to a state. 
			currentState = "NONE";
		} else if (parent1.getNodeName().equals("state")) {
			// This is a transition from the current state to a new state.
			currentState = parent1.getAttributes().getNamedItem("name").getNodeValue();
		} else if (parent2.getNodeName().equals("state")) {
			// This is a transition from the current state to a new state.
			currentState = parent2.getAttributes().getNamedItem("name").getNodeValue();
		} else if (parent3.getNodeName().equals("state")) {
			// This is a transition from the current state to a new state.
			currentState = parent3.getAttributes().getNamedItem("name").getNodeValue();
		} else {
			currentState = "???";
		}
		
		return currentState;
	}
	
	/*
	 * MAIN/TEST DRIVER.
	 */
	
	public static void main(String[] args) {
		/*
		 * TEST: build QM State ID<->NAME Map. 
		 */
		
//		QMFile qmfile = new QMFile("../../applications/bite_counter/bite_counter.qm");
		QMFile qmfile = new QMFile("../../applications/fall_detection/fall_detection.qm");
				
		for (QMClass qmclass : qmfile.getClasses()) {
			System.out.println("num classes = " + qmfile.getClasses().size() + ", class name = " + qmclass.getName());
			
			int numStates = qmclass.getStates().size();
			for (int i = 0; i < numStates; i++) {
				QMState qmstate = (QMState) qmclass.getStates().get(i);
				qmclass.addQmSateId2NameMapEntry(i+1, qmstate.getName() );
			}
		}
		
		System.out.println( qmfile.getClasses().get(0).getQmStateId2NameMap());
	}
}

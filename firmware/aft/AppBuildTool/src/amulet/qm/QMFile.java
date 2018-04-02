package amulet.qm;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.StringWriter;
import java.util.Collection;
import java.util.Vector;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.xpath.XPathExpressionException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;

import amulet.fsm.State;

public class QMFile {
	private final String MAGIC_QUOTE_STRING = "___QQQQ__________12341241231_____";
	private Document m_xmlDocument;
	private Vector<QMClass> m_classes;
	private Vector<Node> m_instances;
	private String m_filename;
	 
	
	public QMFile(String filename)
	{
		m_filename = filename;
		loadQMFile(filename);
		m_instances = new Vector<Node>();
	}
	
	public boolean addFile(String name, String content)
	{
		try {
			//get the package node
			Node dirnode = getXPathList("/model/directory[@name=\"./\"]").item(0);
			Element newfile = m_xmlDocument.createElement("file");
			newfile.setAttribute("name", name);
			Element textel = m_xmlDocument.createElement("text");
			newfile.appendChild(textel);
			//newfile.setTextContent(StringEscapeUtils.escapeXml10(content));
			textel.setTextContent(content);
			
			
			dirnode.appendChild(newfile);
			return true;
		}
		catch (Exception e)
		{
			e.printStackTrace();
			return false;
		}
	}
	
	public boolean addClass(QMClass c)
	{
		try {
			//get the package node
			Node packagenode = getXPathList("/model/package[@name=\"AOs\"]").item(0);
			m_classes.add(c);
			Node imp = m_xmlDocument.importNode(c.getNode(), true);
			packagenode.appendChild(imp);
			return true;
		}
		catch (Exception e)
		{
			e.printStackTrace();
			return false;
		}
	}

	public boolean addClassAttribute(QMClass c)
	{
		try {
			//get the package node
			Node packagenode = getXPathList("/model/package[@name=\"AOs\"]").item(0);
			Node imp = m_xmlDocument.importNode(c.getAttribNode(), true);
			packagenode.appendChild(imp);
			return true;
		}
		catch (Exception e)
		{
			e.printStackTrace();
			return false;
		}
	}

	public boolean addClassOperation(QMClass c)
	{
		try {
			//get the package node
			Node packagenode = getXPathList("/model/package[@name=\"AOs\"]").item(0);
			Node imp = m_xmlDocument.importNode(c.getOpNode(), true);
			packagenode.appendChild(imp);
			return true;
		}
		catch (Exception e)
		{
			e.printStackTrace();
			return false;
		}
	}
	
	public void addClasses(Collection<QMClass> classlist)
	{
		for (QMClass c : classlist)
		{
			addClass(c);
		}
	}

	public void addClassAttributes(Collection<QMClass> classlist)
	{
		for (QMClass c : classlist)
		{
			addClassAttribute(c);
		}
	}

	public void addClassOperations(Collection<QMClass> classlist)
	{
		for (QMClass c : classlist)
		{
			addClassOperation(c);
		}
	}
	
	/**
	 * Load important parts of the QM XML file into QM POJOs created for easier manipulation.
	 * 
	 * @param filename The name of the QM file to parse.
	 * @return
	 */
	private boolean loadQMFile(String filename) {
		boolean loaded = false;
		
		try {
            FileInputStream file = new FileInputStream(new File(filename));
                 
            DocumentBuilderFactory builderFactory = DocumentBuilderFactory.newInstance();
            builderFactory.setNamespaceAware(true);
            DocumentBuilder builder =  builderFactory.newDocumentBuilder();
            m_xmlDocument = builder.parse(file);
            
            /* 
             * Extract "class" nodes in the QM XML file where the node's name is "class"; the correct 
             * class node is identified by having a "superclass" attribute that is defined as "qpc:QActive".
             * 
             * Ex.
             *    <class name="BiteCounter" superclass="qpc::QActive">
             */
            NodeList classnodes = getXPathList("/model/package/class[@superclass=\"qpc::QActive\"]");
            
            /*
             * Extract "operation" nodes. These are essentially stand-alone functions defined outside 
             * of the context of a particular state in the state machine.
             * 
             * Ex.
             *    <operation name="BiteCounter_ctor" type="void" visibility="0x00" properties="0x00">
             *        <code>
             *        BiteCounter *me = (BiteCounter *)AO_BiteCounter;
             *        QActive_ctor(&amp;me-&gt;super, (QStateHandler)&amp;BiteCounter_initial);
             *        </code>
             *    </operation>
             */
            NodeList classOpNodes = getXPathList("/model/package/operation");
            
            /*
             * Extract "attribute" nodes. These are essentially stand-alone variables (global variables?) 
             * defined outside of the context of a particular state in the state machine.
             * 
             * Ex.
             *    <attribute name="NSAMPLES" type="uint8_t" visibility="0x02" properties="0x00">
             */
            NodeList classAttribNodes = getXPathList("/model/package/attribute");

//        	System.out.println("classes=" + classnodes.getLength() + ", operations=" + classOpNodes.getLength() + ", attrs=" + classAttribNodes.getLength());

            // Now, construct a set of QMClass objects that are easier to work with in future operations.
            m_classes = new Vector<QMClass>();
            for (int i=0; i < classnodes.getLength(); i++) {
            	/* 
            	 * TODO: Is this correct? It seems like for each state, there could be multiple operation and attribute nodes. 
            	 * For some reason though, this code takes each class item (in most of our existing apps, there is only 
            	 * one class), and creates a QM class from the i^th item in each NodeList fetched above. 
            	 * 
            	 * In the BiteCounter app, when I check the XML file myself, there appears to be 1 class, 
            	 * 2 operation nodes and MANY attribute nodes...
            	 */
            	QMClass qc = new QMClass(classnodes.item(i), classOpNodes.item(i), classAttribNodes.item(i));
            	m_classes.add(qc);
            }
            
            // Only if we make it all the way through the parsing/construction of internal QM Objects should 
            // we indicate that the file was loaded successfully. 
            loaded = true;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (SAXException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        } catch (XPathExpressionException e) {
            e.printStackTrace();
        }
		
		return loaded;
    }
	
	public Vector<QMClass> getClasses()
	{
		return m_classes;
	}
	
	//for legacy reasons, I have left this in.
	public Vector<State> getStates() {
		QMClass firstClass = getClasses().get(0);
		if (firstClass == null) return null;
		return firstClass.getStates();
	}
		
	/* ****************************************************************** *
	 *                            Helper Methods                          *
	 * ****************************************************************** */

	private NodeList getXPathList(String query) throws XPathExpressionException
	{
		return QMXPath.getXPathList(query, m_xmlDocument);
	}
	
//	private Node getXPathNode(String query) throws XPathExpressionException
//	{
//		return QMXPath.getXPathNode(query, m_xmlDocument);
//	}
//	
//	private String getXPathStr(String query) throws XPathExpressionException
//	{
//		return QMXPath.getXPathStr(query, m_xmlDocument);
//	}

	private void hidequotes ()
	{
		try {
			NodeList nl = getXPathList("//text()");
			for (int i=0; i < nl.getLength(); i++)
			{
				Node n = nl.item(i);
				//System.out.println("item("+i+"): "+n.getNodeValue());
				n.setNodeValue(n.getNodeValue().replace("\"", MAGIC_QUOTE_STRING));
			}
		} catch (XPathExpressionException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public boolean saveAs(String filename) {
		try {
			Node packagenode = getXPathList("/model/package[@name=\"AOs\"]").item(0);
			for (Node inst : m_instances)
			{
				packagenode.appendChild(inst);
			}
			
			hidequotes(); //dirty hack to appease QM
			
			
			m_xmlDocument.setXmlStandalone(true);
			m_xmlDocument.setXmlVersion("1.0");
			TransformerFactory tFactory = TransformerFactory.newInstance(); 
			Transformer transformer = tFactory.newTransformer();
			transformer.setOutputProperty(OutputKeys.METHOD, "xml");
			Source source = new DOMSource(m_xmlDocument);
			//Result output = new StreamResult(new File(filename));
			StringWriter sw = new StringWriter();
			Result output = new StreamResult(sw);
			
			transformer.transform(source, output); 
			
			
			//dirty hack continued.
			FileWriter fw = new FileWriter(filename);
			fw.write(sw.toString().replace(MAGIC_QUOTE_STRING, "&quot;"));
			fw.close();
			
			return true;
		}
		catch (TransformerConfigurationException e)
		{
			System.out.println("Error saving XML. Transformer misconfigured.");
			e.printStackTrace();
			return false;
		}
		catch (Exception e)
		{
			System.out.println("Error saving XML.");
			e.printStackTrace();
			return false;
		}
	}
	
	public String toString() {
		String str = "QM File: " + m_filename + "\n";

		str += "*Classes*\n";
		for (QMClass qmclass : m_classes) {
			str += qmclass + "\n";
		}

		return str;
	}

}

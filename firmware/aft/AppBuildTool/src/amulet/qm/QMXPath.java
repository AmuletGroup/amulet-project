package amulet.qm;

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class QMXPath {
	private static XPath theXPath = XPathFactory.newInstance().newXPath();
	
	
	public static Node getXPathNode(String query, Object context) throws XPathExpressionException
	{
		Node node = (Node) theXPath.compile(query).evaluate(context, XPathConstants.NODE);
        return node;
	}
	public static NodeList getXPathList(String query, Object context) throws XPathExpressionException
	{
		return (NodeList) theXPath.compile(query).evaluate(context, XPathConstants.NODESET);
	}
	public static String getXPathStr(String query, Object context) throws XPathExpressionException
	{
		return theXPath.compile(query).evaluate(context);
	}
}

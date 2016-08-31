/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.manifestreader;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.*;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.xpath.XPathExpressionException;

import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import amulet.qm.QMXPath;

public class ManifestReader {
	
	// XML manifest file name.
	private String m_manifest_filename;
	
	// XML objects for holding application manifest information. 
	private Document m_xmlDocument;
	private Node m_application_node;
	private NodeList m_api_nodes;

	// Parsed information from the XML manifest file.
	private Vector<String> m_authorizedApiList;
	private HashMap< String, List<String> > m_firstArgumentMap;
	private String m_app_name;
	private String m_app_short_name;
	private String m_app_status_flags;
	private String m_app_description; 

	public ManifestReader(String qm_filename) {
		// Strip of ".qm" extension from the QM filename & replace with ".xml"
		m_manifest_filename = qm_filename.substring(0, qm_filename.length()-2)+"xml";
		
		// Create new lists for recoding (1) the permissible API usage by this application, 
		// and (2) specific constraints on arguments passed to permissible API calls. 
		m_authorizedApiList = new Vector<String>();
		m_firstArgumentMap = new HashMap<String, List<String> >();

		// Setup file stream with input file.
		FileInputStream file = null;
		try {
			file = new FileInputStream(new File(m_manifest_filename));
		} catch (FileNotFoundException e) {
			System.err.println("Manifest file '" + m_manifest_filename +"' not found!");
			e.printStackTrace();
			System.exit(-1);
		}
		
		// Setup XML parser object.
        DocumentBuilderFactory builderFactory = DocumentBuilderFactory.newInstance();
        builderFactory.setNamespaceAware(true);

        // Initialize XML document object.
        DocumentBuilder builder;
        try {
			builder = builderFactory.newDocumentBuilder();
			m_xmlDocument = builder.parse(file);
		} catch (ParserConfigurationException parseConfigException) {
			error("error with XML parser configuration!", parseConfigException);
		} catch (SAXException saxException) {
			error("error building XML Document object!", saxException);
		} catch (IOException ioException) {
			error("error parsing XML file!", ioException);
		}

        // Get the application's metadata from the application's XML file.
        try {
			m_application_node = getXPathNode("/manifest/application");
			setAppMetadata();
		} catch (XPathExpressionException e) {
			error("error parsing XML file application node!", e);
		}
        
        // Get a list of the application's API permissions from the application's XML file. 
        try {
			m_api_nodes = getXPathList("/manifest/permissions/api");
	        updateApiAuthList();
		} catch (XPathExpressionException e) {
			error("error parsing XML file API nodes!", e);
		}        
	}

	/**
	 * Parse important, high-level, application-specific meta-data.
	 * 
	 * Fields marked with (!) are required. 
	 */
	public void setAppMetadata() {
		// (!) Get the application's name.
		try {
			m_app_name = getAppMetaData("./name");
		} catch (XPathExpressionException e) {
			error("error parsing XML application 'name' metadata!", e);
		} catch (NullPointerException e) {
			error("application 'name' metadata not found!", e);
		}

		// (!) Get the application's short name.
		try {
			m_app_short_name = getAppMetaData("./short_name");
		} catch (XPathExpressionException e) {
			error("error parsing XML application 'short name' metadata!", e);
		} catch (NullPointerException e) {
			error("application 'short name' metadata not found!", e);
		}

		// (!) Get the application's status characters.
		try {
			m_app_status_flags = getAppMetaData("./app_status");
		} catch (XPathExpressionException e) {
			error("error parsing XML application 'status' metadata!", e);
		} catch (NullPointerException e) {
			error("application 'status' metadata not found!", e);
		}

		// Get the application's description.
		try {
			// Remove extraneous newlines & make a single paragraph description. 
			m_app_description = "";
			String[] appDescLines = getAppMetaData("./app_description").trim().split("\n");
			for (String line : appDescLines) {
				m_app_description += line.trim() + " ";
			}
		} catch (XPathExpressionException e) {
			warning("error parsing XML application 'description' metadata!", e);
		} catch (NullPointerException e) {
			warning("application 'description' metadata not found!", e);
		}
	}
	
	/**
	 * Update an internal list which keeps track of the Amulet API calls 
	 * that are permissible for this application to invoke. 
	 */
	public void updateApiAuthList() {
		// Iterate over all of the API nodes in the XML file. 
		for (int i = 0; i < m_api_nodes.getLength(); i++) {
			Node api_node = m_api_nodes.item(i);
			NamedNodeMap attr = api_node.getAttributes();

			// Record this Amulet API call as permissible for this application.
			Node nameNode = attr.getNamedItem("name");
			String api_name = nameNode.getNodeValue();
			m_authorizedApiList.add(api_name);

			/*
			 *  Build a list of acceptable values to pass as (first) arguments when calling the
			 *  Amulet API function.
			 *  
			 *  Ex.
			 *  	<api name="AmuletSubscribeInternalSensor" first_argument="ACCELEROMETER;HEARTRATE" />
			 */
			Node arguments_node = attr.getNamedItem("first_argument");
			if (arguments_node != null) {
				String first_arguments = arguments_node.getNodeValue();
				String[] first_arguments_arr = first_arguments.split(";");
				List<String> first_arguments_lst = Arrays.asList(first_arguments_arr);
				m_firstArgumentMap.put(api_name, first_arguments_lst);
			}
		}
	}
	
	/*
	 * Accessor Methods.
	 */
	
	public String getAppName() {
		return m_app_name;
	}

	public String getAppShortName() {
		return m_app_short_name;
	}

	public String getAppStatusFlags() {
		return m_app_status_flags;
	}

	public String getAppDescription() {
		return m_app_description;
	}

	public Vector<String> getAuthorizedApiList() {
		return m_authorizedApiList;
	}

	public HashMap< String, List<String> > getAuthorizedApiArgumentMap() {
		return m_firstArgumentMap;
	}
	
	/*
	 * Helper methods.	
	 */

	private String getAppMetaData(String query) throws XPathExpressionException {
		return QMXPath.getXPathNode(query, m_application_node).getTextContent(); //getNodeValue();
	}

	private NodeList getXPathList(String query) throws XPathExpressionException {
		return QMXPath.getXPathList(query, m_xmlDocument);
	}

	private Node getXPathNode(String query) throws XPathExpressionException {
		return QMXPath.getXPathNode(query, m_xmlDocument);
	}
	
	private void error(String errorMsg, Exception e) {
		System.err.println(" + " + getClass().getSimpleName() + " ERROR:: XML manifest file '" + m_manifest_filename +"':: " + errorMsg);
		e.printStackTrace();
		System.exit(-1);
	}

	private void warning(String warningMsg, Exception e) {
		System.err.println(" + " + getClass().getSimpleName() + " WARNING:: XML manifest file '" + m_manifest_filename +"':: " + warningMsg);
	}

	/*
	 * Main (stand-alone test)
	 */

	public static void main(String[] args) {
		ManifestReader reader = new ManifestReader("/Users/traviswp/projects/project-amulet/src/applications/fall_detection/fall_detection.qm");
		System.out.println("App Name       : " + reader.getAppName());
		System.out.println("App Short Name : " + reader.getAppShortName());
		System.out.println("App Status     : " + reader.getAppStatusFlags());
		System.out.println("App Description: " + reader.getAppDescription());
	}
	
}

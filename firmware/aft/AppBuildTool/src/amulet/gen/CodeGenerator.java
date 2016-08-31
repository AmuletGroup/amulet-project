/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.gen;

import java.util.Collection;
import java.util.Vector;
import java.util.HashMap;
import java.util.Map;

import amulet.qm.QMClass;

public class CodeGenerator {
	private final static int APP_STACK_SIZE = 1024;
	private final static int SERVICE_STACK_SIZE = 0;
	
	/*
	 * See: amulet.h in the Amulet OS. 
	 */
	private String lastSig = "AMULET_APPLICATIONS_Q_SIG_START_SIG";
	
	Vector<String> m_services;
	Vector<QMClass> m_applications;
	Vector<String> m_application_names;
	Vector<String> m_application_short_names;
	Vector<String> m_application_types;

	
	public CodeGenerator()
	{
		this(null);
	}
	
	public CodeGenerator(Vector<String> services)
	{
		m_applications = new Vector<QMClass>();
		m_services = services;
		m_application_names = new Vector<String>();
		m_application_short_names = new Vector<String>();
		m_application_types = new Vector<String>();
	}

	public Vector<QMClass> getApplications() {
		return m_applications;
	}
	
	public boolean AddApplication(QMClass qmc, String appName, String appShortName, String appType)
	{
		if (m_applications.contains(qmc)) return false;
		m_application_names.add(appName);
		m_application_short_names.add(appShortName);
		m_application_types.add(appType);
		return m_applications.add(qmc);
	}
	
	/* ********************************************************************** *
	 *                      Generation Code for apps.c
	 * ********************************************************************** */

	/**
	 * This function effectively generates all of the file content for apps.c.
	 */
	public String getAppsC()
	{
		String code = "\n#include \"apps.h\"\n";
		
		for (QMClass qmc : m_applications)
		{
			code = code + generateAppCFileBlock(qmc)+"\n\n";
		}
		
		return code;
	}
	
	public String generateAppCFileBlock(QMClass qmc)
	{
		String global_array_init = "";
		String table_init = "static "+qmc.getName()+" l_"+qmc.getName()+" = {\n";
		HashMap<String,String[]> attribute_details = qmc.getAttributesWithDetails();
		int global_arrays_cnt = 0;
		for (Map.Entry<String, String[]> entry : attribute_details.entrySet()) {
			String name = entry.getKey();
			String[] array_atts = entry.getValue();
			int size = Integer.parseInt(array_atts[1]);
			if(size > 1) {
				global_arrays_cnt++;
				String type = array_atts[0].substring(2, array_atts[0].indexOf("_array"));
				String internal_array_name = "__l_"+qmc.getName()+"_internal_array_"+name;
				global_array_init += " "+type+" "+internal_array_name+"["+size+"]; __attribute__ ((section(\".upper.rodata\")))\n";
				table_init += "		."+name+" = {.values = &"+internal_array_name+"[0], .__arr_len = "+size+"},\n";
			}
		}
		table_init+="};\n\n";

		if(global_arrays_cnt == 0) {
			table_init = "static "+qmc.getName()+" l_"+qmc.getName()+";\n\n";
		}
		global_array_init +="\n";

		String code = "\n"+

				"/* Local objects -----------------------------------------------------------*/\n"+
				global_array_init +
				table_init + 
				//"static "+qmc.getName()+" l_"+qmc.getName()+"; /* the single instance of the Table active object */\n\n"+
				
				//"static DataLogger l_DataLogger = {.hr = {.values = __l_DataLogger_internal_array_hr, .__arr_len = 32}};\n"+
				
				
				"/* Global-scope objects ----------------------------------------------------*/\n"+
				"QActive * const AO_"+qmc.getName()+" = &l_"+qmc.getName()+".super; /* \"opaque\" AO pointer */\n\n"+
				"$define(AOs::"+qmc.getName()+"_ctor)\n"+
				"$define(AOs::"+qmc.getName()+")\n";
		
		/*for (String attrib: qmc.getAttributes())
		{
			code = code + "$define (AOs:"+qmc.getName()+"::"+attrib+")\n";
		}
		
		for (String op: qmc.getOperations())
		{
			code = code + "$define (AOs:"+qmc.getName()+"::"+op+")\n";
		}*/
		
		return code;
	}
	
	/* ********************************************************************** *
	 *                      Generation Code for apps.h
	 * ********************************************************************** */

	/**
	 * This function effectively generates all of the file content for apps.h.
	 */
	public String getAppsH()
	{
		String code =   
			"#ifndef APPS_H_\n"+
		 	"#define APPS_H_\n"+
			"\n"+
			"#ifdef	__cplusplus\n"+
			"extern \"C\" {\n"+
			"#endif\n\n"+
			"#include \"amulet.h\"\n"+
			"\n"+
			"//\n"+
			"//Globals\n"+
			"//\n"+
			"\n"+
			generateAppIDDefines()+"\n"+
			generateAppLogIDDefines()+"\n"+
			generateTypeDefMacro()+"\n";
		
		for (QMClass qmc : m_applications)
		{
			code += 
				"//\n"+
				"//"+qmc.getName()+" defines\n"+
				"//\n"+
				"\n"+
				"$declare(AOs::"+qmc.getName()+")	\n"+
				"$declare(AOs::AO_"+qmc.getName()+")	\n"+
				"$declare(AOs::"+qmc.getName()+"_ctor)	\n"+
				generateAppSignals(qmc);
			
		}
		code += "#ifdef	__cplusplus\n"+
				"}\n"+
				"#endif\n\n"+
				"#endif //APPS_H_";
		
		return code;
	}
	
	// Generate #defines for App IDs
	private String generateAppIDDefines() {
		String code = "//Included Apps\n";
		
		for (QMClass qmc: m_applications)
		{
			code = code+
				"#define "+this.getComponentID(qmc.getName())+" "+qmc.getID()+"\n";
		}

		return code;
	}

	// Generate #defines for each App's Log ID
	private String generateAppLogIDDefines() 
	{
		String code = "//Included App Log IDs\n";
		
		for (QMClass qmc: m_applications)
		{
			code = code+
				"#define "+this.getComponentLogID(qmc.getName())+" "+qmc.getLogID()+"\n";
		}

		return code;
	}

	private String generateTypeDefMacro() 
	{ 
		String comment = "//Amulet array helper declarations\n";
		String arrayLengthMacro = "#define getArrayLength(a) a.__arr_len\n";
		String arrayReadMacro = "#define __read_array(a,index) (index >= 0 && index < a.__arr_len)?a.values[index]:-111111111\n";
		String initialization = comment+arrayLengthMacro+arrayReadMacro;
		return initialization;
	}

	private String generateAppSignals(QMClass q)
	{
		Vector<String> sigs = q.getSignals();
		Vector<String> sofar = new Vector<String>();
		
		String code = "enum "+ q.getName()+" {		\n";
		boolean first = true;
		
		//add a dummy sig at the end, to use as a starting point for others
		sigs.add("___DUMMY___"+q.getID()+"___");
		for (String sig : sigs)
		{
			if (!sofar.contains(sig) && !sig.startsWith("AMULET"))
			{
				sofar.add(sig);
				code = code + sig+"_SIG";
				if (first)
				{
					code = code + " = "+lastSig+", \n";
					first= false;
				} else {
					code = code + ",\n";
				}
				lastSig = sig+"_SIG";
			} else {
				//duplicate. don't list;
			}
		}
		code =code + "};\n\n";
		
		return code;
	}

	/* ********************************************************************** *
	 *                      Generation Code for main.c
	 * ********************************************************************** */

	/**
	 * This function effectively generates all of the file content for main.c.
	 */
	public String getMainC()
	{
		String code = 
				"\n//Hardware Dependent Includes\n"+
				"#include \"qp_port.h\"\n"+
				"#include \"bsp_setup.h\"\n"+
				"\n"+
				"//Hardware Agnostic Includes\n"+
				"#include \"amulet.h\"\n"+
				"\n"+
				"//Amulet Application Includes\n"+
				"#include \"apps.h\"\n"+
				"\n"+
				"//Amulet Core Global Declarations\n"+
				"static QSubscrList l_subscrSto[5];\n"+
				"static QF_MPOOL_EL(AmuletSensorsEvt) l_smlPoolStoCoreSensors[20]; /* small pool */\n"+
				"\n"+
				"#define SERVICE_EVT_QUEUE_LENGTH  10\n"+
				"#define APP_EVT_QUEUE_LENGTH      10\n"+
				"\n"+
				getEventQueueDecls()+
				"\n"+
				"App apps["+m_applications.size()+"]; __attribute__ ((section(\".upper.rodata\")))\n"+
				"\n"+
				getMainMethod();
		
		return code;
					
	}

	private String getEventQueueDecls()
	{
		String code = "//Amulet Core Services Declarations\n";
		
		for (String s : m_services)
		{
			code = code + "static QEvt const *"+this.getEvtQueueName(s)+"[SERVICE_EVT_QUEUE_LENGTH]; \n";
		}
		
		code = code + "\n"+
						"//Amulet Top-level Application Declarations\n"+
						//"Subscription * GlobalSubscriptionQueueHead;	\n"+
						//"Subscription sensorSubscriptions[MAX_SENSOR_SUBSCRIPTIONS];\n" +
						//"uint8_t currentNumSubscriptions;\n" +
						"App * GlobalAppQueueHead;\n\n";
		
		for (QMClass qmc : m_applications)
		{
			code = code + "static QEvt const *"+this.getEvtQueueName(qmc.getName())+"[APP_EVT_QUEUE_LENGTH];\n";
		}
		
		return code;
	}

	/* *****************************************************************  */

	/**
	 * This function effectively generates all of the function content for 
	 * the main method in main.c.
	 */	
	public String getMainMethod()
	{
		
		String code = 
			"//Main function for Amulet startup\n"+
			"int main() {\n"+
			"	currentNumSubscriptions = 0;\n"+
			"\n"+

			"\t//Hardware initializations\n"+
			"\tQF_init();\n" +
			"\tBSP_init();\n" +
			"\n"+

			"\t//\n"+
			"\t//Create Objects\n"+
			"\t//\n"+
			"\n"+

			//createServiceObjects()+
			callServiceCtors()+
			callAppCtors()+
			setupDictionaries()+
			setupEventPools() + 

			"\t{\n"+
			generateQueueUpApps()+
			"\t\t//Amulet UI startup initializations\n" +
			"\t\tCoreUIInit();\n" +
			"\t}\n\n" +

			generateAllAppsStart()+
			// "{\n"+
			// generateQueueUpApps()+
			// "// The UI has various initializations that should take place at startup.\n" +
			// "CoreUIInit();\n\n" +
			// "//printQueueApp(GlobalAppQueueHead);	\n\n"+
			//"\tbsp_printf(\"Amulet Core Started!\\n\\r\");\n\n"+
			"\t//Run the QF application\n"+
			"\treturn QF_run();\n"+
			"}\n";
		
		return code;
	}
	
	private String callServiceCtors()
	{
		String code = "\t//Amulet Services (initializations)\n";
		for (String s : m_services)
		{
			code += "\t"+callCtor(s)+"\n";
		}
		return code+"\n";
	}
	
	private String callAppCtors()
	{
		return callCtors("Amulet Applications (initializations)", m_applications);
	}
	
	private String callCtors(String heading, Collection<QMClass> ctors)
	{
		String code = "\t//"+heading+"\n";
		for (QMClass qmc : ctors)
		{
			code += "\t"+callCtor(qmc.getName())+"\n";
		}
		return code+"\n";
	}
	
	private String callCtor(String className)
	{
		return className+"_ctor();";
	}
	
	private String setupDictionaries()
	{
		String code = 
			"\t//Setup Dictionaries\n"+
			"\tQS_OBJ_DICTIONARY(l_smlPoolStoCoreSensors);\n";

		for (String s : m_services)
		{
			code += "\tQS_OBJ_DICTIONARY("+this.getEvtQueueName(s)+");\n";
		}
		return code+"\n";
	}
	
	private String setupEventPools()
	{
		String code = 
			"\t//Initialize Publish-Subscribe Event Pools \n"+
			"\tQF_psInit(l_subscrSto, Q_DIM(l_subscrSto));\n";
		code += "\tQF_poolInit(l_smlPoolStoCoreSensors, sizeof (l_smlPoolStoCoreSensors), sizeof (l_smlPoolStoCoreSensors[0]));\n\n";
		return code;
	}
		
	private String generateQueueUpApps()
	{
		if (m_applications.size() == 0) return "";
		
		String code = 
			"\t\t//Queue-up Apps\n"+
			"\n"+
			"\t\tGlobalAppQueueHead = &apps[0];\n"+
			"\n";

		int index = 0;
		for (QMClass qmc: m_applications)
		{
			code = code + generateQueueUpApp(qmc.getName(), index++);
		}
		return code;
	}
	
	private String generateQueueUpApp(String app, int index)
	{
		String appobj = "apps["+index+"]";
		String code = 
			"\t\t"+appobj+".appID = "+getComponentID(app)+";\n"+
			"\t\t"+appobj+".appLogID = "+getComponentLogID(app)+";\n"+
			"\t\t"+appobj+".activeObject = AO_"+app+";\n" + 
			"\t\t"+appobj+".next = NULL;\n" +
			"\t\t"+"strncpy("+appobj+".appName, \""+m_application_names.get(index)+"\", MAX_DISPLAY_LEN);\n"+
			"\t\t"+"strncpy("+appobj+".appShortName , \""+m_application_short_names.get(index)+"\", MAX_STATUS_LEN);\n"+
			"\t\t"+appobj+".appType = "+m_application_types.get(index)+";\n";
		
		// Don't enqueue the first app (causes infinite recursion when enqueuing).  
		if(index > 0)
			code += "\t\tenqueueApp(GlobalAppQueueHead, &"+appobj+");\n";
		code += "\n";
		return code;
	}
	
	private String generateAppStart(String app, int priority, int stacksize)
	{
		return "\tQActive_start(AO_"+app+", (uint8_t)"+priority+", "+getEvtQueueName(app)+","+" Q_DIM("+getEvtQueueName(app)+"), (void *) 0, "+stacksize+"U, (QEvt *) 0);\n";
	}

	private String generateAllAppsStart()
	{
		String code = "\t//Start amulet services & apps (NOTE: make sure to set priorities!)\n";
		int count = 1;
		
		for (String s: m_services)
		{
			code = code + generateAppStart(s, count++, SERVICE_STACK_SIZE);
		}
		for (QMClass qmc: m_applications)
		{
			code = code + generateAppStart(qmc.getName(), count++, APP_STACK_SIZE);
		}
		return code + "\n";
	}
	
	/* ********************************************************************** *
	 *                     Code Generation Helper Methods
	 * ********************************************************************** */

	private String getEvtQueueName(String component)
	{
		return component+"_queueSto";
	}
	
	private String getComponentID(String component)
	{
		return component.toUpperCase()+"_APP";
	}

	private String getComponentLogID(String component)
	{
		return component.toUpperCase()+"_APP_LOG";
	}

}

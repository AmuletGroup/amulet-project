package amulet.appbuilder;

import java.util.Vector;
import java.util.HashMap;
import java.nio.file.Path;
import java.nio.file.Files;
import java.nio.file.FileSystem;
import java.nio.file.FileSystems;

import amulet.fsm.State;
import amulet.manifestreader.ManifestReader;
import amulet.qm.QMClass;
import amulet.qm.QMReader;
import amulet.resourceprofiler.Resource;
import amulet.resourceprofiler.ResourceProfiler;
import amulet.resourceprofiler.Resource.ComputationType;
import amulet.resourceprofiler.Resource.ResourceType;
import amulet.translator.attributemapper.AttributeMapper;
import amulet.translator.authorizationmodule.AuthorizationModule;
import amulet.translator.compatibilitychecker.CompatChecker;
import amulet.translator.functionwhitelist.FunctionWhitelist;
import amulet.translator.runtimecheck.RuntimeCheck;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.JSONValue;
import org.w3c.dom.Node;

public class AppBuilder{
	CompatChecker compatchecker = null;
	AuthorizationModule authModule = null;
	RuntimeCheck runtimecheck = null;
    FunctionWhitelist funcWhiteList = null;
    AttributeMapper attributeMapper = null;
    ManifestReader manifestReader = null;
    
    private boolean togglePins = false;              // Default: we do *not* insert code for pin toggling.
	private boolean passToolchain = false;           // Default: we do *not* pass the tool-chain.
	private boolean pipeLineReturnStatusSuccess = false;    // Default: we indicate that the pipe line failed.

	ResourceProfiler resourceProfiler = null;

	private JSONObject finalJSONGraphStructure = null;

	public AppBuilder(String filename, boolean toggle, boolean toolChain, ResourceProfiler resourceProfiler)
	{
		togglePins = toggle;
		passToolchain = toolChain;
		this.resourceProfiler = resourceProfiler;
		this.resourceProfiler.add(filename);
		
		pipeLineReturnStatusSuccess = callPipeLine(filename);

		if (!pipeLineReturnStatusSuccess) {
			System.out.println("ERROR:: App build failed.");
		} else {
			System.out.println("SUCCESS:: App built successfully.");
		}

		cleanUp(filename);
	}
	
	public boolean getAppBuilderReturnStatus() {
		return pipeLineReturnStatusSuccess;
	}

	/**
	 * Get the graphical structure of the current FSM. The structure is denoted as:
	 * 
	 * {
	 *    "applications": [{
	 *       "appname": "APP_NAME",
     *       "states": [
 	 *          {"source": "SRC_STATE", "trigger": "TRIGGER_NAME", "target": "DEST_STATE"},
     *          ...
     *        ]},
     *       ...
     *    ]
     * }
	 */
	public String getFinalJSONGraphStructure() {
			return JSONValue.toJSONString(finalJSONGraphStructure);
	}

	@SuppressWarnings("unchecked")
	private boolean callPipeLine(String filename) {
		QMReader qm = new QMReader(filename);
		try {
			manifestReader = new ManifestReader(filename);
		} catch (Exception exp) {
			System.out.println("Exception when creating manifest reader for "+filename);
			exp.printStackTrace();
			return false;
		}

		// This will be written to a file for parsing by the Resource Profile UI
		finalJSONGraphStructure = new JSONObject();

		if (passToolchain) {
		
			Vector<QMClass> classes = qm.getClasses();
			authModule = new AuthorizationModule(manifestReader);
			
			for (int j=0; j < classes.size(); j++) {
				QMClass qmclass = classes.get(j);
				Vector<State> states = qmclass.getStates();
				Vector<Node> action_nodes = qmclass.getActionNodes();
				Vector<Node> guard_nodes = qmclass.getGuardNodes();
				Vector<Node> operation_nodes = qmclass.getOperationNodes();
				
				// Get all the attributes (global variables) that are put in FRAM
				HashMap<String, String[]> attdetails = qmclass.getAttributesWithDetails();
				resourceProfiler.setGlobalContext(qmclass.getName());
				for (String key : attdetails.keySet()) {
					String[] attinfo = attdetails.get(key);
					resourceProfiler.addMemoryResource(qmclass.getName(), attinfo[0], key, attinfo[1], ResourceType.GLOBAL_MEMORY);
				}
				resourceProfiler.clearProfilerContext();

				// This outputs a JSON format of the QMClass
				finalJSONGraphStructure.put("appname", qmclass.getName());
				finalJSONGraphStructure.put("app_human_readable_name", manifestReader.getAppName());
				finalJSONGraphStructure.put("app_human_readable_description", manifestReader.getAppDescription());
				
				/*
				 * States.
				 */
				// DEBUG::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
				//NodeList initNodes = qmclass.getInitialTransition();
				//for(int n = 0; n < initNodes.getLength(); n++) {
				//	String target = initNodes.item(n).getAttributes().getNamedItem("target").getNodeValue();
				//	System.out.println("##DEBUG## init->" + qmclass.getQmSateId2NameMapEntry("NONE", target));
				//}
				// DEBUG::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
				
				for (int i=0; i < states.size(); i++) {
					State state = states.get(i);
					if (state != null) {
						// Set state attributes.
						String stateName = state.getName();
						
						// DEBUG::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
						//for(Transition t : state.getTransitions()) {
						//	QMTransition qmt = (QMTransition) t;
						//	System.out.println("##DEBUG## " + qmt.getSource().getName() + "->" + qmt.getTrigger() + "->" + qmt.getTarget().getName());
						//}
						// DEBUG::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

						// [ENTER] State i. 
						resourceProfiler.setStateContext(stateName);
												
			    		// AFT analyzer/translator/profiler (entry)
						if(state.getEntryCode() != null && state.getEntryCode().length() > 0) {
							compatchecker = new CompatChecker(state.getEntryCode(), qmclass.getName());
							if (!compatchecker.getSuccess()) return false;
							if (!authModule.checkApiAuthorization(state.getEntryCode())) return false;
							runtimecheck = new RuntimeCheck(qmclass, state.getEntryCode(), togglePins, qmclass.getArrayAttributes(), resourceProfiler);
							state.setEntryCode(runtimecheck.getCheckedCode());
							attributeMapper = new AttributeMapper(state.getEntryCode(),qmclass.getAttributes());
							state.setEntryCode(attributeMapper.getMappedCode());
						}
						
			    		// AFT analyzer/translator/profiler (exit)
						if(state.getExitCode() != null && state.getExitCode().length() > 0) {
							compatchecker = new CompatChecker(state.getExitCode(), qmclass.getName());
							if (!compatchecker.getSuccess()) return false;
							if (!authModule.checkApiAuthorization(state.getExitCode())) return false;
							runtimecheck = new RuntimeCheck(qmclass, state.getExitCode(), togglePins, qmclass.getArrayAttributes(), resourceProfiler);
							state.setExitCode(runtimecheck.getCheckedCode());
							attributeMapper = new AttributeMapper(state.getExitCode(), qmclass.getAttributes());
							state.setExitCode(attributeMapper.getMappedCode());
						}
						
						// Add "basic blocks" record to state.
		    			Resource record = new Resource(ComputationType.BASIC_BLOCKS.text(), qmclass.getName(), ResourceType.COMPUTATION, 0.0);
		    			resourceProfiler.add( record );
		    			
						// [EXIT] State i. 
			         	resourceProfiler.clearProfilerContext();
					}
	         	}
				
				/*
				 * Action Nodes (i.e., Transitions).
				 */
	         	JSONArray statesJsonArray = new JSONArray();
	         	String prevState = null, prevTrigger = null, prevTarget = null;
	         	for (int i=0; i < action_nodes.size(); i++) {
					if (action_nodes.get(i) != null && action_nodes.get(i).getTextContent() != null) {
						/*
						 * Set transition attributes.
						 */
						String currentState = "", triggerName = "", targetNameID = "";
						
						Node action_node = action_nodes.get(i);
						Node parent1 = action_node.getParentNode();
						Node parent2 = parent1.getParentNode();

			    		// Set the "source" (i.e., the state which this action node is contained within---the "parent state").
			    		currentState = QMClass.getCurrentQMStateFromActionNode(action_node);

						// Set the "trigger".
						if (parent1.getAttributes().getNamedItem("trig") != null) {
							triggerName = parent1.getAttributes().getNamedItem("trig").getNodeValue();
						} else if (parent2.getAttributes().getNamedItem("trig") != null) {
							triggerName = parent2.getAttributes().getNamedItem("trig").getNodeValue();
						} else {
							triggerName = "init";
						}

						// Set the "target".
						if (parent1.getAttributes().getNamedItem("target") != null) {
							targetNameID = parent1.getAttributes().getNamedItem("target").getNodeValue();
						} else if (parent2.getAttributes().getNamedItem("target") != null) {
							targetNameID = parent2.getAttributes().getNamedItem("target").getNodeValue();
						} else {
							targetNameID = "init-target-unset?";
						}

			    		// Fetch the (human-readable) name of the state referenced by the ID specified in the action node.
			    		String targetName = qmclass.getQmSateId2NameMapEntry(currentState, targetNameID);
			    		
			    		/*
			    		 * Guard against recording duplicate records (it can happen!).
			    		 * 
			    		 * NOTE: This is a little sloppy, since it assumes that the duplicate records happen in 
			    		 * sequence; this seems to always be true with the QM files we've worked with, but this 
			    		 * issue may need to be re-addressed later...
			    		 */
			    		if (!currentState.equals(prevState) || !triggerName.equals(prevTrigger) || !targetName.equals(prevTarget)) {
				    		//System.out.println(" + TRIGGER:: currentState=" + currentState + "->triggerName="+triggerName + "->targetName="+targetName + " (targetNameID="+targetNameID+")");

				    		// Maintain a JSON representation of the FSM (i.e., states and transitions).
				    		JSONObject actionJsonObj = new JSONObject();
				    		actionJsonObj.put("source", currentState);
				    		actionJsonObj.put("trigger", triggerName);
				    		actionJsonObj.put("target", targetName);
				    		statesJsonArray.add(actionJsonObj);
				    		
							// [ENTER] Action Node i. 
				    		resourceProfiler.setActionContext(currentState, triggerName, targetName);
							
				    		// AFT analyzer/translator/profiler
							compatchecker = new CompatChecker(action_nodes.get(i).getTextContent(), qmclass.getName());
							if (!compatchecker.getSuccess()) return false;
							if (!authModule.checkApiAuthorization(action_nodes.get(i).getTextContent())) return false;
							runtimecheck = new RuntimeCheck(qmclass, action_nodes.get(i).getTextContent(), togglePins, qmclass.getArrayAttributes(), resourceProfiler);
							action_nodes.get(i).setTextContent(runtimecheck.getCheckedCode());
							attributeMapper = new AttributeMapper(action_nodes.get(i).getTextContent(),qmclass.getAttributes());
							action_nodes.get(i).setTextContent(attributeMapper.getMappedCode());
							
							// Add "basic blocks" record to action.
			    			Resource record = new Resource(ComputationType.BASIC_BLOCKS.text(), qmclass.getName(), ResourceType.COMPUTATION, 0.0);
			    			resourceProfiler.add( record );
			    			
							// [EXIT] Action Node i. 
							resourceProfiler.clearProfilerContext();
			    		}
			    		
			    		// Keep track of the previous trigger information...
			    		prevState = currentState;
			    		prevTrigger = triggerName;
			    		prevTarget = targetName;
					}
	         	}
	         	finalJSONGraphStructure.put("states", statesJsonArray);
	         	
				/*
				 * Guard Nodes.
				 */
	         	for (int i=0; i < guard_nodes.size(); i++) {
					if (guard_nodes.get(i) != null && guard_nodes.get(i).getTextContent() != null && guard_nodes.get(i).getTextContent().length() > 0) {
						// [ENTER] Guard Node i. 
						resourceProfiler.setGuardContext();
						
			    		// AFT analyzer/translator/profiler
						compatchecker = new CompatChecker(guard_nodes.get(i).getTextContent(), qmclass.getName());
						if (!compatchecker.getSuccess()) return false;
						if (!authModule.checkApiAuthorization(guard_nodes.get(i).getTextContent())) return false;
						runtimecheck = new RuntimeCheck(qmclass, guard_nodes.get(i).getTextContent(), togglePins, qmclass.getArrayAttributes(), resourceProfiler);
						guard_nodes.get(i).setTextContent(runtimecheck.getCheckedCode());
						attributeMapper = new AttributeMapper(guard_nodes.get(i).getTextContent(),qmclass.getAttributes());
						guard_nodes.get(i).setTextContent(attributeMapper.getMappedCode());
						
						// Add "basic blocks" record to guard.
		    			Resource record = new Resource(ComputationType.BASIC_BLOCKS.text(), qmclass.getName(), ResourceType.COMPUTATION, 0.0);
		    			resourceProfiler.add( record );
						
						// [EXIT] Guard Node i. 
						resourceProfiler.clearProfilerContext();
					}
	         	}
					         	
				/*
				 * Operation Nodes.
				 */
	         	for (int i=0; i < operation_nodes.size(); i++) {
					if (operation_nodes.get(i) != null && operation_nodes.get(i).getTextContent() != null && operation_nodes.get(i).getTextContent().length() > 0) {
						// Set operations attributes.
						String name = operation_nodes.get(i).getParentNode().getAttributes().getNamedItem("name").getNodeValue();
						String type = operation_nodes.get(i).getParentNode().getAttributes().getNamedItem("type").getNodeValue();
						String visibility = operation_nodes.get(i).getParentNode().getAttributes().getNamedItem("visibility").getNodeValue();
						String properties = operation_nodes.get(i).getParentNode().getAttributes().getNamedItem("properties").getNodeValue();
						
						// [ENTER] Operation Node i. 
						resourceProfiler.setOperationContext(name, type, visibility, properties);

			    		// AFT analyzer/translator/profiler
						compatchecker = new CompatChecker(operation_nodes.get(i).getTextContent(), qmclass.getName());
						if (!compatchecker.getSuccess()) return false;
						if (!authModule.checkApiAuthorization(operation_nodes.get(i).getTextContent())) return false;
						runtimecheck = new RuntimeCheck(qmclass, operation_nodes.get(i).getTextContent(), togglePins, qmclass.getArrayAttributes(), resourceProfiler);
						operation_nodes.get(i).setTextContent(runtimecheck.getCheckedCode());
						attributeMapper = new AttributeMapper(operation_nodes.get(i).getTextContent(),qmclass.getAttributes());
						operation_nodes.get(i).setTextContent(attributeMapper.getMappedCode());

						// Add "basic blocks" record to operation.
		    			Resource record = new Resource(ComputationType.BASIC_BLOCKS.text(), qmclass.getName(), ResourceType.COMPUTATION, 0.0);
		    			resourceProfiler.add( record );

						// [EXIT] Operation Node i. 
						resourceProfiler.clearProfilerContext();
						
						// For now, add a place-holder entry to the operation map -- it will be updated later
						// when a request is made to the Resource Profiler to calculate the cost of some application.
						resourceProfiler.getCurrentQMAppObj().addOperationRecord(name, 0.0, 0.0);
					}
	         	}
	         	
	         	// Verify checks against function white-list pass...
	            funcWhiteList = new FunctionWhitelist(qmclass.getName(), states, action_nodes, guard_nodes, qmclass.getOperations());
	            if (!funcWhiteList.getSuccess())
	            	return false;
	        }
			
	    } else {
	    	System.out.println("AFT WARNING: skipping AFT processing -- code has not been properly validated!");
	    }
		
        qm.saveAs(filename+".temp");
		return true;
    }

    public ManifestReader getManifestReader()
    {
    	return manifestReader;
    }

    private void cleanUp(String filename) {
    	try {
	    	FileSystem fs = FileSystems.getDefault();
	    	Path interRep = fs.getPath(filename+".ir");
	    	Path resRep = fs.getPath(filename+".res");
	    	Path staticRep = fs.getPath(filename+".stat");
	    	Path translateRep = fs.getPath(filename+".trans");
	    	Files.deleteIfExists(interRep);
	    	Files.deleteIfExists(resRep);
	    	Files.deleteIfExists(staticRep);
	    	Files.deleteIfExists(translateRep);
    	} catch (Exception exp) {
    		System.err.println("Unexpected exception:" + exp.getMessage());
			exp.printStackTrace();
    	}
    }
	
	public static void main(String[] args) {
		try {
			if (args.length != 1) {
				System.out.println("ERROR: Provide filename that needs to be translated");
				return;
			}
            String inputFile = args[0];
            if (inputFile.length() < 1 ) {
            	System.out.println("ERROR: Provide filename that needs to be translated");
            	return;
            }
            
            ResourceProfiler resourceProfiler = new ResourceProfiler();
            resourceProfiler.add(inputFile);
            new AppBuilder(inputFile, true, true, resourceProfiler);
            
		} catch (Exception exp) {
            System.err.println("Unexpected exception:" + exp.getMessage());
			exp.printStackTrace();
			return;
        }
	}
}

/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.resourceprofiler;

import java.util.Vector;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.JSONValue;

import java.util.HashMap;
import java.util.LinkedHashMap;

import amulet.resourceprofiler.JSONResourceReader.DeviceInfo;
import amulet.resourceprofiler.JSONResourceReader.EnergyParam;
import amulet.resourceprofiler.JSONResourceReader.SteadyStateInfo;
import amulet.resourceprofiler.Resource.ComputationType;
import amulet.resourceprofiler.Resource.QmStateMachineContext;
import amulet.resourceprofiler.Resource.ResourceType;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;


public class ResourceProfiler {

	/**
	 * This is the basic internal structure for a QM file and its resources.
	 */
	public class QMApp {
		public String filename;                          // Each QMApp record has an original filename.
		public Vector<Resource> resources;               // A list of Resource objects belonging to the QMApp (QMFile).
		public HashMap<String, Double> operationCostMap; // A map of names/estimated costs for functions defined by the application.
		public HashMap<String, Double> operationTimeMap; // A map of names/estimated execution times for functions defined by the application.
		
		public QMApp(String filename) {
			this.filename = filename;
			this.resources = new Vector<Resource>();
			this.operationCostMap = new HashMap<String, Double>();
			this.operationTimeMap = new HashMap<String, Double>();
		}
		
		public void addOperationRecord(String name, Double cost, Double time) {
			operationCostMap.put(name, cost);
			operationTimeMap.put(name, time);
		}
		
		public String debugPrintOperationMap() {
			String str = "\nOperations::\n";
			for (String key : operationCostMap.keySet()) {
				str += " + Operation:: name=" + key + ", cost=" + operationCostMap.get(key) + ", time=" + operationTimeMap.get(key) + "\n";
			}
			return str;
		}
	}

	/* ********************************************************************* *
	 *            v  Resource Profiler state & helper methods                *
	 * ********************************************************************* */
	
	/*
	 * Resource Profiler objects.
	 */
	private Vector<QMApp> m_qmapps;                      // A collection of QMApps (see: private class definition above)
	private QMApp m_currentQMApp;                        // Keep track of the current QMApp
	private JSONResourceReader m_jsonResourceReader;     // An object that holds parsed energy data from amulet hardware instrumentation/evaluation.
	private Vector<String> m_resourceProfilerWarnings;   // Aggregate warnings from the Resource Profiler.

	/*
	 * Resource Profiler state variables.
	 */
	private boolean resourceProfilerEnabled = true;      // Maintain the state of the Profiler (enabled/disabled) -- while disabled, the Profiler will not update internal structures.
	private HashMap<String, String> intVariableResolver; // HACK: keep track of integer-valued variables that may be used for for-loops, etc -- this can be used to resolve values used for conditional execution.
	public ProfilerContext profilerContext;              // The ResourceProfiler maintains "context" for some of its operations
	public LoopContext loopContext;                      // The ResourceProfiler maintains a "loop context"
		
	public ResourceProfiler() {
		m_qmapps = new Vector<QMApp>();
		m_jsonResourceReader = new JSONResourceReader();
		m_resourceProfilerWarnings = new Vector<String>();
		profilerContext = new ProfilerContext();
		loopContext = new LoopContext();
		intVariableResolver = new HashMap<String, String>();
	}

	public QMApp getCurrentQMAppObj() {
		return m_currentQMApp;
	}
	
	public boolean isEnabled() {
		return resourceProfilerEnabled;
	}
	
	public void disableResourceProfiler() {
		resourceProfilerEnabled = false;
	}
	
	public void enableResourceProfiler() {
		resourceProfilerEnabled = true;
	}
	
	public Vector<String> getRPWarnings() {
		return m_resourceProfilerWarnings;
	}
	
	/* ********************************************************************* *
	 *                           Convenience Methods                         *
	 * ********************************************************************* */
		
	/**
	 * Create a new QMApp object for the given filename.
	 * 
	 * NOTE: This operation sets the "state" of the Resource Profiler to 
	 * track the newly added application as the "current QM application."
	 * 
	 * @param filename The name of the QM file. 
	 */
	public void add(String filename) {
		QMApp qmapp = new QMApp(filename);
		m_qmapps.add( qmapp );
		m_currentQMApp = qmapp;
	}
	
	/**
	 * Convenience wrapper for adding a Resource object directly to a ResourceProfiler object. 
	 * Other overloaded methods could be implemented, but for now this one simply assumes that 
	 * before calling add(), the caller uses the Profiler Context handling methods (below) 
	 * to set contextual information before adding certain types of resources. 
	 * 
	 * Example: In AppBuilder.java, there is a large for-loop which goes over each QMClass in 
	 *          a QMFile. Within each QMClass are a collection of states, transitions, guards, etc., 
	 *          which are represented as XML "Node" objects for the most part. There are various classes/methods 
	 *          which are invoked to check the source code that exists within each of those XML Node objects. 
	 *          Since there are various classes/methods which handle the XML Nodes in a similar "context", 
	 *          the ResourceProfiler allows a context to be set, and then *ANY* resources that are added after 
	 *          that point will share some over-arching context. You can either call another set*Context() method 
	 *          to change the context information that is used when adding new resources to the ResourceProfiler, 
	 *          or you can simply "clear" the context by calling clearProfilerContext().
	 * 
	 * @param record The Resource object to add.
	 */
	public void add(Resource record) {
		// Update the resource record based on the Resource Profiler context objects.
		record.updateInfoFromResourceProfilerContext(profilerContext, loopContext);
		
		// Finally, record the resource in the app's list of resource records.
		m_currentQMApp.resources.add( record );
	}

	/**
	 * Add the cost of memory, parsed from the code, taking into account the type
	 * Check the types length here: http://mspgcc.sourceforge.net/manual/x987.html
	 * @param type The type of the variable (for arrays, the actual type)
	 * @param name The name of the variable
	 * @param length The length of the array
	 */
	public void addMemoryResource(String requestor, String type, String name, String length, ResourceType rtype) {
		// Assume char, so 1byte
		int multi = 0;
		switch(type) {
			case "uint16_t":
			case "int16_t":
			case "int":
				multi = 2;
				break;
			case "uint32_t":
			case "int32_t":
			case "long":
			case "float":
				multi = 4;
				break;
			case "long long":
			case "uint64_t":
			case "int64_t":
			case "double":
				multi = 8;
				break;
			default:
				multi = 1;
		}
		
		Resource memory_array = new Resource(name+":"+type, requestor, rtype, Integer.parseInt(length) * multi);
		add(memory_array);
	}
	
	/**
	 * Get a collection of only the resources which belong to the specified 
	 * QM state machine context from the entire collection of Resource objects.
	 * 
	 * @param resVector A collection of Resource objects ("records").
	 * @param resContext A context of interest (i.e., the query selector for identifying the context of resources). 
	 * @return A collection of Resources that belong to the given resource context.
	 */
	public Vector<Resource> getResourcesByContext(Vector<Resource> resVector, Resource.QmStateMachineContext desiredResourceCtx) {
		Vector<Resource> resSubsetVector = new Vector<Resource>();
		for (Resource resource : resVector) {
			if (resource.context == desiredResourceCtx) {
				resSubsetVector.add( resource );
			}
		}
		return resSubsetVector;
	}

	/**
	 * Get a collection of applicatio
	 * @param desiredMemoryResourceType
	 * @return
	 */
	public HashMap<String, Integer> getMemoryCosts(Resource.ResourceType desiredMemoryResourceType) {
		// Check: is desiredMemoryResourceType a valid memory type?
		if (desiredMemoryResourceType != ResourceType.MEMORY && desiredMemoryResourceType != ResourceType.GLOBAL_MEMORY) {
			return null;
		}

		HashMap<String, Integer> memCostMap = new HashMap<String, Integer>();
		
		// For each of the QM applications...
		for (QMApp qmapp : m_qmapps) {
			// For each of the Resources in this QM application...
			int sum = 0;
			for (Resource res: qmapp.resources) {
				// Keep a running sum of the "cost" of the desired memory type.
				if(res.type == desiredMemoryResourceType) {
					sum += res.cost;
				}
			}
			memCostMap.put(qmapp.filename, sum);
		}
		return memCostMap;
	}
	
	/* ********************************************************************* *
	 *                    ResourceProfiler Calculation Methods               *
	 * ********************************************************************* */
	
	/**
	 * Create a "cost profiler" object from a JSON formatted file which contains 
	 * a mapping of RESOURCE<-->ENERGY_COST which can be used by the ResourceProfiler 
	 * to compute "cost scores" that can be used to give application developers 
	 * feedback about their application's runtime/energy consumption behavior.
	 * 
	 * @param filename Name of the JSON file with energy cost information.
	 * 
	 * @warning There are typing issues with the parameterization of JSONObjects 
	 * in the `org.json.simple.*` package which require us to 'SupressWarnings' manually. 
	 * In the future we can implement a wrapper class or find a new JSON library to 
	 * solve this issue. 
	 * 
	 * TODO: each state here should have a JSON object written for stateEntryCost & stateExitCost.
	 *   --> this may mean we need to modify how we account for resources when "walking" the code. (state context variable for "entry" vs. "exit")
	 */
	@SuppressWarnings("unchecked")
	public String calculateCost(String filename) {
		/*
		 * 0. Load the lookup table
		 */
		BufferedReader br = null;
		String line = "";
		String dataFile = "AppBuildTool/src/amulet/resourceprofiler/clear_fill_pixel_lookup.csv";
		double[][] fill_rect_lookup = new double[128][128];
		double[][] clear_rect_lookup = new double[128][128];
		int line_counter = 0;
		try {
			br = new BufferedReader(new FileReader(dataFile));
			while ((line = br.readLine()) != null) {

			        // use comma as separator
				String[] table_entry = line.split(",");
				if(line_counter++ % 2 == 1) { // Odd is Clear
					clear_rect_lookup[Integer.parseInt(table_entry[2].trim())][Integer.parseInt(table_entry[3].trim())] = Double.parseDouble(table_entry[0].trim());
				} else {
					// Even is Fill
					fill_rect_lookup[Integer.parseInt(table_entry[2].trim())][Integer.parseInt(table_entry[3].trim())] = Double.parseDouble(table_entry[0].trim());
				}
			}

		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (br != null) {
				try {
					br.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

		/*
		 * 1. Load energy data from the input JSON file.
		 */
		m_jsonResourceReader.read(filename);
		
		/*
		 * 2. Update the resource records for the QM Apps known to the ResourceProfiler.
		 *  --> Use the JSON model file supplied by the device manufacturer.
		 */
		
		// Get primary data objects from the JSON resource reader.
		LinkedHashMap<String, EnergyParam> api_energy_lookup = m_jsonResourceReader.getEnergyParams();		
		DeviceInfo deviceInfo = m_jsonResourceReader.getDeviceInfo();
		SteadyStateInfo steadyStateInfo = m_jsonResourceReader.getSteadyStateInfo();
		
		// Update 1:: Resolve variables used in iterative blocks (e.g., for-loops).
		for (QMApp qmapp : m_qmapps) {
			for (Resource resource : qmapp.resources) {
				String strStopVal = resource.extras.get(Resource.EXTRA_LOOP_END_VALUE);
				if (strStopVal == null) 
					continue;
				
				// If the current "stop value" is an integer, it will be parsed successfully
				// and no update is necessary; if, however, a NumberFormatException is raised, 
				// attempt to look up the string (a variable name) in the simple intVariableResolver.
				try {
					Integer.parseInt(strStopVal);
				} catch (NumberFormatException nfe) {
					String resolvedValue = getResolverIntVar(strStopVal);
					if (resolvedValue != null) {
						// Resolved integer value found -- update the extra's value. 
						resource.extras.put(Resource.EXTRA_LOOP_END_VALUE, resolvedValue);
					}
				}
			}
		}
		
		// Update 2:: Update "cost" values for each operation defined in each QM application.
		for (QMApp qmapp : m_qmapps) {
			for (String currentOp : qmapp.operationCostMap.keySet()) {
				/* 
				 * NOTE--this only computes an ENERGY cost and EXECUTION TIME of calling some operation--no memory costs are calculated.
				 * This seems permissible since the memory is only used when the function is called and is "released" as 
				 * soon as the function returns; also, we have no intension of attributing "memory" costs to a state/transition 
				 * when it calls some Amulet API call or a non-Amulet API call. 
				 */
				
				// Compute the cost of all resources used in the current operation.
				double updatedOpCost = 0.0;
				double updatedOpTime = 0.0;
				for (Resource opResource : getResourcesByContext(qmapp.resources, QmStateMachineContext.OPERATION)) {
					if (opResource.getStringExtra(Resource.EXTRA_QM_OPERATION_NAME).equals( currentOp )) {
						updatedOpCost += getCostHelper(opResource, qmapp, deviceInfo, steadyStateInfo, api_energy_lookup, fill_rect_lookup, clear_rect_lookup);
						updatedOpTime += getTimeHelper(opResource, qmapp, deviceInfo, steadyStateInfo, api_energy_lookup, fill_rect_lookup, clear_rect_lookup);
					}
				}
				
				// Update the record in the operation map.
				qmapp.operationCostMap.put(currentOp, updatedOpCost);
				qmapp.operationTimeMap.put(currentOp, updatedOpTime);
			}
		}
		
		// Update 3:: Update "cost" values for each resource record with real power/time measurements from JSON file.
		for (QMApp qmapp : m_qmapps) {
			for (Resource resource : qmapp.resources) {
//				if(resource.name.contains("Amulet")) {
//					System.out.println("2 (before)************************" + resource);
//				}
				resource.cost = getCostHelper(resource, qmapp, deviceInfo, steadyStateInfo, api_energy_lookup, fill_rect_lookup, clear_rect_lookup);
				resource.time = getTimeHelper(resource, qmapp, deviceInfo, steadyStateInfo, api_energy_lookup, fill_rect_lookup, clear_rect_lookup);
//				if(resource.name.contains("Amulet")) {
//					System.out.println("2 (after)************************" + resource);
//				}
			}
		}

		/*
		 * 3. Compute sums, and provide feedback in the form of a JSON file for later use by any program. 
		 *  --> Currently the output file is used by our web application.
		 */
		JSONArray appJSONlist = new JSONArray();
		for (QMApp qmapp : m_qmapps) {
			/*
			 *  Record costs for energy, memory, and global memory resources across various application "contexts". 
			 */
			LinkedHashMap<String, Double> stateEnergySums = new LinkedHashMap<String, Double>();
			LinkedHashMap<String, Double> transitionEnergySums = new LinkedHashMap<String, Double>();
			//LinkedHashMap<String, Double> guardEnergySums = new LinkedHashMap<String, Double>();

			LinkedHashMap<String, Double> stateExecutionTimeSums = new LinkedHashMap<String, Double>();
			LinkedHashMap<String, Double> transitionExecutionTimeSums = new LinkedHashMap<String, Double>();
			//LinkedHashMap<String, Double> guardExecutionTimeSums = new LinkedHashMap<String, Double>();

			LinkedHashMap<String, Double> sensorSubscriptions = new LinkedHashMap<String, Double>();

			LinkedHashMap<String, Integer> timerSubscriptions = new LinkedHashMap<String, Integer>();

			LinkedHashMap<String, Double> stateLocalMemorySums = new LinkedHashMap<String, Double>();
			LinkedHashMap<String, Double> transitionLocalMemorySums = new LinkedHashMap<String, Double>();
			//LinkedHashMap<String, Double> guardLocalMemorySums = new LinkedHashMap<String, Double>();
			
			//LinkedHashMap<String, Double> globalMemorySums = new LinkedHashMap<String, Double>();
			
			// Iterate over each resource, summing costs along the way. 
			for (Resource resource : qmapp.resources) {
				// A temp holder for the current map to add resource to.
				LinkedHashMap<String, Double> currentCostSumMap = null;
				LinkedHashMap<String, Double> currentTimeSumMap = null;

				// High-level resource information.
				String name = null;
				Double cost = null;
				Double time = null;
				
				// Use information about the *CONTEXT* of the resource to determine where cost 
				// is incurred in the application/model.
				if (resource.isQmStateResource()) {
					// State.
					currentCostSumMap = stateEnergySums;
					currentTimeSumMap = stateExecutionTimeSums;
					
					// Use state name as "key".
					name = resource.getStringExtra(Resource.EXTRA_QM_STATE_NAME);
					cost = resource.cost;
					time = resource.time;
				} else if (resource.isQmActionResource()) {
					// Transition.
					currentCostSumMap = transitionEnergySums;
					currentTimeSumMap = transitionExecutionTimeSums;
					
					// Use SRC->TRAN->DEST as "key".
					String currentState = resource.getStringExtra(Resource.EXTRA_QM_ACTION_CURRENT_STATE_NAME);
					String trigger = resource.getStringExtra(Resource.EXTRA_QM_ACTION_TRIGGER_NAME);
					String targetState = resource.getStringExtra(Resource.EXTRA_QM_ACTION_TARGET_STATE_NAME);
					name = currentState + "->" + trigger + "->" + targetState;
					cost = resource.cost;
					time = resource.time;
				} else if (resource.isQmGuardResource()) {
					// Guard. IGNORE FOR NOW -- NO APPLICATIONS USE THESE (ANYMORE).
				} else if (resource.isQmOperationResource()) {
					// Operation. IGNORE FOR NOW -- THE COST OF OPERATIONS ARE COMPUTED BEFOREHAND AND ARE ADDED TO THE CALLING STATE/TRANSITION AS ANY OTHER FUNCTION CALL.
				}
				
				// Use information about the *TYPE* of resource (in combination with information from above) 
				// to compute energy/memory costs for relevant areas of the application.
				switch (resource.type) {
				case MEMORY:
					if (resource.isQmStateResource()) {
						// Local State Memory.
						currentCostSumMap = stateLocalMemorySums;
					} else if (resource.isQmActionResource()) {
						// Local Transition Memory.
						currentCostSumMap = transitionLocalMemorySums;
					}
					break;
				case GLOBAL_MEMORY:
					//currentSumMap = globalMemorySums;
					break;
				case SENSOR_SUBSCRIPTION:
					name = resource.name;
					currentCostSumMap = sensorSubscriptions;
					currentTimeSumMap = null;
					break;
				case TIMER_SUBSCRIPTION:
//					timerSubscriptions.put("AmuletTimer", resource.getIntExtra(Resource.EXTRA_TIMER_DELAY_IN_SECONDS));
					timerSubscriptions.put("Timer", resource.getIntExtra(Resource.EXTRA_TIMER_DELAY_IN_SECONDS));
				case AMULET_API_FUNCTION_CALL:
					break;
				case NON_AMULET_API_FUNCTION_CALL:
					break;
				case COMPUTATION:
					break;
				case UNKNOWN:
					break;
				default:
					// Do nothing (at least for now).
					break;
				}
				
				// If an appropriate "cost sum" map has been set, add to it. 
				if (currentCostSumMap != null && name != null & cost != null) {
					sumMapHelper(currentCostSumMap, name, cost);
				}
				
				// If an appropriate "time sum" map has been set, add to it. 
				if (currentTimeSumMap != null && name != null & time != null) {
					sumMapHelper(currentTimeSumMap, name, time);
				}
			}
			
			// Slightly easier to read DEBUG output...
//			System.out.println("=======> DEBUG stateEnergySums::" + stateEnergySums);
//			System.out.println("=======> DEBUG transitionEnergySums::" + transitionEnergySums);
//			System.out.println("=======> DEBUG localMemorySums::" + localMemorySums);
//			System.out.println("=======> DEBUG globalMemorySums::" + globalMemorySums);
		
			/*
			 * 4. Make JSON object(s) and return string representation that will be written out. 
			 */
			JSONObject appCostJSONObject = new JSONObject();
			
			JSONObject stateEnergyCostJSON = new JSONObject(stateEnergySums);
			JSONObject transitionEnergyCostJSON = new JSONObject(transitionEnergySums);
			//JSONObject guardEnergyCostJSON = new JSONObject(guardEnergySums);

			JSONObject stateExecutionTimesJSON = new JSONObject(stateExecutionTimeSums);
			JSONObject transitionExecutionTimesJSON = new JSONObject(transitionExecutionTimeSums);
			//JSONObject guardExecutionTimesJSON = new JSONObject(guardExecutionTimeSums);

			JSONObject sensorSubscriptionsJSON = new JSONObject(sensorSubscriptions);
			
			JSONObject timerSubscriptionsJSON = new JSONObject(timerSubscriptions);

			JSONObject stateLocalMemoryCostJSON = new JSONObject(stateLocalMemorySums);
			JSONObject transitionLocalMemoryCostJSON = new JSONObject(transitionLocalMemorySums);
			//JSONObject guardLocalMemoryCostJSON = new JSONObject(guardLocalMemorySums);
			
//			int globalMemoryCost = getGlobalMemoryCosts().get(qmapp.filename);
			int globalMemoryCost = getMemoryCosts(ResourceType.GLOBAL_MEMORY).get(qmapp.filename);
			
			// Write data to JSON "App Costs" object.
			appCostJSONObject.put("filename", qmapp.filename);
			appCostJSONObject.put("device_info", deviceInfo.jsonDeviceInfo);                  // hack: add to each app
			appCostJSONObject.put("steady_state_info", steadyStateInfo.jsonSteadyStateInfo);  // hack: add to each app
			appCostJSONObject.put("state_energy_costs", stateEnergyCostJSON);
			appCostJSONObject.put("transition_energy_costs", transitionEnergyCostJSON);
			//appCostJSONObject.put("guard_energy_costs", guardEnergyCostJSON);
			appCostJSONObject.put("state_execution_times", stateExecutionTimesJSON);
			appCostJSONObject.put("transition_execution_times", transitionExecutionTimesJSON);
			//appCostJSONObject.put("guard_execution_times", guardExecutionTimesJSON);
			appCostJSONObject.put("app_sensor_subscriptions", sensorSubscriptionsJSON);
			appCostJSONObject.put("app_timer_subscriptions", timerSubscriptionsJSON);
			appCostJSONObject.put("state_sram_costs", stateLocalMemoryCostJSON);
			appCostJSONObject.put("transition_sram_costs", transitionLocalMemoryCostJSON);
			//appCostJSONObject.put("guard_sram_costs", guardLocalMemoryCostJSON);
			appCostJSONObject.put("app_fram_costs", globalMemoryCost);
			
			appJSONlist.add(appCostJSONObject);
		}

		JSONObject costsJSONObject = new JSONObject();
		costsJSONObject.put("applications", appJSONlist);
		costsJSONObject.put("device_info", deviceInfo.jsonDeviceInfo);
		costsJSONObject.put("steady_state_info", steadyStateInfo.jsonSteadyStateInfo);
		costsJSONObject.put("api_calls", m_jsonResourceReader.getApiInfo());
		
		return JSONValue.toJSONString(costsJSONObject);
	}
	
	private void sumMapHelper(HashMap<String, Double> sums, String name, double value) {
		if(sums.containsKey(name)) {
			sums.put(name, value+sums.get(name));
		} else {
			sums.put(name, value); 
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	/*
	 * TODO: [UPDATE ME AS NEEDED] -- Used for providing dummy data when nothing better can be done.
	 */
	private int UNKNOWN_QUANTITY_SCALAR = 2;
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Helper function for getting the AVG. EXECUTION TIME for a particular resource.
	 * @param resource
	 * @param qmapp
	 * @param deviceInfo
	 * @param steadyStateInfo
	 * @param api_energy_lookup
	 * @return
	 */
	private double getTimeHelper(Resource resource, QMApp qmapp, DeviceInfo deviceInfo, SteadyStateInfo steadyStateInfo, LinkedHashMap<String, EnergyParam> api_energy_lookup,  double[][] fill_rect_lookup, double[][] clear_rect_lookup) {
		// Compute the avg. time to execute a line of code (LoC).
		double AVG_LOC_TIME = deviceInfo.avgNumInstructionsPerLoC * deviceInfo.avgBasicInstructionTime;
		
		// Define an arbitrary time to give to un-recognized function calls/operations. 
		double UNKNOWN_TIME = UNKNOWN_QUANTITY_SCALAR * AVG_LOC_TIME;
		
		// A time variable for keeping track of the time value to be returned to the caller. 
		double time = resource.time;
		
		switch (resource.type) {
		case AMULET_API_FUNCTION_CALL:
			if (api_energy_lookup.containsKey(resource.name)) {
				// If the Amulet API call is recognized, then we just assign the real measurement value.
				EnergyParam energyparam = api_energy_lookup.get(resource.name);
				time = energyparam.avgTime;

				if(resource.name.contains("ClearRect")) {
					// MAGIC NUMBERS!!! Gawd this whole thing is a bit hacky for the special lookup functions
					int w = Math.min(resource.getIntExtra("width"), 127);
					int h = Math.min(resource.getIntExtra("height"), 114);
					time = clear_rect_lookup[w][h];
				}

				if(resource.name.contains("FillRect")) {
					int w = Math.min(resource.getIntExtra("width"), 127);
					int h = Math.min(resource.getIntExtra("height"), 114);
					time = fill_rect_lookup[w][h];
				}

			} else {
				// If the Amulet API call is *not* recognized, then we just assign a fixed time.
				m_resourceProfilerWarnings.add( "  + (!) LOOK-UP WARNING:: Time for Amulet API function '"+resource.name+"' not found in api_energy_lookup table; assigning UNKNOWN_TIME="+UNKNOWN_TIME+"." );
				time = UNKNOWN_TIME;
			}

			// If this call is nested within a loop, the time of this function call (really, the 
			// resource record) needs to be multiplied by the number of times this call is actually made.
			if (resource.isContainedInLoop()) {
//				System.out.println("**** RESOURCE " + resource.name + " time was = " + time);
				time *= resource.getNumLoopIterations();
//				System.out.println("**** RESOURCE " + resource.name + " is now = " + time + " (orignal-time x " + resource.getNumLoopIterations() + ")");
			}
			
			break;
		case NON_AMULET_API_FUNCTION_CALL:
			if (qmapp.operationTimeMap.containsKey(resource.name)) {
				// This is a function defined within the QM application (i.e., an "operation" by QM's definition). 
				time = qmapp.operationTimeMap.get(resource.name);
			} else {
				// If the Non-Amulet API call is *not* recognized, then we just assign a fixed cost.
				m_resourceProfilerWarnings.add( "  + (!) LOOK-UP WARNING:: Time for Non-Amulet API function '"+resource.name+"' not found; assigning UNKNOWN_TIME="+UNKNOWN_TIME+"." );
				time = UNKNOWN_TIME;
			}
			
			// If this call is nested within a loop, the time of this function call (really, the 
			// resource record) needs to be multiplied by the number of times this call is actually made.
			if (resource.isContainedInLoop()) {
//				System.out.println("**** RESOURCE " + resource.name + " time was = " + time);
				time *= resource.getNumLoopIterations();
//				System.out.println("**** RESOURCE " + resource.name + " is now = " + time + " (orignal-time x " + resource.getNumLoopIterations() + ")");
			}
			
			break;
		case COMPUTATION:
			if (resource.name.equals(ComputationType.BASIC_BLOCKS.text())) {
				// Get the number of lines of code.
				double nLinesOfCode = resource.getIntExtra(Resource.EXTRA_NUM_LINES_OF_CODE);

				time = nLinesOfCode * AVG_LOC_TIME;
			} else if (resource.name.equals(ComputationType.FOR_LOOP.text())) {
				// Get number of iterations in this loop.
				double nIterations = resource.getNumLoopIterations();

				// Get the number of lines of code.
				double nLinesOfCode = resource.getIntExtra(Resource.EXTRA_LOOP_NUM_STATEMENTS);

				time = (nLinesOfCode * AVG_LOC_TIME) * nIterations;
			}
			break;
		case SENSOR_SUBSCRIPTION:
			if (resource.name.equalsIgnoreCase("ACCELEROMETER")) {
				time = 0.0; // no time here exactly -- compute.js scales the subscription to be in terms of "per week"
			} else if (resource.name.equalsIgnoreCase("HEARTRATE")) {
				time = 0.0; // no time here exactly -- compute.js scales the subscription to be in terms of "per week"
			}
			break;
		case MEMORY:
		case GLOBAL_MEMORY:
		case UNKNOWN:
		default:
		}
		
		return time;
	}	
	
	/**
	 * Helper function for getting the AVG. ENERGY COST for a particular resource.
	 * @param resource
	 * @param qmapp
	 * @param deviceInfo
	 * @param steadyStateInfo
	 * @param api_energy_lookup
	 * @return
	 */
	private double getCostHelper(Resource resource, QMApp qmapp, DeviceInfo deviceInfo, SteadyStateInfo steadyStateInfo, LinkedHashMap<String, EnergyParam> api_energy_lookup, double[][] fill_rect_lookup, double[][] clear_rect_lookup) {
		// Compute the avg. energy cost to execute a line of code (LoC).
		double ENERGY_COST_PER_LOC = deviceInfo.avgNumInstructionsPerLoC * (deviceInfo.avgBasicInstructionPower * deviceInfo.avgBasicInstructionTime);
		
		// Define an arbitrary cost to give to un-recognized function calls. 
		double UNKNOWN_COST = UNKNOWN_QUANTITY_SCALAR * ENERGY_COST_PER_LOC;
		
		// A cost variable for keeping track of the cost value to be returned to the caller. 
		double cost = resource.cost;
		
		switch (resource.type) {
		case AMULET_API_FUNCTION_CALL:
			if (api_energy_lookup.containsKey(resource.name)) {
				// If the Amulet API call is recognized, then we just assign the real measurement value.
				EnergyParam energyparam = api_energy_lookup.get(resource.name);
				
				// TODO: is there a better/more correct way to do this?!
				// If cost is zero, calculate actual cost with known energy values; otherwise, leave it alone.
				double scalar = cost;
				if (cost == 0.0) {
					scalar = 1.0;
				}

				cost = scalar * energyparam.avgPower * energyparam.avgTime;

				if(resource.name.contains("ClearRect")) {
					int w = Math.min(resource.getIntExtra("width"), 127);
					int h = Math.min(resource.getIntExtra("height"), 114);
					double time_cost = clear_rect_lookup[w][h];
					cost = scalar * energyparam.avgPower * time_cost;
					System.out.println("In clear "+cost);
				}

				if(resource.name.contains("FillRect")) {
					int w = Math.min(resource.getIntExtra("width"), 127);
					int h = Math.min(resource.getIntExtra("height"), 114);
					double time_cost = fill_rect_lookup[w][h];
					cost = scalar * energyparam.avgPower * time_cost;
				}
				
			} else {
				// If the Amulet API call is *not* recognized, then we just assign a fixed cost.
				m_resourceProfilerWarnings.add( "  + (!) LOOK-UP WARNING:: Cost for Amulet API function '"+resource.name+"' not found in api_energy_lookup table; assigning UNKNOWN_COST="+UNKNOWN_COST+"." );
				cost = UNKNOWN_COST;
			}
			
			// If this call is nested within a loop, the cost of this function call needs to 
			// be multiplied by the number of times this call is actually made.
			if (resource.isContainedInLoop()) {
//				System.out.println("**** RESOURCE " + resource.name + " cost was = " + cost);
				cost *= resource.getNumLoopIterations();
//				System.out.println("**** RESOURCE " + resource.name + " is now = " + cost + " (orignal-cost x " + resource.getNumLoopIterations() + ")");
			}
			
			break;
		case NON_AMULET_API_FUNCTION_CALL:
			if (qmapp.operationCostMap.containsKey(resource.name)) {
				// This is a function defined within the QM application (i.e., an "operation" by QM's definition). 
				cost = qmapp.operationCostMap.get(resource.name) * qmapp.operationTimeMap.get(resource.name);
			} else {
				// If the Non-Amulet API call is *not* recognized, then we just assign a fixed cost.
				m_resourceProfilerWarnings.add( "  + (!) LOOK-UP WARNING:: Cost for Non-Amulet API function '"+resource.name+"' not found; assigning UNKNOWN_COST="+UNKNOWN_COST+"." );
				cost = UNKNOWN_COST;
			}
			
			// TODO: scalar handling?!

			// If this call is nested within a loop, the cost of this function call needs to 
			// be multiplied by the number of times this call is actually made.
			if (resource.isContainedInLoop()) {
//				System.out.println("**** RESOURCE " + resource.name + " cost was = " + cost);
				cost *= resource.getNumLoopIterations();
//				System.out.println("**** RESOURCE " + resource.name + " is now = " + cost + " (orignal-cost x " + resource.getNumLoopIterations() + ")");
			}
			
			break;
		case COMPUTATION:
			if (resource.name.equals(ComputationType.BASIC_BLOCKS.text())) {
				// Get the number of lines of code.
				double nLinesOfCode = resource.getIntExtra(Resource.EXTRA_NUM_LINES_OF_CODE);

				// Calculate cost.
				cost = nLinesOfCode * ENERGY_COST_PER_LOC;
				
			} else if (resource.name.equals(ComputationType.FOR_LOOP.text())) {
				// Get the number of lines of code.
				double nLinesOfCode = resource.getIntExtra(Resource.EXTRA_LOOP_NUM_STATEMENTS);
				
				// Get number of iterations in this loop.
				double nIterations = resource.getNumLoopIterations();
				
				// Calculate cost.
				cost = (ENERGY_COST_PER_LOC * nLinesOfCode) * nIterations;
				
			}
			break;
		case SENSOR_SUBSCRIPTION:
			if (resource.name.equalsIgnoreCase("ACCELEROMETER")) {
				cost = steadyStateInfo.sensorAccelerometer;
			} else if (resource.name.equalsIgnoreCase("HEARTRATE")) {
				cost = steadyStateInfo.sensorHeartRate;
			}
			break;
		case MEMORY:
		case GLOBAL_MEMORY:
		case UNKNOWN:
		default:
		}
		
		return cost;
	}
	
	/* ********************************************************************* *
	 *                       Profiler Context Handling                       *
	 * ********************************************************************* */
	
	/**
	 * A simple structure/class for carrying around contextual information 
	 * relevant to what can be extracted from XML nodes in a QMFile (i.e., QM Application). 
	 * 
	 * When analyzing the application resources, there is high-level QM "state" that is
	 * relevant for a collection of resources that are being recorded---this structure 
	 * holds that high-level state and is used to annotate resource "records" when they 
	 * are added to this profiler. 
	 */
	public class ProfilerContext {
		public QmStateMachineContext context;
		public String className;
		public String stateName;
		public String actionCurrentState;
		public String actionTriggerName;
		public String actionTargetName;
		public String operationName;
		public String operationReturnType;
		public String operationVisibility;
		public String operationProperties;
		public int nLinesOfCode;
	}
	
	public int getNumLinesOfCode() {
		return profilerContext.nLinesOfCode;
	}
	
	public void incNumLinesOfCode() {
		profilerContext.nLinesOfCode++;
	}
	
	/**
	 * A well-defined "context" that should be set before adding Resource Records 
	 * to the ResourceProfiler in the context of a QM State in the QM Application. 
	 * 
	 * @param stateName The name of the current state.
	 */
	public void setStateContext(String stateName) {
		clearProfilerContext();
		profilerContext.context = QmStateMachineContext.STATE;
		profilerContext.stateName = stateName;
	}
	
	/**
	 * A well-defined "context" that should be set before adding Resource Records 
	 * to the ResourceProfiler in the context of a QM Action (e.g., Transitions) 
	 * in the QM Application. 
	 * 
	 * @param triggerName Name of the trigger that invokes the transition.
	 * @param targetName A reference to the state that will be transitioned to after 
	 *        the transition code runs. NOTE: QM simplifies this to be a number (each 
	 *        state is given a number and not referred to be name), thus, this is 
	 *        not very user friendly right now -- @todo consider resolving 
	 *        target # <--> target name mapping. 
	 */
	public void setActionContext(String currentState, String triggerName, String targetName) {
		clearProfilerContext();
		profilerContext.context = QmStateMachineContext.ACTION;
		profilerContext.actionCurrentState = currentState;
		profilerContext.actionTriggerName = triggerName;
		profilerContext.actionTargetName = targetName;
	}
	
	/**
	 * A well-defined "context" that should be set before adding Resource Records 
	 * to the ResourceProfiler in the context of a QM Guard in the QM Application. 
	 * 
	 * @note These appear to be very simple (no interesting context to capture...).
	 * This really only exists for consistency with other "contexts" in a QM Application 
	 * file.
	 */
	public void setGuardContext() {
		clearProfilerContext();
		profilerContext.context = QmStateMachineContext.GUARD;
	}
	
	/**
	 * A well-defined "context" that should be set before adding Resource Records 
	 * to the ResourceProfiler in the context of a QM Operation (e.g., stand-alone function) 
	 * in the QM Application. 
	 * 
	 * @param opName The name of the function.
	 * @param opReturnType The return type of the function.
	 * @param opVisibility The visibility of the function (in HEX -- is this needed?).
	 * @param opProperties The properties for the function (QM Specific, I think -- is this needed?).
	 */
	public void setOperationContext(String opName, String opReturnType, String opVisibility, String opProperties) {
		clearProfilerContext();
		profilerContext.context = QmStateMachineContext.OPERATION;
		profilerContext.operationName = opName;
		profilerContext.operationReturnType = opReturnType;
		profilerContext.operationVisibility = opVisibility;
		profilerContext.operationProperties = opProperties;
	}

	/**
	 * A well-defined "context" that should be set before adding Resource Records to the ResourceProfiler in 
	 * the context of a QM Class, but no specific QM Node (e.g., variables declared in global scope).
	 */
	public void setGlobalContext(String qmclassName) {
		clearProfilerContext();
		profilerContext.context = QmStateMachineContext.GLOBAL;
		profilerContext.className = qmclassName;
	}

	/**
	 * Clear the ResourceProfiler's current context.
	 */
	public void clearProfilerContext() {
		profilerContext.context = QmStateMachineContext.NONE;
		profilerContext.className = null;
		profilerContext.stateName = null;
		profilerContext.actionCurrentState = null;
		profilerContext.actionTriggerName = null;
		profilerContext.actionTargetName = null;
		profilerContext.operationName = null;
		profilerContext.operationReturnType = null;
		profilerContext.operationVisibility = null;
		profilerContext.operationProperties = null;
		profilerContext.nLinesOfCode = 0;
	}
	
	public void printProfilerContext() {
		String str = "ProfilerContext::";
		if (profilerContext != null) {
			str += "\n + context=" + profilerContext.context;
			str += "\n + className=" + profilerContext.className;
			str += "\n + stateName=" + profilerContext.stateName;
			str += "\n + actionCurrentState=" + profilerContext.actionCurrentState;
			str += "\n + actionTriggerName=" + profilerContext.actionTriggerName;
			str += "\n + actionTargetName=" + profilerContext.actionTargetName;
			str += "\n + operationName=" + profilerContext.operationName;
			str += "\n + operationReturnType=" + profilerContext.operationReturnType;
			str += "\n + operationVisibility=" + profilerContext.operationVisibility;
			str += "\n + operationProperties=" + profilerContext.operationProperties;
			str += "\n + nLinesOfCode=" + profilerContext.nLinesOfCode;
		} else {
			str += " null context";
		}
		System.out.println( str );
	}
	
	/* ********************************************************************* *
	 *                         Loop Context Handling                         *
	 * ********************************************************************* */
	
	public enum LoopContextType {
		NONE,
		FOR,
		WHILE
	}
	
	public class LoopContext {
		public LoopContextType context;
		public String start;
		public String end;
		public String update;
		public int nStatements = 0;
		
		public boolean isForLoopContext () {
			return (context == LoopContextType.FOR);
		}
		
		public int getNumStatements() {
			return nStatements;
		}
		
		public void incNumStatements() {
			nStatements++;
		}
		
		public void reset() {
			context = LoopContextType.NONE;
			start = null;
			end = null;
			update = null;
			nStatements = 0;
		}
	}
	
	public void enterForLoopContext(String start, String end, String update) {
		loopContext.context = LoopContextType.FOR;
		loopContext.start = start;
		loopContext.end = end;
		loopContext.update = update;
	}
		
	public void incNumberStatementsInLoop() {
		loopContext.incNumStatements();
	}

	public int getNumberStatementsInLoop() {
		return loopContext.getNumStatements();
	}
	
	public void exitForLoopContext() {
//		System.out.println("DEBUG::ResourceProfiler::exitForLoopContext()::nStatements="+getNumberStatementsInLoop());
		loopContext.reset();
	}
	
	/* ********************************************************************* *
	 *                          Hack-related Methods                         *
	 * ********************************************************************* */

	public void setResolverIntVar(String variable, String value) {
		intVariableResolver.put(variable, value);
	}
	
	public String getResolverIntVar(String variable) {
		return intVariableResolver.get(variable);
	}
	
	public String debugPrintIntVariableResolver() {
		String str = "\nInteger Variable Resolver:\n";
		for(String key : intVariableResolver.keySet()) {
			str += " + Variable:: " + key + "=" + intVariableResolver.get(key) + "\n";
		}
		return str;
	}
	
	/* ********************************************************************* *
	 *                         Output-related Methods                        *
	 * ********************************************************************* */
	
	/**
	 * A nice string representation of the ResourceProfilers. 
	 * 
	 * TODO Implement writing the ResourceProfiler contents to a file. 
	 */
	public String toString() {
		boolean showMemory = true;
		
		String str = "\n[Begin] ResourceProfiler ===================================\n";
		str += m_jsonResourceReader.toString();
		str += debugPrintIntVariableResolver();

		HashMap<String, Integer> localCostMap = getMemoryCosts(ResourceType.MEMORY);
		HashMap<String, Integer> globalCostMap = getMemoryCosts(ResourceType.GLOBAL_MEMORY);
		
		for (QMApp qmapp : m_qmapps) {
			int gcost = globalCostMap.get(qmapp.filename);
			int lcost = localCostMap.get(qmapp.filename);

			str += "\nFile::" + qmapp.filename + " | resource records=" + qmapp.resources.size() +" | local (SRAM) memory cost (bytes)=" + lcost+" | global (FRAM) memory cost (bytes)=" + gcost + "\n";
			str += qmapp.debugPrintOperationMap();
			
			// Build QMClass Info. for the QMApp (QMFile).
			str += "\nResources::\n";
			for (Resource resource : qmapp.resources) {
				if (!showMemory && (resource.type == ResourceType.MEMORY || resource.type == ResourceType.GLOBAL_MEMORY))
					continue;
				
				str += " + " + resource + "\n";
			}
		}
		
		str +=       "\n[End] ResourceProfiler =====================================";
		return str;
	}
	
}

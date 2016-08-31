/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.resourceprofiler;

import java.util.HashMap;

import amulet.resourceprofiler.ResourceProfiler.LoopContext;
import amulet.resourceprofiler.ResourceProfiler.ProfilerContext;

public class Resource {

	/**
	 * In order to characterize resource usage, we identify resources 
	 * by a small set of categories ("types"). 
	 */
	public enum ResourceType {
		UNKNOWN,
		AMULET_API_FUNCTION_CALL,
		NON_AMULET_API_FUNCTION_CALL,
		SENSOR_SUBSCRIPTION,
		TIMER_SUBSCRIPTION,
		MEMORY,
		GLOBAL_MEMORY, 
		COMPUTATION
	}
	
	/**
	 * QM State Machine-specific "contexts" (i.e., all places where code can exist 
	 * and, thus, where a resource can be used/allocated).
	 */
	public enum QmStateMachineContext {
		NONE,           // default
		STATE, 
		ACTION, 
		GUARD, 
		OPERATION,
		GLOBAL
	}

	
	/**
	 * When identifying a COMPUTATION resource, ComputationType is intended 
	 * to identify a sort of "sub-type" or specifier for the COMPUTATION.
	 * 
	 * TODO: later we may consider more fine-grained types such as MATH, etc.
	 */
	public enum ComputationType {
		UNKNOWN("UNKNOWN"),
		BASIC_BLOCKS("BASIC_BLOCKS"),
		FOR_LOOP("FOR-LOOP"),
		WHILE_LOOP("WHILE-LOOP");
		
		private String text;
		private ComputationType(String text) { this.text = text; }		
		public String text() { return this.text; }
	}
	
	public String name;                       // Name of the resource in question.
	public String requestor;                  // Requester/user of the resource in question (identified by QM class).
	public ResourceType type;                 // Type of resource (see: ResourceType).
	public double cost;                       // "Cost" of the resource (e.g., energy cost, memory cost).
	public double time;                       // Execution "time" for the resource.
	public QmStateMachineContext context;     // Context of the resource being used (see: ResourceCaptureContext).

	// HashMap for holding extra data of interest about various resources.
	public HashMap<String, String> extras;
	
	// Extras for contextual information
	public static String EXTRA_QM_STATE_NAME = "qmStateName";
	public static String EXTRA_QM_ACTION_CURRENT_STATE_NAME = "qmActionCurrentStateName";
	public static String EXTRA_QM_ACTION_TRIGGER_NAME = "qmActionTriggerName";
	public static String EXTRA_QM_ACTION_TARGET_STATE_NAME = "qmActionTargetStateName";
	public static String EXTRA_QM_OPERATION_NAME = "qmOperationName";
	public static String EXTRA_QM_OPERATION_RETURN_TYPE = "qmOperationReturnType";
	public static String EXTRA_QM_OPERATION_VISIBILITY = "qmOperationVisibility";
	public static String EXTRA_QM_OPERATION_PROPERTIES = "qmOperationProperties";

	// Extras related to the number of Lines of Code (LoC).
	public static String EXTRA_NUM_LINES_OF_CODE = "LoC";
	
	// Extras related to iterative blocks (e.g., for-loops).
	public static String EXTRA_FUNCTION_CALL_IN_LOOP = "thisFuncCallInLoop";
	public static String EXTRA_LOOP_START_VALUE = "loopStartVal";
	public static String EXTRA_LOOP_END_VALUE = "loopEndVal";
	public static String EXTRA_LOOP_UPDATE_VALUE = "loopUpdateVal";
	public static String EXTRA_LOOP_NUM_STATEMENTS = "nStatements";
	
	// Extras for timers
	public static String EXTRA_TIMER_DELAY_IN_SECONDS = "timerDelayInSeconds";
	
	/*
	 * Constructor(s).
	 */

	public Resource(Resource resource) {
		this.name = resource.name;
		this.requestor = resource.requestor;
		this.type = resource.type;
		this.cost = resource.cost;
		this.context = resource.context;
		this.cost = resource.time;
		this.extras = (resource.extras != null) ? resource.extras : new HashMap<String, String>();
//		this.extras = new HashMap<String, String>();
	}
	
	public Resource(String name, String requestor, ResourceType type, double cost) {
		this.name = name;
		this.requestor = requestor;
		this.type = type;
		this.cost = cost;
		this.context = QmStateMachineContext.NONE;
		this.time = 0.0;
		this.extras = new HashMap<String, String>();
	}
	
	public HashMap<String, String> getExtras() {
		return extras;
	}
	
	// NOTE: no special handling is done here so MAKE SURE the value is an int.
	public int getIntExtra(String key) {
		return Integer.parseInt( extras.get(key) );
	}
	
	public String getStringExtra(String key) {
		return extras.get(key);
	}
	
	public void addExtra(String key, String value) {
		extras.put(key, value);
	}

	public void addExtra(String key, int value) {
		extras.put(key, Integer.toString(value));
	}

	public String toString() {
		String str = "Resource:: ";
		str += "name=" + name;
		str += ", requestor=" + requestor;
		str += ", type=" + type;
		str += ", context=" + context;
		str += ", cost=" + cost;
		str += ", time=" + time;

		// Add extras to the toString output as well!
		for(String key : extras.keySet()) {
			str += ", "+key+"="+extras.get(key);
		}
		
		return str;
	}
	
	/*
	 * QM State Machine context-related helper methods.
	 */
	
	public boolean isQmStateResource() {
		return context == QmStateMachineContext.STATE;
	}
	
	public boolean isQmActionResource() {
		return context == QmStateMachineContext.ACTION;
	}
	
	public boolean isQmGuardResource() {
		return context == QmStateMachineContext.GUARD;
	}
	
	public boolean isQmOperationResource() {
		return context == QmStateMachineContext.OPERATION;
	}
	
	public boolean isQmGlobalResource() {
		return context == QmStateMachineContext.GLOBAL;
	}
	
	/**
	 * Given external contextual information, update the resource record.
	 * 
	 * For specific information about the type of context information, see:
	 *   + ProfilerContext : defined in ResourceProfiler.java
	 *   + LoopContext     : defined in ResourceProfiler.java
	 */
	public void updateInfoFromResourceProfilerContext(ProfilerContext profilerContext, LoopContext loopContext) {
		// Set the context of the resource record to the current context of the ResourceProfiler.
		context = profilerContext.context;
		
		// Set the requestor field (if not set already + if QM class is defined).
		if (requestor == null && profilerContext.className != null) {
			requestor = profilerContext.className;
		}
		
		/*
		 * Record Modifications Based on Profiler Context.
		 * > Add specific meta-information to the Resource based on the profiler 
		 *   context (i.e., where the resource was found).
		 */
		switch (context) {
		case STATE:
			addExtra(Resource.EXTRA_QM_STATE_NAME, profilerContext.stateName);
			break;
		case ACTION:
			addExtra(Resource.EXTRA_QM_ACTION_CURRENT_STATE_NAME, profilerContext.actionCurrentState);
			addExtra(Resource.EXTRA_QM_ACTION_TRIGGER_NAME, profilerContext.actionTriggerName);
			addExtra(Resource.EXTRA_QM_ACTION_TARGET_STATE_NAME, profilerContext.actionTargetName);
			break;
		case OPERATION:
			addExtra(Resource.EXTRA_QM_OPERATION_NAME, profilerContext.operationName);
			addExtra(Resource.EXTRA_QM_OPERATION_RETURN_TYPE, profilerContext.operationReturnType);
			addExtra(Resource.EXTRA_QM_OPERATION_VISIBILITY, profilerContext.operationVisibility);
			addExtra(Resource.EXTRA_QM_OPERATION_PROPERTIES, profilerContext.operationProperties);
			break;
		case GUARD:
			break;
		case GLOBAL:
			break;
		case NONE:
			break;
		default:
			// ERROR: INVALID RESOURCE CONTEXT.
			break;
		}		
		
		/*
		 * Record Modifications Based on Record Type.
		 * > Update/modify record information based on the record's type.
		 */
		switch (type) {
		case COMPUTATION:
			if ((name == ComputationType.FOR_LOOP.text()) && loopContext.isForLoopContext()) {
				addExtra(Resource.EXTRA_LOOP_START_VALUE, loopContext.start);
				addExtra(Resource.EXTRA_LOOP_END_VALUE, loopContext.end);
				addExtra(Resource.EXTRA_LOOP_UPDATE_VALUE, loopContext.update);
				addExtra(Resource.EXTRA_LOOP_NUM_STATEMENTS, loopContext.getNumStatements());
			} else if ((name == ComputationType.BASIC_BLOCKS.text()) && !loopContext.isForLoopContext()) {
				addExtra(Resource.EXTRA_NUM_LINES_OF_CODE, profilerContext.nLinesOfCode);
			}
			break;
		case UNKNOWN:
			// No modifications needed.
			break;
		case AMULET_API_FUNCTION_CALL:
		case NON_AMULET_API_FUNCTION_CALL:
			// If a function call is made in the context of a forloop, we need to 
			// account for the fact that this call is made repeated times---annotate
			// this record with information about the # of iterations for the containing loop
			if (loopContext.isForLoopContext()) {
				addExtra(Resource.EXTRA_FUNCTION_CALL_IN_LOOP, "true");
				addExtra(Resource.EXTRA_LOOP_START_VALUE, loopContext.start);
				addExtra(Resource.EXTRA_LOOP_END_VALUE, loopContext.end);
				addExtra(Resource.EXTRA_LOOP_UPDATE_VALUE, loopContext.update);
			}
			break;
		case SENSOR_SUBSCRIPTION:
			// No modifications needed.
			break;
		case MEMORY:
			// No modifications needed.
			break;
		case GLOBAL_MEMORY:
			// No modifications needed.
			break;
		default:
			// ERROR: INVALID RESOURCE TYPE.
			break;
		}
	}
	
	/*
	 * QM resource type-related helper methods.
	 */
	
	public static boolean isMemoryResource(Resource resource) {
		return (resource.type == ResourceType.MEMORY || resource.type == ResourceType.GLOBAL_MEMORY);
	}

	public static boolean isEnergyResource(Resource resource) {
		return (resource.type == ResourceType.AMULET_API_FUNCTION_CALL || resource.type == ResourceType.NON_AMULET_API_FUNCTION_CALL || resource.type == ResourceType.SENSOR_SUBSCRIPTION || resource.type == ResourceType.COMPUTATION);
	}
	
	/*
	 * QM resource meta-information helper methods.
	 */
	
	public boolean isContainedInLoop() {
		return (getStringExtra(EXTRA_FUNCTION_CALL_IN_LOOP) != null) ;
	}
	
	public int getNumLoopIterations() {
		int result;
		try {
			double startVal = Integer.parseInt( extras.get(EXTRA_LOOP_START_VALUE) );
			double stopVal = Integer.parseInt( extras.get(EXTRA_LOOP_END_VALUE) );
			double updateVal = Integer.parseInt( extras.get(EXTRA_LOOP_UPDATE_VALUE) );
			result = (int) ((stopVal-startVal)/updateVal);
		} catch (NumberFormatException nfe) {
			// Parsing at least one of the loop values above failed -- this resource is 
			// likely not contained within a for-loop OR the loop is conditioned on a 
			// variable that coudn't be resolved.
			//
			// --> Returning 1 is equivalent to not being contained within a for-loop. 
			result = 1;
		}
		return result;
	}
}
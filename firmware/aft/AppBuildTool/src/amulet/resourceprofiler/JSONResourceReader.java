/*
 *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

package amulet.resourceprofiler;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.LinkedHashMap;
import java.util.Map.Entry;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

public class JSONResourceReader {
	public boolean debugEnabled = false;

	public String filename;
	
	// Containers for information parsed from JSON file.
	private DeviceInfo deviceInformation;
	private SteadyStateInfo steadyStateInformation;
	private JSONArray apiInfo;
	private LinkedHashMap<String, EnergyParam> energyParameters;
	
	public JSONResourceReader() {
		deviceInformation = new DeviceInfo();
		steadyStateInformation = new SteadyStateInfo();
		apiInfo = new JSONArray();
		energyParameters = new LinkedHashMap<String, EnergyParam>();
	}
	
	public String toString() {
		String str = "\nJSON Resource Reader: reading '" + filename + "' with energy cost parameters...\n\n";
		str += deviceInformation + "\n\n";
		str += steadyStateInformation + "\n\n";
		str += "API Energy Parameters::\n";
		for (Entry<String, EnergyParam> key : getEnergyParams().entrySet()) {
			str += " + " + key.getValue() + "\n";
		}
		return str;
	}
	
	/**
	 * Parse the input JSON file for energy data collected during our 
	 * evaluation of the amulet hardware. JSON data content will be 
	 * loaded into Plain Old Java Objects (POJOs) for easy handling 
	 * in later parts of the AFT. 
	 * 
	 * @param filename The JSON file with energy data. 
	 */
	public void read(String filename) {
		this.filename = filename;
		
		JSONParser parser = new JSONParser();
		try {
			// Top-level JSON object.
			JSONObject jsonRootObj = (JSONObject) parser.parse(new FileReader(this.filename));
//			System.out.println("DEBUG:: jsonRootObj="+jsonRootObj);
			
			// Extract Device Information.
			JSONObject jsonDeviceInfo = (JSONObject) jsonRootObj.get("device_information");

			deviceInformation.jsonDeviceInfo = jsonDeviceInfo;
			deviceInformation.deviceName = (String) jsonDeviceInfo.get("device_name");
			deviceInformation.batterySize = (Long) jsonDeviceInfo.get("battery_size");
			deviceInformation.batterySizeUnits = (String) jsonDeviceInfo.get("battery_size_units");
			deviceInformation.memTypeVolatile = (String) jsonDeviceInfo.get("volatile_mem_type");
			deviceInformation.memSizeVolatile = (Long) jsonDeviceInfo.get("volatile_mem_size_bytes");
			deviceInformation.memTypeNonVolatile = (String) jsonDeviceInfo.get("non_volatile_mem_type");
			deviceInformation.memSizeNonVolatile = (Long) jsonDeviceInfo.get("non_volatile_mem_size_bytes");
			deviceInformation.memTypeSecondary = (String) jsonDeviceInfo.get("secondary_storage_type");
			deviceInformation.memSizeSecondary = (Double) jsonDeviceInfo.get("secondary_storage_size_bytes");
			deviceInformation.avgNumInstructionsPerLoC = (Double) jsonDeviceInfo.get("instructions_per_loc");
			deviceInformation.avgBasicInstructionPower = (Double) jsonDeviceInfo.get("basic_instruction_power");
			deviceInformation.avgBasicInstructionTime = (Double) jsonDeviceInfo.get("basic_instruction_time");
			deviceInformation.avgBasicInstructionUnit = (String) jsonDeviceInfo.get("basic_loc_time_unit");
			
			// Extract Steady-State Information.
			JSONObject jsonSteadyStateInfo = (JSONObject) jsonRootObj.get("steady_state_info");

			steadyStateInformation.jsonSteadyStateInfo = jsonSteadyStateInfo;
			steadyStateInformation.radioBoardSleepPower = (Double) jsonSteadyStateInfo.get("radio_board_sleep_power");
			steadyStateInformation.appBoardSleepPower = (Double) jsonSteadyStateInfo.get("app_board_sleep_power");
			steadyStateInformation.displayIdlePower = (Double) jsonSteadyStateInfo.get("display_idle_power");
			steadyStateInformation.sensorAccelerometer = (Double) jsonSteadyStateInfo.get("sensor_accelerometer");
			steadyStateInformation.sensorHeartRate = (Double) jsonSteadyStateInfo.get("sensor_heartrate");

			// Array of "parameters".
			JSONArray paramArray = (JSONArray) jsonRootObj.get("api_calls");
			apiInfo = paramArray;
			for(int i = 0; i < paramArray.size(); i++) {
				JSONObject jsonObj = (JSONObject) paramArray.get(i);
				
				/*
				 * Build POJO based on energy parameter element from JSON data file.
				 * 
				 * NOTE: Extend this section of code to parse more parameters 
				 * as needed. Also, see: `JSONEnergyParam` below.
				 */
				EnergyParam param = new EnergyParam();
				param.name = jsonObj.get("resource_name").toString();
				param.type = jsonObj.get("resource_type").toString();
				param.avgPower = (Double) jsonObj.get("avg_power");
				param.avgTime = (Double) jsonObj.get("time");
				param.timeUnit = jsonObj.get("time_unit").toString();
				
				// Add the param to the collection of params read in. 
				addEnergyParam(param);
			}
			
			// DEBUG OUTPUT BLOCK /////////////////////////////////////
			if (debugEnabled) {
				System.out.println( this.toString() );
			}
			///////////////////////////////////////////////////////////

		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ParseException e) {
			e.printStackTrace();
		}
	}
	
	/* ************************************************************ *
	 * For details on the format of the "device profile" JSON file, *
	 * see: device_profile_amuletprotov1.0.json                     *
	 * ************************************************************ */
	
	/* ****************  Device Information POJO  ***************** */
	
	public DeviceInfo getDeviceInfo() {
		return deviceInformation;
	}
	
	public class DeviceInfo {
		public JSONObject jsonDeviceInfo;		
		public String deviceName;
		public Long batterySize;
		public String batterySizeUnits;
		public String memTypeVolatile;
		public Long memSizeVolatile;
		public String memTypeNonVolatile;
		public Long memSizeNonVolatile;
		public String memTypeSecondary;
		public Double memSizeSecondary;
		public Double avgNumInstructionsPerLoC;
		public Double avgBasicInstructionPower;
		public Double avgBasicInstructionTime;
		public String avgBasicInstructionUnit;
//		public Double basicBlockPower;
//		public Double basicBlockTime;
//		public String basicBlockUnit;		

		public DeviceInfo() { }
				
		public String toString() {
			return "DeviceInfo::" 
					+ "\n + deviceName=" + deviceName 
					+ ",\n + batterySize=" + batterySize 
					+ ",\n + batterySizeUnits=" + batterySizeUnits
					+ ",\n + memTypeVolatile=" + memTypeVolatile
					+ ",\n + memSizeVolatile=" + memSizeVolatile
					+ ",\n + memTypeNonVolatile=" + memTypeNonVolatile
					+ ",\n + memSizeNonVolatile=" + memSizeNonVolatile
					+ ",\n + memTypeSecondary=" + memTypeSecondary
					+ ",\n + memSizeSecondary=" + memSizeSecondary
					+ ",\n + avgNumInstructionsPerLoC=" + avgNumInstructionsPerLoC
					+ ",\n + avgBasicInstructionPower=" + avgBasicInstructionPower
					+ ",\n + avgBasicInstructionTime=" + avgBasicInstructionTime
					+ ",\n + avgBasicInstructionUnit=" + avgBasicInstructionUnit;
		}
	}
	
	/* *************  Steady State Information POJO  ************** */

	public SteadyStateInfo getSteadyStateInfo() {
		return steadyStateInformation;
	}
	
	public class SteadyStateInfo {
		public JSONObject jsonSteadyStateInfo;
		public Double radioBoardSleepPower;
		public Double appBoardSleepPower;
		public Double displayIdlePower;
		public Double sensorAccelerometer;
		public Double sensorHeartRate;

		public SteadyStateInfo() { }
		
		public String toString() {
			return "SteadyStateInfo::"
					+ "\n + radioBoardSleepPower=" + radioBoardSleepPower 
					+ ",\n + appBoardSleepPower=" + appBoardSleepPower 
					+ ",\n + displayIdlePower=" + displayIdlePower 
					+ ",\n + sensorAccelerometer=" + sensorAccelerometer 
					+ ",\n + sensorHeartRate=" + sensorHeartRate;
		}
	}	
	
	/* *******************  Energy Param POJO  ******************** */

	public JSONArray getApiInfo() {
		return apiInfo;
	}
	
	public LinkedHashMap<String, EnergyParam> getEnergyParams() {
		return energyParameters;
	}
	
	public void addEnergyParam(EnergyParam param) {
		energyParameters.put(param.name, param);
	}

	public class EnergyParam {
		public String name;
		public String type;
		public Double avgPower;
		public Double avgTime;
		public String timeUnit;

		public EnergyParam() { }

		public String toString() {
			return "EnergyParam:: name=" + name + ", type=" + type + ", avgPower=" + avgPower + ", time=" + avgTime+ ", timeUnit=" + timeUnit;
		}
	}
	
	/* **************************  MAIN  ************************** */
	
	public static void main(String[] args) {
		String testFilename = "./src/amulet/resourceprofiler/device_profile_amuletprotov1.0.json";
		
		JSONResourceReader reader = new JSONResourceReader();
		reader.debugEnabled = true;
		reader.read(testFilename);
	}
	
}

// TODOs:
//   - In the model notation below, verify notation consistency between paper, notes, and code below.

BATTERY_VOLTAGE = 3.7

// The # of seconds in 1 week. 
SECONDS_IN_WEEK = 60 * 60 * 24 * 7;

// Events per minute
SLIDER_MULTIPLIER_EVENTS_PER_MINUTE = 60 * 24 * 7;

// Events per hour
SLIDER_MULTIPLIER_EVENTS_PER_HOUR = 24 * 7;

// Events per days
SLIDER_MULTIPLIER_EVENTS_PER_DAY = 7;

/* ************************************************************************** *
 *                                Main Functions.                             *
 * ************************************************************************** */

/*
 * (Re)compute the energy impact for both the single- and multi-app models. 
 *
 * NOTE: It was getting a little ridiculous to have the same computation and UI 
 * update code across multiple files so now we have this nice wrapper that does 
 * the work of updating the models (single- and multi-app) as well as update the 
 * ARP view with the results.
 *
 * Ex. 
 * 
 *    compute_energy_impact_models_and_update_ui(
 *        firmware_resource_profile, 
 *        current_application_ndx, 
 *        parseInt($('#battery_size').val(), 10));
 */
function compute_energy_impact_models_and_update_ui(firmware_resource_profile, current_app_ndx, battery_size) {
	// Recompute the change to the app energy cost models. 
	var single_app_energy_cost = compute_energy_impact_single_app(
		firmware_resource_profile.applications[current_app_ndx], 
		current_app_ndx,
		battery_size); 

	var single_app_lifetime_days = compute_lifetime_in_days(single_app_energy_cost);

	// var multi_app_energy_cost = 0;
	var multi_app_energy_cost = compute_energy_impact_multi_app(
		firmware_resource_profile,
		battery_size);

	var multi_app_lifetime_days = compute_lifetime_in_days(multi_app_energy_cost); 

	// Update the lifetime UI (calls functions in visual.js)
	update_lifetime_estimate_single_app(single_app_energy_cost, single_app_lifetime_days);
	update_lifetime_estimate_multi_app(multi_app_energy_cost, multi_app_lifetime_days);
}

// Takes a battery percentage and gets lifetime in days
function compute_lifetime_in_days(energy_cost) {
	return (1 / (energy_cost / 100) ) * 7;
}

/*
 * NEW VERSION OF OLD "compute_energy_impact" FUNCTION.
 *
 * Given information about application energy costs and the size of a battery, 
 * calculate the percentage "impact" of a single application.
 */
function compute_energy_impact_single_app(resource_profile, current_app_ndx, battery_size_mah) {
	// Convert mAh to joules
	var battery_energy_joules = (battery_size_mah / 1000) * 3600 * BATTERY_VOLTAGE;
	var energy_cost_per_unit_time = compute_energy_cost_per_unit_time_single_app(resource_profile, current_app_ndx);
	var percentage = ((energy_cost_per_unit_time) / battery_energy_joules) * 100;
	return percentage;
}

/* 
 * ***NEW***
 *
 * Given information about application energy costs and the size of a battery, 
 * calculate the percentage "impact" of multiple applications.
 */
function compute_energy_impact_multi_app(firmware_resource_profile, battery_size_mah) {
	// Convert mAh to joules
	var battery_energy_joules = (battery_size_mah / 1000) * 3600 * BATTERY_VOLTAGE;
	var energy_cost_per_unit_time = compute_energy_cost_per_unit_time_multi_app(firmware_resource_profile);
	var percentage = ((energy_cost_per_unit_time) / battery_energy_joules) * 100;
	return percentage;
}

/* ************************************************************************** *
 *                               Helper Functions.                            *
 * ************************************************************************** */

/*
 * Linear Model Calculation (Single Applications).
 */
function compute_energy_cost_per_unit_time_single_app(resource_profile, current_app_ndx) {
	// Setup: get this application's slider timescale.
	var timescale = get_app_sliders_timescale_cache(current_app_ndx);
	
	// Setup: extract device & energy cost information from resource profile.
	var device_info = resource_profile.device_info;
	var steady_state_info = resource_profile.steady_state_info;
	
	var transition_energy_costs = resource_profile.transition_energy_costs;
	var transition_execution_times = resource_profile.transition_execution_times;
	
	var state_energy_costs = resource_profile.state_energy_costs;
	var state_execution_times = resource_profile.state_execution_times;

	// Set baseline costs.
	// Get E0, the steady energy state cost PER WEEK, P * t = E
	var b0 = steady_state_info.radio_board_sleep_power * SECONDS_IN_WEEK; // radio board
	var b1 = steady_state_info.app_board_sleep_power * SECONDS_IN_WEEK;   // application board (includes steady_state_info.display_idle_power)
	var E0 = b0 + b1;

	// Compute sensor subscription costs energy PER WEEK.
	var sumEs = 0;
	sumEs = _.reduce(_.toArray(resource_profile.app_sensor_subscriptions), function(memo, sensor_subscription_cost){ 
		// Compute Es term: cost for sensor subscription s in terms of energy per week.
		var Es = (sensor_subscription_cost || 0) * SECONDS_IN_WEEK;
		return memo + Es;
	}, 0);
	
	/*
	 * Compute state->transition->state (sts) energy costs over all transitions & how often
	 * they occur (i.e., number of times it executes).
	 *
	 * See: Eq 3. [Amulet: A Development Framework for Energy-Efficient Multi-Application Wearables]
	 */
	var sumRtEt = 0;
	sumRtEt = _.reduce(_.toArray(slider_values), function(memo, slider){ 
		/*
		 * Compute RtEt term: number of times this transition->state is 
		 *   executed * energy cost of that code executing.
		 */
		var state_key = slider.target;
		var scost = state_energy_costs[state_key];           // note: the state's cost was totaled already in ResourceProfiler.java
		var stime = state_execution_times[state_key];

		// HACK: The lookup fails here because we have to adjust the "key" that 
		// is used in order to make the graph library draw the graph correctly. 
		// This hack is needed for TIMERS so we check if the key corresponds to 
		// a transitions from a timer event; if so, we modify the key for the 
		// lookup since it was modified earlier. 
		var transition_key = slider.source+"->"+slider.trigger+"->"+slider.target;
		if(transition_key.indexOf("_REQ_CHANGE") > -1) {
			transition_key = transition_key.replace(transition_key.split("->")[0], "TIMER SOURCE");
		}
		var tcost = transition_energy_costs[transition_key]; // note: the transition's cost was totaled already in ResourceProfiler.java
		var ttime = transition_execution_times[transition_key];

		var Rt = slider.value * timescale;  // TODO: restrict this to a reasonable value?
		var Et = tcost + scost;

        ////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////
		// console.log( "  Rt: " + Rt );

		// console.log( state_key + " | " + transition_key );
		// console.log( "  scost: " + scost );
		// console.log( "  stime: " + stime );
		// console.log( "  tcost: " + tcost );
		// console.log( "  ttime: " + ttime );
		// console.log( "  Et: " + Et ); //<--- ?!?!?!?!?! this is the troublemaker... ?!?!?!?!?!
        ////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////

		return memo + Rt*Et;
	}, 0);

	// Eq. 3
	Ea = E0 + sumEs + sumRtEt;
	console.log("Single-App Model: Ea=" + Ea + " (E0=" + E0 + " sumEs=" + sumEs + " sumRtEt=" + sumRtEt + "), Avg Power=" + (Ea/SECONDS_IN_WEEK)); // Debug
	return Ea;
}

/*
 * Linear Model Calculation (Multiple Applications).
 */
function compute_energy_cost_per_unit_time_multi_app(firmware_resource_profile) {
	
	// Setup: high-level device information from the firmware resource profile.
	var device_info = firmware_resource_profile.device_info;
	var steady_state_info = firmware_resource_profile.steady_state_info;
	
	// 1. Set baseline costs.
	// Get E0, the steady energy state cost PER WEEK, P * t = E
	var b0 = steady_state_info.radio_board_sleep_power * SECONDS_IN_WEEK; // radio board
	var b1 = steady_state_info.app_board_sleep_power * SECONDS_IN_WEEK;   // application board (includes steady_state_info.display_idle_power)
	var E0 = b0 + b1;

	// 2. For each application "a" in the collection of applications, "A"...
	//      - Compute sensor subscription costs energy PER WEEK.
	//      - Sum them up!
	var sumAEs = 0;
    for (var i = 0; i < firmware_resource_profile.applications.length; i++) {
    	// Select application "a"
    	var a = firmware_resource_profile.applications[i]; 

    	// Sum the cost of a's sensor subscriptions and add the result to running sum over all applications.
		sumAEs += _.reduce(_.toArray( a.app_sensor_subscriptions ), function(memo, sensor_subscription_cost){ 
			// Compute Es term: cost for sensor subscription s in terms of energy per week.
			var Es = (sensor_subscription_cost || 0) * SECONDS_IN_WEEK;
			return memo + Es;
		}, 0);
    }

	// Compute state->transition->state (sts) energy costs over all apps, all of 
	// their respective transitions, and how often they occur (i.e., number of 
	// times it executes).
	//
	// See: Eq 4. [Amulet: A Development Framework for Energy-Efficient Multi-Application Wearables]
	//
	// 3. For each application "a" in the collection of applications, "A"...
	//      - get the transition slider data from the cache
	//      - compute similar to equation 3 in the single app model
	var sumARtEt = 0;

    for (var i = 0; i < firmware_resource_profile.applications.length; i++) {
    	// Select application "a"
    	var a = firmware_resource_profile.applications[i]; 

		// Get this application's slider timescale.
		var timescale = get_app_sliders_timescale_cache(i);

    	// Get application a's metadata from the resource profiler.
		var transition_energy_costs = a.transition_energy_costs;
		var transition_execution_times = a.transition_execution_times;
		var state_energy_costs = a.state_energy_costs;
		var state_execution_times = a.state_execution_times;

    	// Select application a's slider values from the cache. 
    	var app_a_slider_values = get_app_slider_values_from_cache(i);

		sumARtEt += _.reduce(_.toArray(app_a_slider_values), function(memo, slider){ 
			/*
			 * Compute RtEt term: number of times this transition->state is 
			 *   executed * energy cost of that code executing.
			 */
			var state_key = slider.target;
			var scost = state_energy_costs[state_key];           // note: the state's cost was totaled already in ResourceProfiler.java
			var stime = state_execution_times[state_key];

			// HACK: The lookup fails here because we have to adjust the "key" that 
			// is used in order to make the graph library draw the graph correctly. 
			// This hack is needed for TIMERS so we check if the key corresponds to 
			// a transitions from a timer event; if so, we modify the key for the 
			// lookup since it was modified earlier. 
			var transition_key = slider.source+"->"+slider.trigger+"->"+slider.target;
			if(transition_key.indexOf("_REQ_CHANGE") > -1) {
				transition_key = transition_key.replace(transition_key.split("->")[0], "TIMER SOURCE");
			}
			var tcost = transition_energy_costs[transition_key]; // note: the transition's cost was totaled already in ResourceProfiler.java
			var ttime = transition_execution_times[transition_key];

			var Rt = slider.value * timescale;  // TODO: restrict this to a reasonable value?
			var Et = tcost + scost;

			return memo + Rt*Et;
		}, 0);
    }

  	// Eq. 4
	EA = E0 + sumAEs + sumARtEt;
	console.log("Multi-App Model: EA=" + EA + " (E0=" + E0 + " sumAEs=" + sumAEs + " sumARtEt=" + sumARtEt + "), Avg Power=" + (EA/SECONDS_IN_WEEK)); // Debug
	return EA;
}

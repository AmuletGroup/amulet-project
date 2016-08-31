// Fix for new version of Chrome, I dont understand why this has to be here - JDH
SVGElement.prototype.getTransformToElement = SVGElement.prototype.getTransformToElement || function(elem) {
    return elem.getScreenCTM().inverse().multiply(this.getScreenCTM());
};

function adjustVertices(graph, cell) {

    // If the cell is a view, find its model.
    cell = cell.model || cell;

    if (cell instanceof joint.dia.Element) {

        _.chain(graph.getConnectedLinks(cell)).groupBy(function(link) {
            // the key of the group is the model id of the link's source or target, but not our cell id.
            return _.omit([link.get('source').id, link.get('target').id], cell.id)[0];
        }).each(function(group, key) {
            // If the member of the group has both source and target model adjust vertices.
            if (key !== 'undefined') adjustVertices(graph, _.first(group));
        });

        return;
    }

    // The cell is a link. Let's find its source and target models.
    var srcId = cell.get('source').id || cell.previous('source').id;
    var trgId = cell.get('target').id || cell.previous('target').id;

    // If one of the ends is not a model, the link has no siblings.
    if (!srcId || !trgId) return;

    var siblings = _.filter(graph.getLinks(), function(sibling) {

        var _srcId = sibling.get('source').id;
        var _trgId = sibling.get('target').id;

        return (_srcId === srcId && _trgId === trgId) || (_srcId === trgId && _trgId === srcId);
    });

    switch (siblings.length) {

    case 0:
        // The link was removed and had no siblings.
        break;

    case 1:
        // There is only one link between the source and target. No vertices needed.
        cell.unset('vertices');
        break;

    default:

        // There is more than one siblings. We need to create vertices.

        // First of all we'll find the middle point of the link.
        var srcCenter = graph.getCell(srcId).getBBox().center();
        var trgCenter = graph.getCell(trgId).getBBox().center();
        var midPoint = g.line(srcCenter, trgCenter).midpoint();

        // Then find the angle it forms.
        var theta = srcCenter.theta(trgCenter);

        // This is the maximum distance between links
        var gap = 80;

        _.each(siblings, function(sibling, index) {

            // We want the offset values to be calculated as follows 0, 20, 20, 40, 40, 60, 60 ..
            var offset = gap * Math.ceil(index / 2);

            // Now we need the vertices to be placed at points which are 'offset' pixels distant
            // from the first link and forms a perpendicular angle to it. And as index goes up
            // alternate left and right.
            //
            //  ^  odd indexes 
            //  |
            //  |---->  index 0 line (straight line between a source center and a target center.
            //  |
            //  v  even indexes
            var sign = index % 2 ? 1 : -1;
            var angle = g.toRad(theta + sign * 90);

            // We found the vertex.
            var vertex = g.point.fromPolar(offset, angle, midPoint);

            sibling.set('vertices', [{ x: vertex.x, y: vertex.y }]);
        });
    }
}

/** Stupid hack for materialize to get the sliders to work right */
function update_sliders() {
  var range_type = 'input[type=range]';
  var range_mousedown = false;
  var left;

  $(range_type).each(function () {
    var thumb = $('<span class="thumb"><span class="value"></span></span>');
    $(this).after(thumb);
  });

  var range_wrapper = '.range-field';
  $(document).on('change', range_type, function(e) {
    var thumb = $(this).siblings('.thumb');
    thumb.find('.value').html($(this).val());
  });

  $(document).on('input mousedown touchstart', range_type, function(e) {
    var thumb = $(this).siblings('.thumb');
    var width = $(this).outerWidth();

    // If thumb indicator does not exist yet, create it
    if (thumb.length <= 0) {
      thumb = $('<span class="thumb"><span class="value"></span></span>');
      $(this).after(thumb);
    }

    // Set indicator value
    thumb.find('.value').html($(this).val());

    range_mousedown = true;
    $(this).addClass('active');

    if (!thumb.hasClass('active')) {
      thumb.velocity({ height: "30px", width: "30px", top: "-20px", marginLeft: "-15px"}, { duration: 300, easing: 'easeOutExpo' });
    }

    if (e.type !== 'input') {
      if(e.pageX === undefined || e.pageX === null){//mobile
         left = e.originalEvent.touches[0].pageX - $(this).offset().left;
      }
      else{ // desktop
         left = e.pageX - $(this).offset().left;
      }
      if (left < 0) {
        left = 0;
      }
      else if (left > width) {
        left = width;
      }
      thumb.addClass('active').css('left', left);
    }

    thumb.find('.value').html($(this).val());
  });

  $(document).on('mouseup touchend', range_wrapper, function() {
    range_mousedown = false;
    $(this).removeClass('active');
  });

  $(document).on('mousemove touchmove', range_wrapper, function(e) {
    var thumb = $(this).children('.thumb');
    var left;
    if (range_mousedown) {
      if (!thumb.hasClass('active')) {
        thumb.velocity({ height: '30px', width: '30px', top: '-20px', marginLeft: '-15px'}, { duration: 300, easing: 'easeOutExpo' });
      }
      if (e.pageX === undefined || e.pageX === null) { //mobile
        left = e.originalEvent.touches[0].pageX - $(this).offset().left;
      }
      else{ // desktop
        left = e.pageX - $(this).offset().left;
      }
      var width = $(this).outerWidth();

      if (left < 0) {
        left = 0;
      }
      else if (left > width) {
        left = width;
      }
      thumb.addClass('active').css('left', left);
      thumb.find('.value').html(thumb.siblings(range_type).val());
    }
  });

  $(document).on('mouseout touchleave', range_wrapper, function() {
    if (!range_mousedown) {

      var thumb = $(this).children('.thumb');

      if (thumb.hasClass('active')) {
        thumb.velocity({ height: '0', width: '0', top: '10px', marginLeft: '-6px'}, { duration: 100 });
      }
      thumb.removeClass('active');
    }
  });
}

function create_state(graph, x, y, label, heat) {
    var cell = new joint.shapes.fsa.State({
        position: { x: x, y: y },
        size: { width: 120, height: 75 },
        attrs: {
            text : { text: label, fill: '#000000', 'font-weight': 'normal', 'font-size': '15px'},
            'circle': {
                fill: '#f6f6f6',
                stroke: rgb(0.0,10.0, heat),
                'stroke-width': 2
            }
        }
    });
    graph.addCell(cell);
    return cell;
}

/*
 * In order to clean up the UI a little bit we like to map some of the ugly names 
 * that are used internally (while programming) to slightly more helpful/meaningful 
 * names for developers. 
 *
 * We implement slight variants of the same function for different contexts...
 *   - translate_label_name()
 *   - translate_slider_label_name()
 */
function translate_label_name(labelname) {
    var oldlabelname = labelname;

    if(labelname.indexOf("_REQ_CHANGE") > -1) {
        labelname = platform_name_for_labels+"TimerEvent";
    }
    if(labelname.indexOf("_DEVICE_DATA") > -1) {
        labelname = platform_name_for_labels+"DataGenerated";
    }
    labelname = labelname.replace("AMULET", platform_name_for_labels);

    // console.log( "DEBUG:: link label:[" + oldlabelname + "::" + labelname + "]" );
    return labelname;
}

function translate_slider_label_name(labelname) {
    var oldlabelname = labelname;

    if(labelname.indexOf("_REQ_CHANGE") > -1) {
        labelname = labelname.replace("_REQ_CHANGE", "TimerEvent");
    }
    if(labelname.indexOf("_DEVICE_DATA") > -1) {
        labelname = labelname.replace("_DEVICE_DATA", "DataGenerated");
    }
    labelname = labelname.replace("AMULET", platform_name_for_labels);

    // console.log( "DEBUG:: slider label:[" + oldlabelname + "::" + labelname + "]" );
    return labelname;  
}

/*
 * Given an energy cost in mJ, format the cost with the units and return so that 
 * it can be displayed on a cost label.
 *
 * We are also experimenting with formatting in microjoules.
 */
function format_energy_cost(cost) {
    // millijoules
    // labelCost = labelCost+" mJ";

    // microjoules
    var mchar = '\\u00B5J';
    labelCost = (cost*1000).toFixed()+" "+eval("'"+mchar+"'");  
    return labelCost;
}

/*
 * A wrapper for actually creating a link (arrow) in the visualized graph (i.e., 
 * the application's state machine). 
 */
function create_link(graph, source, target, labelLinkName, labelExecRate, labelCost, vertices) {
    labelLinkName = translate_label_name(labelLinkName);
    labelCost = format_energy_cost(labelCost);

    //DEBUG
    console.log("CREATE LINK="+labelLinkName+"="+labelCost);

    var cell = new joint.shapes.fsa.Arrow({
        __internal_link_id : labelLinkName,
        source: { id: source.id },
        target: { id: target.id },
        labels: [{ position: 0.5, attrs: { text: { text: labelLinkName || '', 'font-weight': 'bold', 'font-size': '15px' } } },
                 { position: 0.3, attrs: { text: { text: labelExecRate || '', 'font-weight': 'bold', 'font-size': '12px' , fill: 'blue'} } },
                 { position: 0.7, attrs: { text: { text: labelCost || '', 'font-weight': 'bold', 'font-size': '12px' , fill: 'blue'} } }],
        vertices: vertices || []
    });

    graph.addCell(cell);
    return cell;
}

////////////////////////////////////////////////////////////////////////////////
//    The Main Function in Visual.js Exists to Switch to a Newly Selected     //
//       Application and Re-render the State Diagram, Sliders, etc.           //
////////////////////////////////////////////////////////////////////////////////

/*
 * NOTE: The "structure" and "firmware_resource_profile" variables are global 
 * variables that are visible to this function and are defined in the 
 * "graph_structure.js" and "costs_gen.js" files, respectively. 
 */
function change_application_and_render_fsm(paper, graph, application_idx, all_mem_stats_compiled) {

  // Initialize specific information about an application (i.e., application "application_idx").
  var application = structure.applications[application_idx];
  var resource_profile = firmware_resource_profile.applications[application_idx];
  var mem_stats_compiled = all_mem_stats_compiled.applications[application_idx];

  // Initialize firmware stats information.
  var total_binary_size = all_mem_stats_compiled.total_binary_size; 
  var total_local_cost = all_mem_stats_compiled.total_local_cost; 
  var max_ram_usage = 0;

  // Restore the slider values that were cached.
  slider_values = get_app_slider_values_from_cache(application_idx) || {};

  // Clear the existing UI for the sliders -- we will restore those below...
  $('#event_sliders').empty();

  // Now update the UI with sliders and the app name
  $('#appname').html('Application: <em>'+application.appname+"</em>");

  // Clear the FSM graph
  graph.clear();

  // Add initial state
  var start = new joint.shapes.fsa.StartState({});
  graph.addCell(start);

  // Add nodes to make custom renderer work
  var states = _.uniq(_.pluck(application.states, "source").concat(_.pluck(application.states, "target")));
  //states = _.reject(states, function(val){ return val == "NONE"; }); //remove dummy state for initial transition.

  var state_objs = {};
  var sram_costs = resource_profile.state_sram_costs;
  var energy_costs = resource_profile.state_energy_costs;
  for (var i = states.length - 1; i >= 0; i--) {
      if(states[i] == "TIMER SOURCE") {
        if(resource_profile.app_timer_subscriptions.Timer) {
          var timer_node = new joint.shapes.fsa.StartState({});
          graph.addCell(timer_node);
          state_objs["TIMER SOURCE"] = timer_node;
        }
        continue;
      }
      // ONLY NEED THIS IF RATES FOR BLE board are changeable
    /*  if(states[i] == "DATA SOURCE") {
        if(resource_profile.app_sensor_subscriptions.ACCELEROMETER) {
          var data_node = new joint.shapes.fsa.StartState({});
          graph.addCell(data_node);
          state_objs["DATA SOURCE"] = data_node;
        }
        if(resource_profile.app_sensor_subscriptions.HEARTRATE) {
          var data_node = new joint.shapes.fsa.StartState({});
          graph.addCell(data_node);
          state_objs["DATA SOURCE"] = data_node;
        }
        continue;
      } */
      if (states[i] != "NONE") {
        var state_energy = energy_costs[states[i]] || 0;
        var state_sram = sram_costs[states[i]] || 0;
        state_objs[states[i]] = create_state(graph, 0, 0, states[i]);//+
                    //'\n('+(state_energy * 1000).toFixed(1)+'mJ)', // Energy costs (Joules)
                    //state_energy); // Heat
                    //'\n('+state_sram+'b)'+  // SRAM bytes
        if (state_sram > max_ram_usage) max_ram_usage = state_sram;
      }
  }
  
  /*
   * Render the application state machine (states & transitions).
   */

  // Initial edge
  var initState = _.find(application.states, function(state){ return state.source === "NONE"; });  
  // create_link(graph, start, state_objs[initState.target],  'Initial', '1x', '0.0 mJ');
  create_link(graph, start, state_objs[initState.target],  'Initial', '1x', 0.0);

  // Add all edges / transitions except loop backs, which are timer events, to the UI (graph and slider panel).
  for (var i = application.states.length - 1; i >= 0; i--) {
      var state = application.states[i];
      if(state.source !== state.target && state.source !== "NONE") {
          // Set identifier information about transition (a.k.a "link").
          var slider_id = state.source + "_" + state.trigger + "_" + state.target;
          var slider_name_val = state.source + "->" + state.trigger + "->" + state.target;

          // Current set value for the slider.
          var curr_rate_val = slider_values[slider_id].value;

          // Create a link in the graph between state.source & state.target with the currently set slider value.
          var initialEnergyCost = get_transition_energy_cost(application_idx, resource_profile, state.source, state.trigger, state.target, curr_rate_val);
          // var link_made = create_link(graph, state_objs[state.source], state_objs[state.target], state.trigger, curr_rate_val+"x", initialEnergyCost.toFixed(1)+" mJ");
          var link_made = create_link(graph, state_objs[state.source], state_objs[state.target], state.trigger, curr_rate_val+"x", initialEnergyCost);
          
          // Update UI for the transition frequency slider.
          labelsliderName = translate_slider_label_name(slider_name_val);
          $('#event_sliders').append('<p class="range-field"><label class="active" for="'+slider_id+'">'+labelsliderName+'</label><input type="range" id="'+slider_id+'" value="'+curr_rate_val+'" min="0" max="100" /></p>');

          // Update slider with a valid (or new) graph link ID. 
          slider_values[slider_id].graph_link_id = link_made.id;
      }
  }

  // Make the layout pretty
  joint.layout.DirectedGraph.layout(graph, { 
    setLinkVertices: false, 
    rankDir : "LR",
    marginX : 40,
    marginY : 40,
    nodeSep : 80,
    edgeSep : 80,
    rankSep : 80
  });

  graph.get('cells').each(function(cell) {
      adjustVertices(graph, cell);
  });

  update_sliders();
  paper.scaleContentToFit();

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  // NOTE: THIS WHOLE SECTION NEEDS TO BE UPDATED/REFACTORED...IT HAS GOTTEN A 
  // LITTLE RIDICULOUS...IT WORKS, BUT IT SURE AIN'T BEAUTIFUL.........

  /* 
   * Update the memory stats & progress bars.
   */
  
  // Device-specific constraints. 
  var appMemTotal = 128;       // (128KB) TODO: GET FROM DEVICE PROFILE  <-- these are the constraints of the amulet device. 
  var totalLocalMemory = 2048; // (2KB)   TODO: GET FROM DEVICE PROFILE  <-- these are the constraints of the amulet device. 

  // Compute total code size for -all- applications.
  var allAppCodeSize = 0;
  for (var i = 0; i < all_mem_stats_compiled.applications.length; i++) {
    allAppCodeSize += all_mem_stats_compiled.applications[i].total_code_size;
  }

  // Now, determine the "system" code size since we know the total code size of the apps. 
  var sysMemUsage = (total_binary_size-allAppCodeSize) / 1024.0;

  //
  // APPLICATION ONLY Memory Usage (FRAM)
  //
  var appMemUsage = mem_stats_compiled.total_code_size/1024.0;
  var appMemProgress = ( (sysMemUsage+appMemUsage)/appMemTotal )*100;

  // Update UI. 
  $('#app_mem_usage').html("Memory Use (FRAM)<br> <em>( "+ (sysMemUsage).toFixed(2) +" KB <sub>System</sub> + <b>"+(appMemUsage).toFixed(2)+"</b> KB <sub>"+application.appname+"</sub> ) / "+appMemTotal+" KB <sub>Total</sub></em>");
  $('#app_mem_usage_progress_width').attr('style', "width: "+(appMemProgress).toFixed(2)+"%");

  //
  // APPLICATION ONLY Local Memory Usage (SRAM)
  //
  var appLocalProgress = ( (total_local_cost+max_ram_usage)/totalLocalMemory )*100;

  // Update UI. 
  $('#app_local_usage').html("Max Ram Use (SRAM)<br> <em>( "+total_local_cost+" B <sub>System</sub> + <b>"+max_ram_usage+"</b> B <sub>"+application.appname+"</sub> ) / "+totalLocalMemory+" B <sub>Total</sub></em>");
  $('#app_local_usage_progress_width').attr('style', "width: "+(appLocalProgress).toFixed(2)+"%");

  //
  // SYSTEM-WIDE Memory Usage (FRAM): 
  //   --> Compute the total code size consumed by -all- apps + the system. 
  //

  // Compute total memory usage for -all- applications.
  var allAppMemUsage = allAppCodeSize/1024.0;

  // Now, compute how much space, of the "appMemTotal", is consumed by the applications and system. 
  var allAppMemProgress = ((sysMemUsage+allAppMemUsage)/appMemTotal)*100;

  // Update UI. 
  var allAppMemUsageStr = ""; 
  for (var i = 0; i < all_mem_stats_compiled.applications.length; i++) {
    var curr_app = structure.applications[i];
    var curr_app_mem_stats = all_mem_stats_compiled.applications[i];
    var curr_app_mem_usage = curr_app_mem_stats.total_code_size/1024.0;
    allAppMemUsageStr += " + " + (curr_app_mem_usage).toFixed(2) + " KB <sub>" + curr_app.appname + "</sub>";
  }
  $('#all_app_mem_usage').html("Memory Use (FRAM)<br> <em>( "+ (sysMemUsage).toFixed(2) +" KB <sub>System</sub>" + allAppMemUsageStr + " ) / "+appMemTotal+" KB <sub>Total</sub></em>");
  $('#all_app_mem_usage_progress_width').attr('style', "width: "+(allAppMemProgress).toFixed(2)+"%");

  //
  // SYSTEM-WIDE Local Memory Usage (SRAM): 
  //   --> Compute the max ram usage by any one of the applications. 
  //
  var allAppMaxRamUsage = 0;

  var appMaxRamUsageTable = [];

  // loop over each application...
  for (var i = 0; i < all_mem_stats_compiled.applications.length; i++) {
    var curr_app = structure.applications[i];
    var curr_app_resource_profile = firmware_resource_profile.applications[i];
    var curr_app_states = _.uniq(_.pluck(curr_app.states, "source").concat(_.pluck(curr_app.states, "target")));

    var curr_app_max_ram_usage = 0; 

    // loop over each state ("s") in this application ("curr_app")...
    var curr_app_sram_costs = curr_app_resource_profile.state_sram_costs;
    for (var s = curr_app_states.length - 1; s >= 0; s--) {
      if(curr_app_states[s] == "TIMER SOURCE") {
        continue;
      }
      if (curr_app_states[s] != "NONE") {
        var state_sram = curr_app_sram_costs[curr_app_states[s]] || 0;

        // "global" max update check.
        if (state_sram > allAppMaxRamUsage) {
          allAppMaxRamUsage = state_sram;
        }

        // "local" max update check.
        if (state_sram > curr_app_max_ram_usage) {
          curr_app_max_ram_usage = state_sram;
        }
      }
    }

    // Keep track of the current app's max ram usage (for each app). 
    appMaxRamUsageTable.push( curr_app_max_ram_usage ); 
  }

  // Now, compute how much space, of the "totalLocalMemory", is consumed by the applications and system. 
  var allAppLocalProgress = ( (total_local_cost+allAppMaxRamUsage)/totalLocalMemory )*100;

  // Update UI. 
  var allAppLocalProgressStr = "";
  for (var i = 0; i < all_mem_stats_compiled.applications.length; i++) {
    var curr_app = structure.applications[i];
    allAppLocalProgressStr += " + " + appMaxRamUsageTable[i] + " B <sub>" + curr_app.appname + "</sub>";
  }
  $('#all_app_local_usage').html("Max Ram Use (SRAM)<br> <em>( "+total_local_cost+" B <sub>System</sub>" + allAppLocalProgressStr + " ) / "+totalLocalMemory+" B <sub>Total</sub></em>");
  $('#all_app_local_usage_progress_width').attr('style', "width: "+(allAppLocalProgress).toFixed(2)+"%");

  ////////////////////////////////////////////////////////////////////////////
  // MEMORY STATS DEBUG //////////////////////////////////////////////////////
  // console.log("systemOnlyMemProgress="+(sysMemUsage/appMemTotal)*100);

  // console.log("appMemProgress="+appMemProgress);
  // console.log("appLocalProgress="+appLocalProgress);

  // console.log("allAppMemProgress="+allAppMemProgress);
  // console.log("allAppLocalProgress="+allAppLocalProgress);
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  /*
   * Update the battery impact models based on changes to slider inputs.
   */

  // This is fired on ANY input to the sliders
  $('input[type=range]').on('input', function (e) {
      var $range = $(this);
      var range_obj_id = $range.attr('id');
      slider_values[range_obj_id].value = parseInt($range.val(),10);
      
      // Set selected slider to be highlighted (if it isn't already).
      if(!slider_values[range_obj_id].is_highlighted) {
          highlight_link(graph, slider_values[range_obj_id].graph_link_id);    
          slider_values[range_obj_id].is_highlighted = true;
      }

      // SLIDER VALUE CACHE UPDATE //////////////////////////////////////
      update_slider_values_cache(current_application_ndx, slider_values);
      ///////////////////////////////////////////////////////////////////

      // Recompute the change to the app energy cost models. 
      compute_energy_impact_models_and_update_ui(
          firmware_resource_profile, 
          current_application_ndx, 
          parseInt($('#battery_size').val(), 10));
      // Debug
      // console.log("model update called [slider update]: current_application_ndx: "+current_application_ndx);

      // Update "rate" label to show new slider value.
      set_link_label_rate(graph, slider_values[range_obj_id].graph_link_id, slider_values[range_obj_id].value+"x");

      // Update "cost" label to show new energy cost with updated rate (i.e., slider) value.
      var energyCost = get_transition_energy_cost(current_application_ndx, resource_profile, slider_values[range_obj_id].source, slider_values[range_obj_id].trigger, slider_values[range_obj_id].target, slider_values[range_obj_id].value)
      set_link_label_cost(graph, slider_values[range_obj_id].graph_link_id, energyCost);
  });

  // Event slider / range change event: only fired on end
  $('input[type=range]').on('change', function (e) {
      // Update the slider value in the map / obj
      var $range = $(this);
      var range_obj_id = $range.attr('id');
      slider_values[range_obj_id].value = parseInt($range.val(),10);
      
      // Reset back to non-highlighted link
      unhighlight_link(graph, slider_values[range_obj_id].graph_link_id);
      slider_values[range_obj_id].is_highlighted = false;
  });

}

/*
 * Given attributes about a state transition, compute the energy cost.
 */
function get_transition_energy_cost(current_app_ndx, resource_profile, sourceStateName, transitionName, destinationStateName, rate) {
  // Construct lookup key in the format of the resource profile mappings.
  var key = sourceStateName + "->" + transitionName + "->" + destinationStateName; 

  // NOTE: "Keys" related to "Timer Events" and "Datat Generation Events" were modified 
  // directly in the energy cost look-up map so we need to modify the key we use 
  // to look up energy costs if we encounter these types of keys. 
  if(key.indexOf("_REQ_CHANGE") > -1) {
    key = key.replace(key.split("->")[0], "TIMER SOURCE");
  }

  // Compute cost.
  var timescale = get_app_sliders_timescale_cache(current_app_ndx);
  var dcost = resource_profile.state_energy_costs[destinationStateName];
  var tcost = resource_profile.transition_energy_costs[key];
  var cost = (timescale * rate) * (dcost + tcost);

  // Debug
  // console.log("timescale="+timescale);
  // console.log("dcost["+destinationStateName+"]="+dcost);
  // console.log("tcost["+key+"]="+tcost);
  // console.log("cost="+cost);
  // console.log("   ("+timescale+" * "+rate+") * ("+dcost+" + "+tcost+")");

  return cost;  
}

// Generic function for setting a label on a link in the graph (see helper functions below).
function set_link_label(graph, linkid, ndx, thelabel) {
  var linkcell = graph.getCell(linkid);
  linkcell.label(ndx, { attrs: { text: { text: thelabel } } });
}

// Helper function: update a link's NAME.
function set_link_label_name(graph, linkId, labelName) {
  set_link_label(graph, linkId, 0, labelName);
}
// Helper function: update a link's EXECUTION RATE.
function set_link_label_rate(graph, linkId, labelRate) {
  set_link_label(graph, linkId, 1, labelRate);
}
// Helper function: update a link's ENERGY COST.
function set_link_label_cost(graph, linkId, labelEnergyCost) {
  labelEnergyCost = format_energy_cost(labelEnergyCost);
  set_link_label(graph, linkId, 2, labelEnergyCost);
}

// Helper function: update the computed energy impact on battery percentage for only the selected application.
function update_lifetime_estimate_single_app(percentage_impact, days) {
  $('#lifetime_estimate_text_single_app').html("<b>"+percentage_impact.toFixed(2)+"% ("+Math.round(days)+" day lifetime)</b>");
}

// Helper function: update the computed energy impact on battery percentage for *ALL* applications compiled in an amulet firmware image.
function update_lifetime_estimate_multi_app(percentage_impact, days) {
  $('#lifetime_estimate_text_multi_app').html("<b>"+percentage_impact.toFixed(2)+"% ("+Math.round(days)+" day lifetime)</b>");
}

function highlight_link(graph, linkid) {
  var linkcell = graph.getCell(linkid);
  linkcell.attr({
    '.connection': { stroke: '#26A69A', 'stroke-width' :5 },
    '.marker-source': { fill: '#26A69A'},
    '.marker-target': { fill: '#26A69A'}
  });
  linkcell.label(0, { attrs: { text: { fill: '#26A69A' } } });
  linkcell.label(1, { attrs: { text: { fill: '#26A69A' } } });
  linkcell.label(2, { attrs: { text: { fill: '#26A69A' } } });
}

function unhighlight_link(graph, linkid) {
  var linkcell = graph.getCell(linkid);
  linkcell.attr({
    '.connection': { stroke: '#000000', 'stroke-width' :1 },
    '.marker-source': { fill: '#000000'},
    '.marker-target': { fill: '#000000'}
  });
  linkcell.label(0, { attrs: { text: { fill: 'black' } } });
  linkcell.label(1, { attrs: { text: { fill: 'blue' } } });
  linkcell.label(2, { attrs: { text: { fill: 'blue' } } });
}

function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function rgb(minimum, maximum, value) {
    var r = parseInt((value-minimum) / (maximum - minimum) * 255, 10)
//     return rgbToHex(r,0, 0);
    return rgbToHex(0,0,0);
}

/* ************************* * 
 * SLIDER CACHE HELPER CODE. *
 * ************************* */

// Predefined cache key(s).
TIMESCALE_KEY="AppSliderTimescale";

/*
 * Construct a legitimate "cache" for each app and its transition information 
 * (i.e., cost, slider value (frequency), human readable names, and so forth).
 */
function init_slider_values_cache() {
    for (var app_idx = 0; app_idx < structure.applications.length; app_idx++) {

        // Select the current application.
        var app = structure.applications[app_idx];

        // Store an initial setting for the transition attributes/costs/frequency in the slider values cache. 
        var app_slider_values_cache = {};
        // console.log( "appname: " + app.appname ); 

        // For each of the transitions (edges) add the costs. 
        for (var state_idx = app.states.length - 1; state_idx >= 0; state_idx--) {
            var state = app.states[state_idx];
            if(state.source !== state.target && state.source !== "NONE") {
                // Transition Defaults.
                var default_rate_val = 1;
                var default_cost_val = 0.0;

                if(state.timer_period) {
                    /****
                    *  CHANGE THIS IF WE DONT USE MINUTES FOR EVENT SLIDERS
                    *  CHANGE THIS IF WE DONT USE MINUTES FOR EVENT SLIDERS
                    *  CHANGE THIS IF WE DONT USE MINUTES FOR EVENT SLIDERS
                    */
                    default_rate_val = 60 / state.timer_period; // Seconds
                }

                // Slider value map update
                var slider_id = state.source + "_" + state.trigger + "_" + state.target;
                app_slider_values_cache[slider_id] = {
                    source         : state.source,     // source state
                    trigger        : state.trigger,    // trigger that moves from source state to target state
                    target         : state.target,     // target state
                    value          : default_rate_val, // rate (times/sec) that transition executions
                    cost           : default_cost_val, // energy cost of the transition
                    graph_link_id  : -1,
                    is_highlighted : false
                };
            }
        }

        // Update the cache. 
        slider_values_cache.push( app_slider_values_cache );
        slider_timescale_cache.push( SLIDER_MULTIPLIER_EVENTS_PER_MINUTE ); // default
    }
}

function get_app_slider_values_from_cache(app_ndx) {
    return slider_values_cache[app_ndx];
}

function update_slider_values_cache(app_ndx, curr_slider_values) {
    // console.log("[update app " + app_ndx + "'s slider values in cache]."); //debug
    slider_values_cache[app_ndx] = curr_slider_values;
}

function get_app_sliders_timescale_cache(app_ndx) {
    return slider_timescale_cache[app_ndx];
}

function update_app_sliders_timescale_cache(app_ndx, timescale) {
    slider_timescale_cache[app_ndx] = timescale;
}

function get_selected_index_by_timescale_value(timescale) {
    if (timescale === SLIDER_MULTIPLIER_EVENTS_PER_MINUTE) {
        return 0;
    } else if (timescale === SLIDER_MULTIPLIER_EVENTS_PER_HOUR) {
        return 1;
    } else if (timescale === SLIDER_MULTIPLIER_EVENTS_PER_DAY) {
        return 2;
    }
}

function get_timescale_text_from_val(timescale) {
    if (timescale === SLIDER_MULTIPLIER_EVENTS_PER_MINUTE) {
        return "events per minute";
    } else if (timescale === SLIDER_MULTIPLIER_EVENTS_PER_HOUR) {
        return "events per hour";
    } else if (timescale === SLIDER_MULTIPLIER_EVENTS_PER_DAY) {
        return "events per day";
    }
}

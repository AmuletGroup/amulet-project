# Amulet Firmware Tool (AFT)

The `aft` script, at a high-level, is responsible for generating a compact firmware image based on a simple configuration file that developers can easily customize. An example config file is given in `example_firmware.config` in this directory.

You can use the default config file provided (`example_firmware.config`) by running:

```
$ ./aft
```

Or, if you want to use a custom configuration file you can pass the path to your config file as the first argument when calling the `aft` script:

```
$ ./aft ~/projects/config-files/amulet-build.config
```

When the script completes successfully, generated code can be found in the `gen/` directory and the firmware is in the same directory, labeled as `msp430fr5969.out`.

# Basic Overview of How the AFT works

The basic flow of the script can be thought of as follows: 

1. Read the configuration file that defines desired build specifications. 
2. Clear out the `gen` directory before re-populating that directory with updated code.
3. Build the "AppBuildTool" (calls `AppBuildTool/compile.rb`).
4. (Re-)build all QM components in core (*NOTE: This step will soon be meaningless since QM-related code is currently being removed from the core*).
5. Create a firmware for the amulet (`AppBuildTool/build_firmware.rb`)
	- Check each app for compatibility with AmuletC, add runtime checks wherever needed, and translate code as needed for compilation.
	- Check if each app is authorized to make the API calls included in the app.
  	- Combine all QM applications into a single QM file and generate the `main.c`, `apps.c`, and `apps.h` files in the temporary `gen` directory (see: `AppMerge.java`).
  	- Copy all core (i.e., amulet-os) files to the temporary `gen` directory.
  	- Copy all port (i.e., msp430 specific) files to the temporary `gen` directory.
  	- Copy the Makefile from the `aft/AppBuildTool` directory to the temporary `gen` directory.
6. If a device is connected, the script can also flash the device with the firmware image that was generated above.

# Usage

	Usage: ./aft [options] file

	Convenience:

	        --all                        Executes a "traditional", more complete, flow of the AFT (i.e., purge, sync, merge/analyze, generate, compile, stats)
	        --arp-view-dev               (ARP-view Development) Run `aft --all` once, then run `aft` with this option to run only sync and merge/analyze steps
	        --clean                      Alias for only running the "purge" step to clean the gen/ directory.

	App Development (aftd control):

	    *** NOTE *** The aftd requires that you have the osx program "fswatch"; this tool only
	    exists on osx but we may support linux in the future. This should be OK since the primary
	    development environment we support currently is osx.

	    The aft daemon (a.k.a., "aftd") is a service that will watch for changes under the
	    `src/` directory in your amulet repository (e.g., applications, ARP-view code, amulet core
	    files). If changes are detected, those files will be synced with the `gen/` directory so that
	    that ARP-view can utilize those changes in near real-time to aid developers in developing
	    resource efficient applications.

	    It is recommended that you run `./aft --all` once, then run `./aft --aftd-{start,stop,status}.

	        --aftd-start [CONFIGFILE]    (App Development) Start the aftd that uses fswatch to sync any updated files in near real-time
	        --aftd-status                (APP Development) Check the status of the aftd (i.e., see if any aftd related processes are running)
	        --aftd-stop                  (App Development) Terminate the aftd related processes runnning in the background

	For more granular control of AFT execution:

	    -p, --purge                      Purge AFT-generated files from the AFT-sanctioned gen/ directory
	    -s, --sync                       Synchronize amulet files (e.g., Core, BSP, ARP-view) to the gen/ directory (i.e., simply copy them from the src/ to gen/)
	    -a, --rebuild-abt                Recompiles the Java AppBuildTool (NOTE: You should only need to do this once)
	    -v, --verbose                    Output more information about the execution of the AFT script
	    -m, --merge-and-arp              Merge all QM apps into a single QM file and invoke the Amulet Resource Profiler (ARP)
	    -g, --generate                   Generate C code from a single QM file (i.e., the QM file with all "apps" merged together)
	    -c, --compile-firmware           Runs the step to build (compile) the firmware
	    -f, --firmware-image-stats       Print stats on the built firmware image
	    -i, --install-binary             Installs the generated binary onto the target device using mspdebug
	    -d, --debug                      Attaches to the amulet for debugging using GDB
	    -h                               Display this help screen

	Examples:

	  Clear out any generated files that may be in the gen/ directory:
	      $ ./aft ~/projects/dot-files/amulet-build.config --clean

	  Run a complete pass of the AFT (note: this does not include install/debug):
	      $ ./aft ~/projects/dot-files/amulet-build.config --all

	  (App Developers!) Start the aftd so that the ARP files are recomputed when changes are made to src files:
	      $ ./aft --aftd-start ~/projects/dot-files/amulet-build.config

	  (App Developers!) Check the status of the aftd (i.e., are there any aftd-related processes running?):
	      $ ./aft --aftd-status

	  (App Developers!) Stop the aftd.
	      $ ./aft --aftd-stop
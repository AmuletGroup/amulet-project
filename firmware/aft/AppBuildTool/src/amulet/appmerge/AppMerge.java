package amulet.appmerge;

import java.util.Vector;

import amulet.gen.CodeGenerator;
import amulet.appbuilder.AppBuilder;
import amulet.manifestreader.ManifestReader;
import amulet.qm.QMClass;
import amulet.qm.QMFile;
import amulet.resourceprofiler.ResourceProfiler;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;

public class AppMerge {
	private static boolean togglePins = false;
	private static boolean passToolchain = false;
	private static boolean doResourceProfile = false;
	private static Vector<String> app_gen_json;
	private String mainc;
	private String appsh;
	private String appsc;
	private QMFile m_template;
	private Vector<QMFile> m_apps;
	private CodeGenerator m_generator;
	private AppBuilder m_builder;
	private ManifestReader m_manifest_reader;

	public ResourceProfiler m_resource_profiler;
	
	public AppMerge(CodeGenerator gen, String amulet_project_dir)
	{
		m_generator = gen;
		m_template = new QMFile(amulet_project_dir+"/firmware/aft/AppBuildTool/gen_template.qm");
		m_apps = new Vector<QMFile>();
		app_gen_json = new Vector<String>();
		m_resource_profiler = new ResourceProfiler();
	}
	
	public boolean addApp(String filename, boolean togglePin, boolean toolChain)
	{
		if (filename == null)
			return false;

		/*
		 * Start Pipeline.
		 *
		 * This will cause the *original* source (QM) files to be parsed/translated.
		 */
		m_builder = new AppBuilder(filename, togglePin, toolChain, m_resource_profiler);
		
		// Only do more work if the pipeline was successful...
		if (m_builder.getAppBuilderReturnStatus()) {
			app_gen_json.add(m_builder.getFinalJSONGraphStructure());
			m_manifest_reader = m_builder.getManifestReader();

			/*
			 * Additional parsing/translation based on the FILENAME.qm.temp file that 
			 * was created in the "pipeline" (called above).
			 */
			QMFile newapp = new QMFile(filename+".temp");
			m_apps.add(newapp);
			for (QMClass c: newapp.getClasses())
			{
				c.updateSignals();
				System.out.println("...adding "+c.getName());
				m_generator.AddApplication(c, m_manifest_reader.getAppName(), m_manifest_reader.getAppShortName(), m_manifest_reader.getAppStatusFlags());
				m_template.addClass(c);
			}
			
			return true;
		}
		return false;
	}

	public Vector<QMFile> getQMFiles() {
		return m_apps;
	}

	public Vector<String> getGeneratedAppJSON() {
		return app_gen_json;
	}

	public boolean addAppAttributes()
	{
		for (QMFile app : m_apps) {
			m_template.addClassAttributes(app.getClasses());
		}
		return true;
	}

	public boolean addAppOperations()
	{
		for (QMFile app : m_apps) {
			m_template.addClassOperations(app.getClasses());
		}
		return true;
	}

	public CodeGenerator getCodeGenerator() {
		return m_generator;
	}

	public boolean generate()
	{
		mainc = m_generator.getMainC();
		appsh = m_generator.getAppsH();
		appsc = m_generator.getAppsC();
		
		m_template.addFile("main.c", mainc);
		m_template.addFile("apps.h", appsh);
		m_template.addFile("apps.c", appsc);
		
		return true;
	}
	
	public boolean saveAs(String filename)
	{
		if (mainc == null || appsh == null)
		{
			if (!generate()) return false;
		}
		return m_template.saveAs(filename);
	}
	
	/**
	 * AppMerge takes a list of amulet services and amulet apps and merges 
	 * them into a single amulet-compatible QM file (that includes the runtime 
	 * "glue").
	 *
	 * NOTE: Amulet services should be specified by name (e.g., AmuletSensors,
	 * AmuletStorage) but apps should be specified with the full path to the QM file. 
	 */
	public static void main(String[] args) 
	{
		if (args.length < 3)
		{
			System.out.println("usage: execname <amulet_root> <output_file_name> <-tp> [<amulet_service_name(s)>] <input_application_name> [ <additional apps> ]");
			System.out.println("Add flags: t: To add toggle pin code\np: To pass through toolchain");
			System.exit(1);
		}
		int idx = 0;

		// Set the root of the amulet project
		String amulet_root = args[0];
		
		// Set the name of the output file to write translated/generated QM code.
		String output_file_name = args[1];
		
		// Parse command-line flags that dictate some of the AFT behavior and enables/disables features.
		if (args[2].indexOf("-") != -1 && args[2].indexOf("t") != -1) {
			togglePins = true;
		}
		if (args[2].indexOf("-") != -1 && args[2].indexOf("p") != -1) {
			passToolchain = true;
		}
		if (args[2].indexOf("-") != -1 && args[2].indexOf("r") != -1) {
			doResourceProfile = true;
		}
		
		idx = 3;

		/* 
		 * Merge apps and generate code (main.c, apps.c, and apps.h)
		 */

		// Collect amulet services
		Vector<String> services = new Vector<String>();
		for ( ; idx < args.length; idx++) {
        	if (args[idx].contains(".qm"))
        		break;
			services.add(args[idx]);
        }
		AppMerge merger = new AppMerge(new CodeGenerator(services), amulet_root);

		System.out.println("Merging "+(args.length-idx)+" QM file(s) into \""+output_file_name+"\".\n");

        // Collect amulet apps
		boolean addSuccessful = false;
		for ( ; idx < args.length; idx++) {
			addSuccessful = merger.addApp(args[idx], togglePins, passToolchain);
			
			// Terminate the merging and indicate to the caller (i.e., the AFT tool-chain) that this step was unsuccessful.
			if (!addSuccessful) {
				System.exit(-1);
			}
        }

		/* 
		 * Do resource profiling of application code. 
		 */
        // Output text representation of resource profiler.
		if (doResourceProfile) {
			System.out.println("\nApplication Resource Profiling enabled...");
		
	        // JSON writer for app structure 
			Writer writer = null;
			String dataFile = "AppBuildTool/src/amulet/resourceprofiler/device_profile_amuletprotov1.0.json";
			String graph_structure_filename = "gen/profiler_ui/graph_structure.js";
			String calculated_costs_filename = "gen/profiler_ui/costs_gen.js";
			
			try {
				// Get a JSON formatted string representing the graphical structure of the applications.
		        String finalJSONtoWrite = "var structure = {\n \"applications\":\n";
				finalJSONtoWrite += merger.getGeneratedAppJSON() + "};\n";
				
				System.out.println(" -> Writing graphical structure of QM applications to: " + graph_structure_filename);
				System.out.println(finalJSONtoWrite);

				// Write to "graph_structure.js"
				writer = new BufferedWriter(new OutputStreamWriter(
		              new FileOutputStream(graph_structure_filename), "utf-8"));
				writer.write(finalJSONtoWrite);
				writer.close();
			} catch (Exception e) {
				e.printStackTrace();
			}

			try {
				// Calculate Energy/Memory costs. [compute energy cost of application based on JSON file with evaluation measurements]
				String costsJSONObjectStr = merger.m_resource_profiler.calculateCost(dataFile);
				costsJSONObjectStr = "var firmware_resource_profile = "+costsJSONObjectStr+";";

				System.out.println(" -> Writing computed application 'costs' & 'execution times' to: " + calculated_costs_filename);
				System.out.println(costsJSONObjectStr);

				// Write to "costs_gen.js"
				writer = new BufferedWriter(new OutputStreamWriter(
		              new FileOutputStream(calculated_costs_filename), "utf-8"));
				writer.write(costsJSONObjectStr);
				writer.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
			
			System.out.println("\n -> Amulet Resource Profiler Warnings...");
			for (String warning : merger.m_resource_profiler.getRPWarnings()) {
				System.out.println(warning);
			}

			// Output the ResourceProfiler in a String representation.
	        System.out.println(merger.m_resource_profiler.toString());
		}

		merger.addAppAttributes();
		merger.addAppOperations();		
		merger.saveAs(output_file_name);
	}

}

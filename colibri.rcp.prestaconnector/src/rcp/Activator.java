/*******************************************************************************
 * 2008-2011 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp;

import java.io.IOException;
import java.net.URL;

import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.ui.plugin.AbstractUIPlugin;
import org.osgi.framework.BundleContext;

/**
 * The activator class controls the plug-in life cycle
 */
public class Activator extends AbstractUIPlugin {

	// The plug-in ID
	public static final String PLUGIN_ID = "colibri.rcp.example";

	// The shared instance
	private static Activator plugin;

	/**
	 * The constructor
	 */
	public Activator() {
	}

	public void start(BundleContext context) throws Exception {
		System.out.println(PLUGIN_ID+" ACTIVATOR <start>");
		super.start(context);
		plugin = this;
	}

	public void stop(BundleContext context) throws Exception {
		System.out.println(PLUGIN_ID+" ACTIVATOR <stop>");
		plugin = null;
		super.stop(context);
	}

	/**
	 * Returns the shared instance
	 *
	 * @return the shared instance
	 */
	public static Activator getDefault() {
		return plugin;
	}

	/**
	 * Returns an image descriptor for the image file at the given
	 * plug-in relative path
	 *
	 * @param path the path
	 * @return the image descriptor
	 */
	public static ImageDescriptor getImageDescriptor(String path) {
		return imageDescriptorFromPlugin(PLUGIN_ID, path);
	}

	public static String pathResolver(String fullPath) {
		String file="";
		URL url=FileLocator.find(plugin.getBundle(), new Path(fullPath), null);
		try{
			file=FileLocator.resolve(url).getFile(); // FOLDER PLUGIN
			//file=FileLocator.toFileURL(url).getFile(); // JAR PLUGIN
		} catch (IOException e){}
		return file;
	}

}

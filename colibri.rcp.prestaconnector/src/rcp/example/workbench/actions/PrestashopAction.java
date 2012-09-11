package rcp.example.workbench.actions;

import java.io.File;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.action.Action;
import org.eclipse.ui.actions.ActionFactory.IWorkbenchAction;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import rcp.example.DOMOrderParser;
import rcp.example.DOMCustomerParser;

public class PrestashopAction extends Action implements IWorkbenchAction {
	private static boolean PRESTA_TEST=false;
	public PrestashopAction() {
		setText("#prestashop");
	}

	public final void run() {
		System.out.println("START OF ACTION RUN!!!");
		String ordersFile = new String();
		String customerFile = new String();
		String outputFile = new String();
		String printerName = new String();
		String initCustomers =new String();
		String[] args = Platform.getCommandLineArgs();
		if(!PRESTA_TEST)
		{
		int i = 0;
		System.out.println("begin parsing arguments");
		
		while (i < args.length) {
			System.out.println("number of arguments"+args.length);
			if (args[i].equals("-ordersFile")) {
				i++;
				ordersFile=args[i];
			}
			if (args[i].equals("-customersFile")) {
				i++;
				customerFile=args[i];
			}
			if (args[i].equals("-outputFile")) {
				i++;
				outputFile=args[i];
			}
			if (args[i].equals("-printerName")) {
				i++;
				printerName=args[i];
			}
			if (args[i].equals("-initCustomers")) {
				i++;
				initCustomers=args[i];
			}
			i++;
		}
		System.out.println("END passing arguments");
		}
		else
		{
			//customerFile="adresses";
			//ordersFile="orders";
			//outputFile="test";
			//printerName="";
			//initCustomers="alladresses";
		}
		System.out.println("customerFile="+customerFile+" ordersFile="+ordersFile+
				" outputFile="+outputFile+" printerName="+printerName);
		initCustomers="alladresses";
		Document doc=null;
		try {
			 
			DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
	 
			// root elements
			doc = docBuilder.newDocument();
			Element rootElement = doc.createElement("colibriPrestaConnector");
			doc.appendChild(rootElement);
		  } catch (ParserConfigurationException pce) {
			pce.printStackTrace();
		  } /*
		if(!initCustomers.isEmpty())
		{
			DOMCustomerParser parser = new DOMCustomerParser();
			parser.InitialParse(initCustomers);
		}
		*/
		boolean run=true;
		if(customerFile.isEmpty())
		{
			System.out.println("Customers file is missing");
			run=false;
		}
		if(ordersFile.isEmpty())
		{
			System.out.println("Orders file is missing");
			run=false;
		}
		if(outputFile.isEmpty())
		{
			System.out.println("Output file is missing");
			run=false;
		}
		if(run)
		{
			DOMCustomerParser cparser = new DOMCustomerParser();
			cparser.ParseCustomer(customerFile,doc);
			DOMOrderParser oparser = new DOMOrderParser();
			oparser.ParseOrders(ordersFile,doc,printerName);
			saveDocToFile(doc, outputFile);
		}

	}

	public void dispose() {
	
	}
	
	private void saveDocToFile(Document doc,String file)
	{
		System.out.println("Beging file save");
		// write the content into xml file
		TransformerFactory transformerFactory = TransformerFactory.newInstance();
		Transformer transformer;
		try {
			System.out.println("Try");
			transformer = transformerFactory.newTransformer();
			DOMSource source = new DOMSource(doc);
			StreamResult result = new StreamResult(new File(file));

			// Output to console for testing
			transformer.transform(source, result);
			//StreamResult result2 = new StreamResult(System.out);
			//transformer.transform(source, result);
		}

		catch (TransformerException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("File saved to "+file);
	}
}
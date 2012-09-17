package rcp.example;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;

import java.io.File;
import java.io.IOException;
import java.util.Collection;
import java.util.Iterator;

import org.dma.utils.eclipse.swt.DialogHandler;
import org.dma.utils.java.Debug;
import org.dma.utils.java.array.ErrorList;
import org.eclipse.core.runtime.Platform;

import rcp.colibri.core.mappers.filters.FilterMap;
import rcp.colibri.core.mappers.filters.FilterMap.OPERANDS.TYPES;
import rcp.colibri.core.mappers.filters.FilterMap.OPERATORS;
import rcp.colibri.core.mappers.filters.FilterOperandMap;
import rcp.colibri.core.vars.database.DatabaseVARS;
import rcp.colibri.core.vars.database.DatabaseVARS.FIELDS;
import rcp.colibri.core.vars.database.PopulateVARS.ENTIDADESTIPOS;
import rcp.colibri.dao.database.ColibriDatabase;
import rcp.colibri.dao.database.model.Entidades;
import rcp.colibri.dao.database.query.QueryDefinition;
import rcp.colibri.dao.database.query.QueryDefinition.QUERY_ORDER;
import rcp.colibri.workbench.processamento.entidades.facturacao.FacturacaoPreference.TIPO;

public class DOMCustomerParser {
	public void InitialParse(String customFile)
	{
		try {
			File fXmlFile = new File(customFile);
			DocumentBuilderFactory dbFactory = DocumentBuilderFactory
					.newInstance();
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			Document doc = dBuilder.parse(fXmlFile);
			doc.getDocumentElement().normalize();

			System.out.println("ParseCustomer Root element :"
					+ doc.getDocumentElement().getNodeName());
			NodeList nList = doc.getElementsByTagName("address");
			System.out.println("ParseCustomer number of adress:" + nList.getLength());
			
			for (int temp = 0; temp < nList.getLength(); temp++) {
				Node nNode = nList.item(temp);
				if (nNode.getNodeType() == Node.ELEMENT_NODE) {
					Element eElement = (Element) nNode;
					System.out.println("First Name : "
							+ getTagValue("firstname", eElement));
					System.out.println("Last Name : "
							+ getTagValue("lastname", eElement));
					String name=getTagValue("firstname", eElement)+" "+getTagValue("lastname", eElement);
					String id = getTagValue("id", eElement);
					System.out.println(id);
					id = "presta:" + id;
					Collection<Entidades> collection = ColibriDatabase
							.getCollectionEntidades(new QueryDefinition(
									new FilterMap(DatabaseVARS.entidades)
											.addRule(
													DatabaseVARS.FIELDS.entidades_nome.name,
													FilterMap.OPERATORS.MATH.EQUAL,
													new FilterOperandMap(name,
															TYPES.VALUE)),
									new String[] { DatabaseVARS.FIELDS.entidades_key.name },
									QueryDefinition.QUERY_ORDER.ASCENDING));
					if(collection.size()>1)
						System.out.println("Warning more than one customer found:"+id);
					else if(collection.size()==0)
						System.out.println("Warning no customer found:"+id);
					 
					if (collection.size() ==1) {
						System.out.println("Customer found:"+id);
						try {
							// cria objecto entidade
							Iterator<Entidades> iterator = collection.iterator();
							Entidades entidade = ColibriDatabase.loadEntidades(iterator.next()
									.getKey());
							
							entidade.setObs(id);
							
							// grava a entidade na base de dados
							ErrorList error = ColibriDatabase.storeEntidades(
									entidade, true);
							DialogHandler.error(error.getErrors());

						} catch (Exception e) {
							e.printStackTrace();
						}
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	public void ParseCustomer(String customFile, Document output) {
		try {

			Element adresses = output.createElement("Adresses");
			Element rootElement = output.getDocumentElement();
			rootElement.appendChild(adresses);

			File fXmlFile = new File(customFile);
			DocumentBuilderFactory dbFactory = DocumentBuilderFactory
					.newInstance();
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			Document doc = dBuilder.parse(fXmlFile);
			doc.getDocumentElement().normalize();

			
			NodeList nList = doc.getElementsByTagName("address");
			System.out.println("ParseCustomer number of adresses:" + nList.getLength());
			
			for (int temp = 0; temp < nList.getLength(); temp++) {
				Node nNode = nList.item(temp);
				if (nNode.getNodeType() == Node.ELEMENT_NODE) {
					Element eElement = (Element) nNode;
					
					System.out.println("Parsing Client "+ getTagValue("firstname", eElement)
							+" "+ getTagValue("lastname", eElement)+" with ID="+getTagValue("id", eElement));
					String id = getTagValue("id", eElement);
					id = "presta:" + id;
					Element adress = output.createElement("Adress");
					adresses.appendChild(adress);
					Element ide = output.createElement("presta_id");
					ide.appendChild(output.createTextNode(getTagValue("id", eElement)));
					adress.appendChild(ide);
					Collection<Entidades> collection = ColibriDatabase
							.getCollectionEntidades(new QueryDefinition(
									new FilterMap(DatabaseVARS.entidades)
											.addRule(
													DatabaseVARS.FIELDS.entidades_obs.name,
													FilterMap.OPERATORS.MATH.EQUAL,
													new FilterOperandMap(id,
															TYPES.VALUE)),
									new String[] { DatabaseVARS.FIELDS.entidades_key.name },
									QueryDefinition.QUERY_ORDER.ASCENDING));
					if (collection.size() == 0) {
						Element newe = output.createElement("new");
						newe.appendChild(output.createTextNode("true"));
						adress.appendChild(newe);
						System.out.println("Cliente nao encontrado "
								+ getTagValue("id", eElement));
						try {
							// cria objecto entidade
							Entidades entidade = new Entidades(
									ColibriDatabase
											.getNextNumeroEntidades(ENTIDADESTIPOS.cliente.codigo),
									ColibriDatabase
											.loadEntidadestipos(ENTIDADESTIPOS.cliente.codigo));

							entidade.setNome(getTagValue("firstname", eElement)
									+ " " + getTagValue("lastname", eElement));
							String nif = getTagValue("vat_number", eElement);
							if (!nif.isEmpty())
								entidade.setNif(nif);
							entidade.setMorada(getTagValue("address1", eElement)
									+ "\n"
									+ getTagValue("address2", eElement)
									+ "\n"
									+ getTagValue("postcode", eElement)
									+ " " + getTagValue("city", eElement));
							entidade.setTelefone(getTagValue("phone", eElement));
							entidade.setTelemovel(getTagValue("phone_mobile",
									eElement));
							System.out.println("CountryID"+getTagValue("id_country",
									eElement));
							System.out.println("Country:"+getCountry(getTagValue(
									"id_country", eElement)));
							System.out.println(ColibriDatabase.loadPaises(
									getCountry(getTagValue("id_country",
											eElement))).getCodigoToText());
							entidade.setPais(ColibriDatabase
									.loadPaises(getCountry(getTagValue(
											"id_country", eElement))));
							entidade.setObs(id);
							if (!hasTax(getTagValue("id_country", eElement))) {
								entidade.setTipoiva(1);
								entidade.setIsencaoiva(ColibriDatabase
										.loadIsencoesiva("EXP"));
								entidade.setCodigoiva(ColibriDatabase
										.loadCodigosiva("01"));
							}
							// grava a entidade na base de dados
							System.out.println("Prepare to save new customer");
							ErrorList error = ColibriDatabase.storeEntidades(
									entidade, false);
							Element idcoli = output.createElement("colibri_id");
							idcoli.appendChild(output.createTextNode(entidade.getNumeroToText()));
							adress.appendChild(idcoli);
							// apresenta possiveis erros
							DialogHandler.error(error.getErrors());
							System.out.println("New customer done");

						} catch (Exception e) {
							e.printStackTrace();
						}
					}
					else
					{
						System.out.println("Cliente encontrado");
						Element newe = output.createElement("new");
						newe.appendChild(output.createTextNode("false"));
						adress.appendChild(newe);
	
					}
					
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static String getTagValue(String sTag, Element eElement) {
		NodeList nlList = eElement.getElementsByTagName(sTag).item(0)
				.getChildNodes();
		if (nlList.getLength() == 0)
			return "";
		Node nValue = (Node) nlList.item(0);

		return nValue.getNodeValue();
	}

	private String getCountry(String id) {
		System.out.println("Using country file "+Platform.getInstallLocation().getURL().getPath()+"countries");
		try {
			File fXmlFile = new File(Platform.getInstallLocation().getURL().getPath()+"countries");
			DocumentBuilderFactory dbFactory = DocumentBuilderFactory
					.newInstance();
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			Document doc;
			doc = dBuilder.parse(fXmlFile);
			doc.getDocumentElement().normalize();

			
			NodeList nList = doc.getElementsByTagName("country");
			
			System.out.println("Loaded "+nList.getLength()+" countries");
			for (int temp = 0; temp < nList.getLength(); temp++) {
				Node nNode = nList.item(temp);
				if (nNode.getNodeType() == Node.ELEMENT_NODE) {
					Element eElement = (Element) nNode;
					if (getTagValue("id", eElement).equals(id)) {
						System.out.println("TRUE");
						return getTagValue("iso_code", eElement);
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return "";
	}

	private boolean hasTax(String country) {
		try {
			File fXmlFile = new File(Platform.getInstallLocation().getURL().getPath()+"taxes");
			DocumentBuilderFactory dbFactory = DocumentBuilderFactory
					.newInstance();
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			Document doc;
			doc = dBuilder.parse(fXmlFile);
			doc.getDocumentElement().normalize();

			System.out.println("Root element :"
					+ doc.getDocumentElement().getNodeName());
			NodeList nList = doc.getElementsByTagName("tax_rule");
			System.out.println("-----------------------" + nList.getLength());

			for (int temp = 0; temp < nList.getLength(); temp++) {
				Node nNode = nList.item(temp);
				if (nNode.getNodeType() == Node.ELEMENT_NODE) {
					Element eElement = (Element) nNode;
					if (getTagValue("id_country", eElement).equals(country) && getTagValue("id_tax_rules_group", eElement).equals("1")) {
						if (getTagValue("id_tax", eElement).equals("1")) {
							return true;
						} else {
							System.out
									.println("hasTax:Country found but taxID!=1");
							return false;
						}
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return false;
	}

}

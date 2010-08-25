package com.hiyokogames.server;
import java.io.*;
import java.lang.*;
import java.util.LinkedList;
import nu.xom.*;
import nu.xom.xslt.*;
import nux.xom.xquery.*;
import uk.ltd.getahead.dwr.ExecutionContext;
import javax.servlet.http.HttpServletRequest;

public class XMLManager {
	private XMLManager() throws Exception {
		mBuilder = new Builder();
		Class xslt=null;
		try {
			xslt = Class.forName("com.icl.saxon.TransformerFactoryImpl");
		}
		catch(ClassNotFoundException f) {
			xslt = Class.forName("net.sf.saxon.TransformerFactoryImpl");
		}
		System.setProperty("javax.xml.transform.TransformerFactory", xslt.getName());

		// read data and xslt
		mData = mBuilder.build(new File("/home/deepmind/hiyoko_html/site.xml"));
		mXSLT = new XSLTransform(mBuilder.build(new File("/home/deepmind/hiyoko_html/logic.xsl")));

		System.err.println("====== XMLManager up and running... ========");
	}

	public static synchronized XMLManager getInstance() throws Exception {
		if (mInstance == null) {
			mInstance = new XMLManager();
		}
		return mInstance;
	}
	
	public Object clone() throws CloneNotSupportedException {
		throw new CloneNotSupportedException(); 
	}
	
	private static XMLManager mInstance;

	// 
	// non-singleton members
	// 
	public synchronized Document canonicalize(Document src) throws IOException, ParsingException {
		String xml = new String(nux.xom.pool.XOMUtil.toCanonicalXML(src));
		return mBuilder.build(xml);
	}

	public synchronized void reload() throws Exception {
		mData = null;
		mData = mBuilder.build(new File("/home/deepmind/hiyoko_html/site.xml"));

		mXSLT = null;
		mXSLT = new XSLTransform(mBuilder.build(new File("/home/deepmind/hiyoko_html/logic.xsl")));
	}

	public synchronized void loadSet(String xml, String xslt) throws Exception {
		mData = null;
		mXSLT = null;
		mData = mBuilder.build(new File(xml));
		mXSLT = new XSLTransform(mBuilder.build(new File(xslt)));
	}
	public synchronized Document getDocument(String fileName) throws ParsingException, ValidityException, IOException {
		return mBuilder.build(fileName);
	}
	public synchronized String getUserLanguage() {
		HttpServletRequest req = ExecutionContext.get().getHttpServletRequest();
		String lang="en";
		if (req!=null) {
			String langs = req.getHeader("Accept-Language");
			if (langs!=null) {
				String[] languages = langs.split(",");
				for (int l=0;l<languages.length;++l) {
					if (languages[l]!=null) {
						String[] split = languages[l].split("-");
						if (split!=null) {
							if (split[0].equals("fr") || split[0].equals("ja")) {
								lang=split[0];
								return lang;
							}
						}
					}
				}
			}
		}
		return lang;
	}
	public synchronized Document getSiteFragment(String query, String arg) throws XSLException {
		// 0. Get various headers
		String lang = getUserLanguage();
		
		// 1. Set XSLT parameters
		mXSLT.setParameter("lang", lang);
		mXSLT.setParameter("arg", arg);
		mXSLT.setParameter("xquery", query);
System.err.println("=== Lang: "+lang+" ===");
		
		// 2. Get XQuery results
System.err.println("=== XQuery: "+query+" ===");
		Nodes results = XQueryUtil.xquery(mData, query);
System.err.println("=== Results: "+results.size()+" ===");

		Nodes xformed = mXSLT.transform(results);
System.err.println("=== XSLT: "+xformed.size()+" ===");

		// 3. Build element	
		Element enveloppe = new Element("enveloppe");
		enveloppe.setNamespaceURI("http://www.w3.org/1999/xhtml");

		// 4. Copy nodes
		for (int z=0; z<xformed.size(); ++z) {
			Node node = xformed.get(z).copy();
			try {
				Element element = (Element) node;
				// remove all namespace declarations
				// (TODO)
				// FIXME: hard-coded list for now
				element.removeNamespaceDeclaration("saxon");
				element.removeNamespaceDeclaration("date");
				enveloppe.appendChild(element);
			}
			catch (Exception e) {
				enveloppe.appendChild(node);
			}
		}

		// 5. Create a document
		Document d = new Document(enveloppe);
		System.err.println("=== XML: \n"+d.toXML()+"\n===================");
		return d;
	}
	private Builder      mBuilder;
	private Document     mData;
	private XSLTransform mXSLT;
}


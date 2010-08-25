// $Id: example.cpp 9734 2008-03-04 16:36:47Z jcayzac $
#include <inc/streams/fileinputstream.h>
#include <inc/dom/dom_text.h>
#include <inc/dom/dom_query.h>
#include <inc/dom/dom_reader.h>

namespace ex {

void dom_example() {
	dom::Document xml;
	// Read prod configuration
	msdk::CPath path = app->MakePathAbsolute("dom.xml");
	msdk::shared_ptr<msdk::CFileInputStream> fis(new msdk::CFileInputStream(path));
	dom::CDOMReader dr(fis);
	
	xml = dr.readDocument();
	if (!xml) {
		LOG_DEBUG("While parsing dom.xml:");
		msdk::CString errorMsg;
		while (dr.nextError(errorMsg)) {
			LOG_DEBUG((const char *) errorMsg.GetAsciiStr());
		}
		return false;
	}

	// Read some values...
	
	unsigned short usInformationTime	= 0;
	unsigned short usPreparationTime	= 0;
	unsigned short usExecutionTime		= 0; 
	unsigned short usInformationTimeMin	= 0; 
	unsigned short usPreparationTimeMin	= 0; 
	unsigned short usExecutionTime		= 0; 
	unsigned short usConcatenationTime	= 0; 
	unsigned short usLoadAheadTime		= 0; 
	unsigned short usForwardPosition	= 0; 
	
	dom::Node value;
	dom::XPathExpression expr("/example/times/*");
	expr.evaluateNode(xml);
	while(expr >> value) {
		const dom::DOMString& name = value->nodeName();
		if (name=="information")		value >> usInformationTime; else
		if (name=="preparation")		value >> usPreparationTime; else
		if (name=="execution")			value >> usExecutionTime; else
		if (name=="information-min")	value >> usInformationTimeMin; else
		if (name=="preparation-min")	value >> usPreparationTimeMin; else
		if (name=="execution")			value >> usExecutionTime; else
		if (name=="concatenation")		value >> usConcatenationTime; else
		if (name=="load-ahead")			value >> usLoadAheadTime;  else
		if (name=="forward-position") value >> usForwardPosition; 
	}
}

} //namespace navcore

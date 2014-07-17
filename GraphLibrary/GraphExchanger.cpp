#pragma once

#include "GraphExchanger.h"

namespace msonlab
{
	class XmlDomErrorHandler : public HandlerBase
	{
	public:
		void fatalError(const SAXParseException &exc)
		{
			std::cout << ("Fatal parsing error at line %d\n", (int)exc.getLineNumber()) << std::endl;
		}
	};

	//GraphExchanger::GraphExchanger(msonlab::Graph::gPtr toExchange)
	//{
	//	graph = toExchange;
	//}

	Graph::gPtr GraphExchanger::ImportGraph(std::string inputPath)
	{
		// reset GraphBuilder
		GraphBuilder::reset();

		// initialize xerces
		XMLPlatformUtils::Initialize();

		XercesDOMParser* parser = new XercesDOMParser();
		ErrorHandler* error_handler = new XmlDomErrorHandler();
		parser->setErrorHandler(error_handler);

		// parse input xml
		parser->parse(XMLString::transcode(XMLString::transcode(inputPath.c_str())));
		DOMDocument* doc = parser->getDocument();

		// lookup keys for import
		std::string k_NodeType;
		std::string k_EdgeType;
		std::string k_nodegraphics;
		std::string k_node_CustomData;
		std::string k_edge_CustomData;

		DOMNodeList* key_list = doc->getElementsByTagName(XMLString::transcode(std::string("key").c_str()));
		for (size_t i = 0; i < key_list->getLength(); ++i)
		{
			auto element = dynamic_cast<DOMElement*>(key_list->item(i));

			std::string id = XMLString::transcode(element->getAttribute(XMLString::transcode(std::string("id").c_str())));
			std::string k_for = XMLString::transcode(element->getAttribute(XMLString::transcode(std::string("for").c_str())));
			std::string f_type = XMLString::transcode(element->getAttribute(XMLString::transcode(std::string("yfiles.type").c_str())));
			std::string attr_name = XMLString::transcode(element->getAttribute(XMLString::transcode(std::string("attr.name").c_str())));
			std::string attr_type = XMLString::transcode(element->getAttribute(XMLString::transcode(std::string("attr.type").c_str())));

			if (k_for == "node")
			{
				// possible key for nodegraphics, NodeType and CustomData (for node)
				if (f_type == "nodegraphics") { k_nodegraphics = id; }
				else if (attr_name == "NodeType") { k_NodeType = id; }
				else if (attr_name == "CustomData"){ k_node_CustomData = id; }
			}

			if (k_for == "edge")
			{
				// possible key for edgegraphics, EdgeType and CustomData (for node)
				if (attr_name == "EdgeType") { k_EdgeType = id; }
				else if (attr_name == "CustomData") { k_edge_CustomData = id; }
			}

		}

		// check that all keys found
		if (k_nodegraphics.length() == 0 || k_NodeType.length() == 0 || k_node_CustomData.length() == 0 ||
			k_EdgeType.length() == 0 || k_edge_CustomData.length() == 0)
		{
			std::cout << "At least one key not found. Correct the input XML to the correct format and try again." << std::endl;
			return nullptr;
		}

		// check for "msonlab::GraphExchanger" comment in the document
		bool ge_signal = false;
		DOMNodeList* comment_list = doc->getDocumentElement()->getChildNodes();
		for (size_t i = 0; i < comment_list->getLength(); ++i)
		{
			auto curr_node_xml = dynamic_cast<DOMComment*>(comment_list->item(i));

			if (curr_node_xml)
			{
				std::string comm = XMLString::transcode(curr_node_xml->getTextContent());

				if (comm == "msonlab::GraphExchanger")
				{
					ge_signal = true;
					break;
				}
			}
		}

		// create id variable
		unsigned int id = 0;
		unsigned int node_count = 0;

		// import nodes
		DOMNodeList* node_list = doc->getElementsByTagName(XMLString::transcode(std::string("node").c_str()));
		for (size_t i = 0; i < node_list->getLength(); ++i)
		{
			auto curr_node_xml = dynamic_cast<DOMElement*>(node_list->item(i));

			std::string node_type;
			std::string custom_data;
			types::LabelType nodegraphics_label;
			unsigned int node_id = -1;

			std::string id_string = XMLString::transcode(curr_node_xml->getAttribute(XMLString::transcode(std::string("id").c_str())));
			id_string.erase(0, 1);
			node_id = atoi(id_string.c_str());

			DOMNodeList* data_list = curr_node_xml->getElementsByTagName(XMLString::transcode(std::string("data").c_str()));
			for (size_t j = 0; j < data_list->getLength(); ++j)
			{
				auto curr_data_xml = dynamic_cast<DOMElement*>(data_list->item(j));

				std::string data_key = XMLString::transcode(curr_data_xml->getAttribute(XMLString::transcode(std::string("key").c_str())));

				// node type data
				if (data_key == k_NodeType) { node_type = XMLString::transcode(curr_data_xml->getTextContent()); }
				// custom data
				else if (data_key == k_node_CustomData) { custom_data = XMLString::transcode(curr_data_xml->getTextContent()); }
				// nodegraphics data
				else if (data_key == k_nodegraphics)
				{
					DOMNodeList* label_list = curr_data_xml->getElementsByTagName(XMLString::transcode(std::string("y:NodeLabel").c_str()));
					if (label_list->getLength() > 0)
					{
						std::string stmp = XMLString::transcode(dynamic_cast<DOMElement*>(label_list->item(0))->getTextContent());
						nodegraphics_label = std::wstring(stmp.begin(), stmp.end());
					}
				}
			}

			GraphBuilder::add_node(node_id, node_type, nodegraphics_label, custom_data);

			node_count++;
		}


		// import edges
		DOMNodeList* edge_list = doc->getElementsByTagName(XMLString::transcode(std::string("edge").c_str()));
		for (size_t i = 0; i < edge_list->getLength(); ++i)
		{
			auto curr_edge_xml = dynamic_cast<DOMElement*>(edge_list->item(i));

			std::string edge_type;
			std::string custom_data;
			unsigned int edge_id = -1;
			unsigned int source_id = -1;
			unsigned int target_id = -1;

			std::string id_string = XMLString::transcode(curr_edge_xml->getAttribute(XMLString::transcode(std::string("id").c_str())));
			id_string.erase(0, 1);
			edge_id = atoi(id_string.c_str());
			if (!ge_signal) edge_id += node_count;

			std::string source_id_string = XMLString::transcode(curr_edge_xml->getAttribute(XMLString::transcode(std::string("source").c_str())));
			source_id_string.erase(0, 1);
			source_id = atoi(source_id_string.c_str());

			std::string target_id_string = XMLString::transcode(curr_edge_xml->getAttribute(XMLString::transcode(std::string("target").c_str())));
			target_id_string.erase(0, 1);
			target_id = atoi(target_id_string.c_str());

			DOMNodeList* data_list = curr_edge_xml->getElementsByTagName(XMLString::transcode(std::string("data").c_str()));
			for (size_t j = 0; j < data_list->getLength(); ++j)
			{
				auto curr_data_xml = dynamic_cast<DOMElement*>(data_list->item(j));

				std::string data_key = XMLString::transcode(curr_data_xml->getAttribute(XMLString::transcode(std::string("key").c_str())));

				// edge type data
				if (data_key == k_EdgeType) { edge_type = XMLString::transcode(curr_data_xml->getTextContent()); }
				// custom data
				else if (data_key == k_edge_CustomData) { custom_data = XMLString::transcode(curr_data_xml->getTextContent()); }
			}

			GraphBuilder::add_edge(edge_id, source_id, target_id, edge_type, custom_data);
		}

		if (doc) doc->release();
		XMLPlatformUtils::Terminate();

		return GraphBuilder::build();
	}

	void GraphExchanger::createKeys(DOMDocument* pDOMDocument) const
	{
		DOMElement* rootElement = pDOMDocument->getDocumentElement();

		// create keys

		DOMElement* key0 = pDOMDocument->createElement(L"key");
		key0->setAttribute(L"for", L"node");
		key0->setAttribute(L"id", L"d0");
		key0->setAttribute(L"attr.name", L"NodeType");
		key0->setAttribute(L"attr.type", L"string");
		rootElement->appendChild(key0);

		DOMElement* key1 = pDOMDocument->createElement(L"key");
		key1->setAttribute(L"for", L"edge");
		key1->setAttribute(L"id", L"d1");
		key1->setAttribute(L"attr.name", L"EdgeType");
		key1->setAttribute(L"attr.type", L"string");
		rootElement->appendChild(key1);

		DOMElement* key2 = pDOMDocument->createElement(L"key");
		key2->setAttribute(L"for", L"node");
		key2->setAttribute(L"id", L"d2");
		key2->setAttribute(L"yfiles.type", L"nodegraphics");
		rootElement->appendChild(key2);

		DOMElement* key3 = pDOMDocument->createElement(L"key");
		key3->setAttribute(L"for", L"edge");
		key3->setAttribute(L"id", L"d3");
		key3->setAttribute(L"yfiles.type", L"edgegraphics");
		rootElement->appendChild(key3);

		DOMElement* key4 = pDOMDocument->createElement(L"key");
		key4->setAttribute(L"for", L"node");
		key4->setAttribute(L"id", L"d4");
		key4->setAttribute(L"attr.name", L"CustomData");
		key4->setAttribute(L"attr.type", L"string");
		rootElement->appendChild(key4);

		DOMElement* key5 = pDOMDocument->createElement(L"key");
		key5->setAttribute(L"for", L"edge");
		key5->setAttribute(L"id", L"d5");
		key5->setAttribute(L"attr.name", L"CustomData");
		key5->setAttribute(L"attr.type", L"string");
		rootElement->appendChild(key5);

	}

	bool GraphExchanger::ExportGraph(const Graph::gPtr& graph, std::string outputPath) const
	{
		XMLPlatformUtils::Initialize();

		DOMImplementation* p_DOMImplementation = nullptr;
		p_DOMImplementation = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("core"));

		DOMDocument* pDOMDocument = p_DOMImplementation->createDocument(0, L"graphml", 0);

		// setup the root element
		DOMElement* rootElement = pDOMDocument->getDocumentElement();
		rootElement->setAttribute(L"xmlns", L"http://graphml.graphdrawing.org/xmlns");
		rootElement->setAttribute(L"xmlns:xsi", L"http://www.w3.org/2001/XMLSchema-instance");
		rootElement->setAttribute(L"xmlns:y", L"http://www.yworks.com/xml/graphml");
		rootElement->setAttribute(L"xsi:schemaLocation", L"http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd");

		// comment that msonlab::GraphExchanger exported
		DOMComment* comment = pDOMDocument->createComment(L"msonlab::GraphExchanger");
		rootElement->appendChild(comment);

		// create keys for YED
		createKeys(pDOMDocument);


		// create graph
		DOMElement* graphElement = pDOMDocument->createElement(L"graph");
		graphElement->setAttribute(L"edgedefault", L"directed");
		graphElement->setAttribute(L"id", L"G");
		rootElement->appendChild(graphElement);

		// add nodes to the graph
		auto nodes = graph->getNodes();
		for (unsigned int i = 0; i < nodes.size(); ++i)
		{
			graphElement->appendChild(nodes.at(i)->serialize(pDOMDocument, "d2", "d0", "d4"));
		}

		// add edges to the graph
		auto edges = graph->getEdges();
		for (unsigned int i = 0; i < edges.size(); ++i)
		{
			graphElement->appendChild(edges.at(i)->serialize(pDOMDocument, "d3", "d1", "d5"));
		}

		// save XML to file
		outputXML(pDOMDocument, outputPath);
		outputXML(pDOMDocument, "test.graphml");
		pDOMDocument->release();
		XMLPlatformUtils::Terminate();

		return true;
	}

	bool GraphExchanger::outputXML(DOMDocument* myDOMDocument, std::string filePath) const
	{
		//Return the first registered implementation that has the desired features. In this case, we are after a DOM implementation that has the LS feature... or Load/Save. 
		DOMImplementation *implementation = DOMImplementationRegistry::getDOMImplementation(L"LS");

		// Create a DOMLSSerializer which is used to serialize a DOM tree into an XML document. 
		DOMLSSerializer *serializer = ((DOMImplementationLS*)implementation)->createLSSerializer();

		// Make the output more human readable by inserting line feeds. 
		if (serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
			serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

		// The end-of-line sequence of characters to be used in the XML being written out.  
		serializer->setNewLine(XMLString::transcode("\r\n"));

		// Convert the path into Xerces compatible XMLCh*. 
		XMLCh *tempFilePath = XMLString::transcode(filePath.c_str());

		// Specify the target for the XML output. 
		XMLFormatTarget *formatTarget = new LocalFileFormatTarget(tempFilePath);

		// Create a new empty output destination object. 
		DOMLSOutput *output = ((DOMImplementationLS*)implementation)->createLSOutput();

		// Set the stream to our target. 
		output->setByteStream(formatTarget);

		// Write the serialized output to the destination. 
		serializer->write(myDOMDocument, output);

		// Cleanup. 
		serializer->release();
		XMLString::release(&tempFilePath);
		delete formatTarget;
		output->release();

		return true;
	}
}
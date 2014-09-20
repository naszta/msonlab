#include "NodeTest.h"

namespace msonlab
{
	NodeTest::NodeTest(unsigned int _id, types::LabelType _label, types::DataPtr _value, unsigned comp_time)
		: Node(_id, _label, _value, GraphExchanger::getSupportedNodeTypeName(GraphExchanger::supportedNodeType::ADD), comp_time)
	{
	}

	NodeTest::NodeTest(const NodeTest& other) : Node(other)
	{
		if (this != &other)
		{
			*this = other;
		}
	}

	// does NOT copy connections
	NodeTest& NodeTest::operator=(const NodeTest& other)
	{
		Node::operator=(other);
		return *this;
	}

	Node::nPtr NodeTest::clone()
	{
		return std::make_shared<NodeTest>(*this);
	}

	IProcessable::pVect NodeTest::process()
	{
		IProcessable::pVect ret;
		return ret;
	}

	// green + blue
	std::string NodeTest::get_color() const
	{
		return "#00FFFF";
	}
}
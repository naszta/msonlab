#include "NodeTest.h"


namespace msonlab
{
	NodeTest::NodeTest(unsigned int _id, types::LabelType _label, types::DataPtr _value, unsigned comp_time)
		: Node(_id, _label, _value, NodeDescriptor("TEST", "#00FFFF"), comp_time)
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

	NodePtr NodeTest::clone() const
	{
		return std::make_shared<NodeTest>(*this);
	}

	IProcessableVect NodeTest::process()
	{
		IProcessableVect ret;
		return ret;
	}
}
#ifndef NODE_TEST_H
#define NODE_TEST_H
#include "Node.h"
#include "Edge.h"
#include "GraphExchanger.h"
#include <memory>

namespace msonlab
{
	class NodeTest : public Node
	{
	public:
		NodeTest(unsigned int _id, types::LabelType _label, types::DataPtr _value, unsigned comp_time);
		NodeTest(const NodeTest& other);

		NodeTest& operator=(const NodeTest& other);
		Node::nPtr clone();

		virtual IProcessable::pVect process();

		// compile
		void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule) {}

		// exchange
		std::string get_color() const;
	};
}
#endif
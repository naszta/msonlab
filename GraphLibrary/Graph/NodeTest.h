#ifndef GRAPHLIB_NODE_TEST_H
#define GRAPHLIB_NODE_TEST_H

#include <memory>

#include "Edge.h"
#include "Node.h"

namespace msonlab
{
	class NodeTest : public Node
	{
	public:
		NodeTest(unsigned int _id, types::LabelType _label, types::DataPtr _value, unsigned comp_time);
		NodeTest(const NodeTest& other);

		NodeTest& operator=(const NodeTest& other);
		virtual NodePtr clone() const override;
		virtual IProcessableVect process() override;

		// compile
		void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule) override {
			// only used for testing purpose. Not intended to get compiled.
		}

		// exchange
		std::string get_color() const;
	};
}
#endif
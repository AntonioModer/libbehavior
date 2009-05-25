#include "BehaviorTree.h"
#define BOOST_TEST_MODULE SequentialNodeTest
#include <boost/test/unit_test.hpp>


using namespace BehaviorTree;


BOOST_AUTO_TEST_CASE( currentPositionInitialize )
{
	SequentialNode* node = new SequentialNode();
	BOOST_CHECK(node->currentPosition == -1);
}

//Test for the current position and return value of the sequentialNode. Also check the destructor.
BOOST_AUTO_TEST_CASE( simple1 )
{
	SequentialNode* node = new SequentialNode();
	AlwaysRunning* ar = new AlwaysRunning();
	node->addChild(ar);
	int dummy_agent = 0;
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 0);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 0);
	delete node;
}

BOOST_AUTO_TEST_CASE( simple2 )
{
	SequentialNode* node = new SequentialNode();
	node->addChild(new AlwaysSuccess());
	int dummy_agent = 0;
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_SUCCESS);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	delete node;
}

BOOST_AUTO_TEST_CASE( simple3 )
{
	SequentialNode* node = new SequentialNode();
	node->addChild(new AlwaysFailure());
	int dummy_agent = 0;
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_FAILURE);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	delete node;
}

//run it twice to make sure everything gets reset correctly
BOOST_AUTO_TEST_CASE( simple4 )
{
	SequentialNode* node = new SequentialNode();
	node->addChild(new SuccessAfterOne());
	int dummy_agent = 0;
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 0);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_SUCCESS);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 0);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_SUCCESS);
	delete node;
}

//two level sequentialNodes
//only checking correct behavior from the top
BOOST_AUTO_TEST_CASE( complex1 )
{
	SequentialNode* node = new SequentialNode();
	node->addChild(new SuccessAfterOne());
	SequentialNode* top = new SequentialNode();
	top->addChild(node);
	top->addChild(new SuccessAfterOne());
	top->addChild(new AlwaysFailure());

	int dummy_agent = 0;
	top->init(&dummy_agent);
	for (int i = 0 ; i< 2 ; i++)
	{
		BOOST_CHECK_EQUAL(top->currentPosition, -1);
		BOOST_CHECK_EQUAL(top->getChildren().size(),3);
		BOOST_CHECK_EQUAL(top->execute(&dummy_agent),BT_RUNNING);
		BOOST_CHECK_EQUAL(top->currentPosition, 0);
		BOOST_CHECK_EQUAL(top->execute(&dummy_agent),BT_RUNNING);
		BOOST_CHECK_EQUAL(top->currentPosition, 1);
		BOOST_CHECK_EQUAL(top->execute(&dummy_agent),BT_FAILURE);
		BOOST_CHECK_EQUAL(top->currentPosition, -1);
	}
	delete top;
}
#ifndef CONDITIONALNODE_H_
#define CONDITIONALNODE_H_

#include <vector>
#include <map>
#include "boost\\bind.hpp"
namespace BehaviorTree
{
	class NoClass{};
	enum INT_TEST {LESS_THAN,GREATER_THAN,LESS_OR_EQ,GREATER_OR_EQ,EQUAL,NOT_EQUAL};
	template <class T = NoClass>
	class IntCondition: public BehaviorTreeNode
	{
	public:
		int (T::*func)();
		int (*func2)();
		INT_TEST test;
		int val;
		BEHAVIOR_STATUS execute(void* agent)
		{
			T* obj = (T*) agent;
			bool status;
			switch (test)
			{
				case LESS_THAN:		status = ((obj->*func)() < val); break;
				case GREATER_THAN:	status = ((obj->*func)() > val); break;
				case LESS_OR_EQ:	status = ((obj->*func)() <= val); break;
				case GREATER_OR_EQ: status = ((obj->*func)() >= val); break;
				case EQUAL:			status = ((obj->*func)() == val); break;
				case NOT_EQUAL:		status = ((obj->*func)() != val); break;
			}

			if (status)
				return BT_SUCCESS;
			else
				return BT_FAILURE;
		};

		void init(void* agent)
		{
		};
		IntCondition(int(T::*_func)(), INT_TEST _test, int _val)
		{
			func = _func;
			test = _test;
			val = _val;
		}
		IntCondition(int(*_func)(), INT_TEST _test, int _val)
		{
			func2 = _func;
			test = _test;
			val = _val;
		}
	};

	template<>
	BEHAVIOR_STATUS IntCondition<NoClass>::execute(void* agent)
	{
		bool status;
		switch (test)
		{
		case LESS_THAN:		status = ((*func2)() < val); break;
		case GREATER_THAN:	status = ((*func2)() > val); break;
		case LESS_OR_EQ:	status = ((*func2)() <= val); break;
		case GREATER_OR_EQ: status = ((*func2)() >= val); break;
		case EQUAL:			status = ((*func2)() == val); break;
		case NOT_EQUAL:		status = ((*func2)() != val); break;
		}

		if (status)
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
}

#endif
#include "Common.h"
#include <vector>
#include <map>
#include "boost\\bind.hpp"
namespace BehaviorTree
{
	template <class T = NoClass>
	class BoolCondition: public BehaviorTreeNode
	{
	public:
		bool (T::*func)();
		bool (*func2)();
		bool check;
		BEHAVIOR_STATUS execute(void* agent)
		{
			T* obj = (T*) agent;
			if ((obj->*func)() == check)
				return BT_SUCCESS;
			else
				return BT_FAILURE;
		};

		void init(void* agent)
		{
		};
		BoolCondition(bool(T::*_func)(), bool _check)
		{
			func = _func;
			check = _check;
		}
		BoolCondition(bool(*_func)(), bool _check)
		{
			func2 = _func;
			check = _check;
		}
	};

	template<>
	BEHAVIOR_STATUS BoolCondition<NoClass>::execute(void* agent)
	{
		if ((*func2)() == check)
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
}
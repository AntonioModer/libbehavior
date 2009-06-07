#include <vector>
#include <map>
#include "boost\\bind.hpp"
#include "Common.h"
#include "FPCompare.h"
namespace BehaviorTree
{
	enum FLOAT_TEST {LESS_THAN_FP,GREATER_THAN_FP,LESS_OR_CLOSE,GREATER_OR_CLOSE,CLOSE,NOT_CLOSE};
	template <class T = NoClass>
	class FloatCondition: public BehaviorTreeNode
	{
	public:
		float (T::*func)();
		float (*func2)();
		FLOAT_TEST test;
		float val;
		int ulps;
		BEHAVIOR_STATUS execute(void* agent)
		{
			float objVal = getObjVal(agent);
			bool status;
			switch (test)
			{
			case LESS_THAN_FP:		status = (objVal < val) && !AlmostEqual2sComplement(objVal,val,ulps); break;
			case GREATER_THAN_FP:	status = (objVal > val) && !AlmostEqual2sComplement(objVal,val,ulps); break;
			case LESS_OR_CLOSE:		status = (objVal <= val)|| AlmostEqual2sComplement(objVal,val,ulps); break;
			case GREATER_OR_CLOSE:	status = (objVal >= val)|| AlmostEqual2sComplement(objVal,val,ulps); break;
			case CLOSE:				status = AlmostEqual2sComplement(objVal,val,ulps); break;
			case NOT_CLOSE:			status = !AlmostEqual2sComplement(objVal,val,ulps); break;
			}

			if (status)
				return BT_SUCCESS;
			else
				return BT_FAILURE;
		};

		void init(void* agent)
		{
		};
		FloatCondition(float(T::*_func)(), FLOAT_TEST _test, float _val,int _ulps = 2^25)
		{
			func = _func;
			test = _test;
			val = _val;
			ulps = _ulps;

		}
		FloatCondition(float(*_func)(), FLOAT_TEST _test, float _val,int _ulps = 2^25)
		{
			func2 = _func;
			test = _test;
			val = _val;
			ulps = _ulps;
		}
	private:
		float getObjVal(void* agent)
		{
			T* obj = (T*) agent;
			return (obj->*func)();
		}
	};

	template<>
	float FloatCondition<NoClass>::getObjVal(void* agent)
	{
		return (*func2)();
	}
}
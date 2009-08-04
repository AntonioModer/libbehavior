#include "Common.h"
namespace BehaviorTree
{
	template <class T = NoClass>
	/// Wraps a function or member pointer into a Behavior Tree Node.
	/** To wrap a void function pointer or static class member that takes no arguments , instantiate FunctionCall without a type argument.
		For example, to wrap the function "func", with the signature "void func()", one would do this:
		FunctionCall<> functionNode(&func);

		To wrap the static method "sMethod" of the class "Class", one would do this:
		FunctionCall<> staticMethodNode(&Class::sMethod);

		To wrap a non-static class member, FunctionCall must be parameterized with the class's type.
		For example, to wrap the method "method" of class "Class", one would do this:
		FunctionCall<Class> classMethodNode(&Class:method);
	**/
	class FunctionCall: public BehaviorTreeNode
	{
	public:
		BEHAVIOR_STATUS execute(void* agent)
		{
			T* obj = (T*) agent;
			(obj->*func)();
		};

		void init(void* agent){};

		/** \param _func the address of the (non-static) class member
		*/
		FunctionCall(void(T::*_func)()) : func(_func), func2(NULL){}

		/** \param _func the address of the function or the static class member
		*/
		FunctionCall(void(*_func)()) : func2(_func), func(NULL){}

		/** \param _func the address of the const non-static class member
		*/
		FunctionCall(void(T::* const _func)() const) : func(reinterpret_cast<void(T::* const)()>(_func)), func2(NULL){}

	private:
		void (T::* const func)();
		void (* const func2)();
	};

	template<>
	BEHAVIOR_STATUS FunctionCall<NoClass>::execute(void* agent)
	{
		(*func2)();
	}
}
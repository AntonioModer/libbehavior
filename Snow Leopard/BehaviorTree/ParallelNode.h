namespace BehaviorTree
{
///Execute behaviors in parallel
/** There are two policies that control the flow of execution. The first is the policy for failure, and the second is the policy for success.
For failure, the options are "fail when one child fails" and "fail when all children fail".
For success, the options are similarly "complete when one child completes", and "complete when all children complete".
*/
class ParallelNode:public BehaviorTreeInternalNode
{
public:
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);
	void setFailurePolicy(FAILURE_POLICY policy);
	void setSuccessPolicy(SUCCESS_POLICY policy);
	FAILURE_POLICY getFailurePolicy();
	SUCCESS_POLICY getSuccessPolicy();
	ParallelNode::ParallelNode( FAILURE_POLICY failurePolicy = FAIL_ON_ALL, SUCCESS_POLICY SuccessPolicy = SUCCEED_ON_ALL);

private:
	typedef std::map<BehaviorTreeNode*,BEHAVIOR_STATUS> ChildrenStatusMap;
	ChildrenStatusMap* childrenStatus;
	FAILURE_POLICY failPolicy;
	SUCCESS_POLICY succeedPolicy;
};
}
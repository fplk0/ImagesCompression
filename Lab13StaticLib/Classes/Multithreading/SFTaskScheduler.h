#pragma once
#include "SFObject.h"

#include <Windows.h>
#include <set>
#include <map>
#include <queue>

#include "SFTask.h"

using namespace std;

#undef super
#undef self
#define self SFTaskScheduler
#define super SFObject

class SFTaskScheduler :
	public SFObject
{
	DEFAULT_METACLASS_DECLARATION(self, super);

	int maxConcurrentTasksCount;

	int nextTaskIndex;

	map<int, HANDLE> semaphoresForTasks;
	HANDLE taskSchedulerMutex;
	HANDLE taskSchedulerSemaphore;

	map<int, int> taskCountsForTasks;

	struct TaskWithType
	{
		TaskWithType(SFTask *_task = NULL, int _type = 0)
		{
			task = _task;
			type = _type;
		};
		SFTask *task;
		int type;
	};

	queue<TaskWithType> tasksQueue;

	int currentThreadsCount;
	//set<HANDLE> threadsPool;

	static DWORD WINAPI threadProcessor(void *param);
private:
	void _taskProcessor();

public:
	
	int nextTasksType();
	void addTask(SFTask *task, int taskTypeIndex);
	void waitForTaskFinish(int taskTypeIndex);
	void closeTaskType(int taskTypeIndex);

	static SFTaskScheduler* singleInstance();

	virtual SFTaskScheduler* initWithMaxThreadsCount(int maxThreadsCount);

	virtual ~SFTaskScheduler(void);
};


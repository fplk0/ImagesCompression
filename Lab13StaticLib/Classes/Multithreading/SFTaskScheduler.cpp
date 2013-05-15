#include "SFTaskScheduler.h"
#include "Atomic.h"

DEFINE_OBJECT_IMPL(self);

SFTaskScheduler* SFTaskScheduler::singleInstance()
{
	int cpuCount = 8;
	static SFTaskScheduler *singleton = NULL;
	SFPerformOnceBegin
	{
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );

		cpuCount = sysinfo.dwNumberOfProcessors;
		cpuCount = 8;
		printf("%d\n\n", cpuCount);

		singleton = SFTaskScheduler::alloc()->initWithMaxThreadsCount(cpuCount);
	}
	SFPerformOnceEnd

	return singleton;
}

SFTaskScheduler* SFTaskScheduler::initWithMaxThreadsCount(int maxThreadsCount)
{
	super::init();

	taskSchedulerMutex = CreateMutex(NULL, 0, NULL);
	taskSchedulerSemaphore = CreateSemaphore(NULL, 0, 1e9, NULL);
	maxConcurrentTasksCount = maxThreadsCount;
	nextTaskIndex = 0;
	currentThreadsCount = 0;

	return this;
}

int SFTaskScheduler::nextTasksType()
{
	WaitForSingleObject(taskSchedulerMutex, INFINITE);

	int retVal = nextTaskIndex;
	nextTaskIndex++;

	HANDLE semaphoreForTask = CreateSemaphore(NULL, 1, 1, NULL);
	semaphoresForTasks[retVal] = semaphoreForTask;
	taskCountsForTasks[retVal] = 0;

	ReleaseMutex(taskSchedulerMutex);
	return retVal;
}

void SFTaskScheduler::addTask(SFTask *task, int taskTypeIndex)
{
	WaitForSingleObject(taskSchedulerMutex, INFINITE);

	if (taskTypeIndex != -1)
	{
		if (taskCountsForTasks[taskTypeIndex] == 0)
		{
			HANDLE semaphoreHandle = semaphoresForTasks[taskTypeIndex];
			WaitForSingleObject(semaphoreHandle, INFINITE);
		}
		taskCountsForTasks[taskTypeIndex]++;
	}

	task->retain();
	tasksQueue.push(TaskWithType(task, taskTypeIndex));
	ReleaseSemaphore(taskSchedulerSemaphore, 1, NULL);

	if (currentThreadsCount < maxConcurrentTasksCount && tasksQueue.size() > currentThreadsCount)
	{
		currentThreadsCount++;
		HANDLE newThreadHandle = CreateThread(NULL, 0, threadProcessor, this, 0, 0);
		CloseHandle(newThreadHandle);
	}

	ReleaseMutex(taskSchedulerMutex);
}

DWORD SFTaskScheduler::threadProcessor(void *param)
{
	SFTaskScheduler *ts = (SFTaskScheduler*)param;
	ts->_taskProcessor();

	return 0;
}

void SFTaskScheduler::_taskProcessor()
{
	while (1)
	{
		DWORD waitRes = WaitForSingleObject(taskSchedulerSemaphore, 2000);
		if (waitRes == WAIT_TIMEOUT)
		{
			WaitForSingleObject(taskSchedulerMutex, INFINITE);
			waitRes = WaitForSingleObject(taskSchedulerSemaphore, 0);
			if (waitRes == WAIT_TIMEOUT)
			{
				currentThreadsCount--;
			}
			ReleaseMutex(taskSchedulerMutex);

			if (waitRes == WAIT_TIMEOUT)
				break;
		}

		if (waitRes != WAIT_OBJECT_0)
			continue;

		WaitForSingleObject(taskSchedulerMutex, INFINITE);

		TaskWithType curTask = tasksQueue.front();
		tasksQueue.pop();

		ReleaseMutex(taskSchedulerMutex);

		curTask.task->runTask();
		curTask.task->release();

		if (curTask.type != -1)
		{
			WaitForSingleObject(taskSchedulerMutex, INFINITE);

			taskCountsForTasks[curTask.type]--;

			if (taskCountsForTasks[curTask.type] == 0)
			{
				HANDLE semaphoreHandle = semaphoresForTasks[curTask.type];
				ReleaseSemaphore(semaphoreHandle, 1, NULL);
			}

			ReleaseMutex(taskSchedulerMutex);
		}
	}
}

void SFTaskScheduler::waitForTaskFinish(int taskTypeIndex)
{
	WaitForSingleObject(taskSchedulerMutex, INFINITE);

	HANDLE semaphoreHandle = semaphoresForTasks[taskTypeIndex];

	ReleaseMutex(taskSchedulerMutex);

	if (semaphoreHandle != NULL)
	{
		WaitForSingleObject(semaphoreHandle, INFINITE);
	}
	closeTaskType(taskTypeIndex);
}

void SFTaskScheduler::closeTaskType(int taskTypeIndex)
{
	WaitForSingleObject(taskSchedulerMutex, INFINITE);

	HANDLE semaphoreHandle = semaphoresForTasks[taskTypeIndex];
	CloseHandle(semaphoreHandle);

	semaphoresForTasks.erase(taskTypeIndex);
	taskCountsForTasks.erase(taskTypeIndex);

	ReleaseMutex(taskSchedulerMutex);
}

SFTaskScheduler::~SFTaskScheduler(void)
{

}

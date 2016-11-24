#pragma once
#include <thread>
#include <queue>
#include <SDL_mutex.h>
#include <SDL_thread.h>
#include <vector>
#include <functional>

class TaskQueue
{
public:
	TaskQueue();
	SDL_mutex * getLock();
	void spawnWorkers();
	static TaskQueue * getInstance();
	SDL_cond * canConsume();
	std::function<void()> consumeJob();

	//TaskQueue::addJob(std::bind(&Some_Class::Some_Method, &Some_object));
	void addJob(std::function<void()> f);

private:
	static TaskQueue * m_instance;
	SDL_mutex* m_queueLock;
	SDL_cond* m_canConsume;
	std::queue<std::function<void()>> m_jobs;
	std::vector<SDL_Thread*> m_workerPool;
};

static int worker(void* ptr)
{
	TaskQueue * taskQueue = TaskQueue::getInstance();
	SDL_mutex * lock = taskQueue->getLock();
	SDL_cond * canConsume = taskQueue->canConsume();
	while (true)
	{
		SDL_LockMutex(lock);
		SDL_CondWait(canConsume, lock);
		auto& Job = taskQueue->consumeJob();
		SDL_UnlockMutex(lock);

		Job(); // function<void()> type
	}
}
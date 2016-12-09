#pragma once
#include <thread>
#include <queue>
#include <SDL_mutex.h>
#include <SDL_thread.h>
#include <vector>
#include <functional>
#include <iostream>

class TaskQueue
{
public:
	TaskQueue();
	SDL_mutex * getLock();
	void spawnWorkers();
	static TaskQueue * getInstance();
	SDL_sem * canConsume();
	std::function<void()> consumeJob();

	//TaskQueue::addJob(std::bind(&Some_Class::Some_Method, &Some_object));
	void addJob(std::function<void()> f);

private:
	static TaskQueue * m_instance;
	SDL_mutex* m_queueLock, * m_consumeLock;
	SDL_sem* m_canConsume;
	std::queue<std::function<void()>> m_jobs;
	std::vector<SDL_Thread*> m_workerPool;
};

static int worker(void* ptr)
{
	srand(0);
	TaskQueue * taskQueue = TaskQueue::getInstance();
	SDL_mutex * lock = taskQueue->getLock();
	SDL_sem * canConsume = taskQueue->canConsume();
	while (true)
	{
		SDL_SemWait(canConsume);
		auto& Job = taskQueue->consumeJob();

		try
		{
			Job(); // function<void()> type
		}
		catch (...)
		{
			std::cout << "EARLY RETURN" << std::endl;
		}
	}
}
#include "stdafx.h"
#include "TaskQueue.h"

TaskQueue * TaskQueue::m_instance = nullptr;

TaskQueue::TaskQueue() : m_canConsume(SDL_CreateCond()), m_queueLock(SDL_CreateMutex())
{
}

SDL_mutex * TaskQueue::getLock()
{
	return m_queueLock;
}

void TaskQueue::spawnWorkers()
{
	int numWorkers = std::thread::hardware_concurrency() - 1;

	for (int i = 0; i < numWorkers; i++)
	{
		m_workerPool.push_back(SDL_CreateThread(worker, "GenericWorker", (void*)NULL));
	}
}

TaskQueue * TaskQueue::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new TaskQueue();
	}
	return m_instance;
}

SDL_cond * TaskQueue::canConsume()
{
	return m_canConsume;
}

std::function<void()> TaskQueue::consumeJob()
{
	SDL_LockMutex(m_queueLock);
	std::function<void()> job = m_jobs.front();
	m_jobs.pop();
	SDL_UnlockMutex(m_queueLock);
	return job;
}

void TaskQueue::addJob(std::function<void()> f)
{
	SDL_LockMutex(m_queueLock);
	m_jobs.push(f);
	SDL_UnlockMutex(m_queueLock);
	SDL_CondSignal(m_canConsume);
}
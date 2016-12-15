#include "stdafx.h"
#include "TaskQueue.h"

TaskQueue * TaskQueue::m_instance = nullptr;

TaskQueue::TaskQueue() : m_canConsume(SDL_CreateSemaphore(0)), m_queueLock(SDL_CreateMutex()), m_resultsLock(SDL_CreateMutex()), m_threadingEnabled(true)
{
}

SDL_mutex * TaskQueue::getLock()
{
	return m_queueLock;
}

void TaskQueue::spawnWorkers()
{
	int numWorkers = std::thread::hardware_concurrency() -1;

	for (int i = 0; i < numWorkers; i++)
	{
		m_workerPool.push_back(SDL_CreateThread(worker, "GenericWorker", (void*)new int(i + 1)));
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

SDL_sem * TaskQueue::canConsume()
{
	return m_canConsume;
}

std::pair<int, std::function<void*()>> TaskQueue::consumeJob()
{
	SDL_LockMutex(m_queueLock);
	if (m_jobs.empty()) //if asking for deleted task, return a dummy task
	{
		return std::make_pair(-1, std::function<void*()>());
	}
	auto job = m_jobs.front();
	m_jobs.pop_front();
	SDL_UnlockMutex(m_queueLock);
	return job;
}

void TaskQueue::storeResults(int jobId, void * results)
{
	SDL_LockMutex(m_resultsLock);
	m_jobResults[jobId] = results;
	SDL_UnlockMutex(m_resultsLock);
}

void TaskQueue::deleteResults(int jobId)
{
	SDL_LockMutex(m_resultsLock);
	auto& it = m_jobResults.find(jobId);
	if (it != m_jobResults.end())
	{
		m_jobResults.erase(it);
	}
	SDL_UnlockMutex(m_resultsLock);
}

int TaskQueue::addJob(std::function<void*()> f)
{
	int jobId = m_numJobs;
	SDL_LockMutex(m_queueLock);
	m_jobs.push_back(std::make_pair(jobId, f));
	m_numJobs++;
	SDL_UnlockMutex(m_queueLock);
	SDL_SemPost(m_canConsume);

	return jobId;
}

void* TaskQueue::getResults(int jobId)
{
	SDL_LockMutex(m_resultsLock);
	auto& it = m_jobResults.find(jobId);
	void* result = nullptr;
	if (it != m_jobResults.end())
	{
		result = (*it).second;
	}
	SDL_UnlockMutex(m_resultsLock);
	return result;
}

bool TaskQueue::jobFinished(int jobId)
{
	void *  p_results = TaskQueue::getInstance()->getResults(jobId);
	if (p_results == nullptr)
	{
		return false;
	}
	return true;
}

void TaskQueue::removeJobById(int jobId)
{
	//check if still on queue and execution hasn't started yet
	auto it = std::find_if(m_jobs.begin(), m_jobs.end(), [&](const std::pair<int, std::function<void*()>>& e) { return e.first == jobId; });
	if (it != m_jobs.end())
	{
		m_jobs.erase(it); //remove from queue

	}
}

void TaskQueue::runTasksSequentially()
{
	if (!m_threadingEnabled)
	{
		while (!m_jobs.empty())
		{
			auto& jobId_Job = consumeJob();
			int jobId = jobId_Job.first;
			auto job = jobId_Job.second;

			void* result = (void*)job();
			storeResults(jobId, result);
		}
	}
}

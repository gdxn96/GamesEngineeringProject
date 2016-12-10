#pragma once
#include <thread>
#include <queue>
#include <SDL_mutex.h>
#include <SDL_thread.h>
#include <vector>
#include <functional>
#include <iostream>
#include <unordered_map>

class TaskQueue
{
private:
	void* getResults(int jobId);
	static TaskQueue * m_instance;
	SDL_mutex* m_queueLock, *m_resultsLock;
	SDL_sem* m_canConsume;
	std::deque<std::pair<int, std::function<void*()>>> m_jobs;
	std::vector<SDL_Thread*> m_workerPool;
	int m_numJobs;

public:
	TaskQueue();
	SDL_mutex * getLock();
	void spawnWorkers();
	static TaskQueue * getInstance();
	SDL_sem * canConsume();
	std::pair<int,std::function<void*()>> consumeJob();
	std::unordered_map<int, void*> m_jobResults;
	void storeResults(int jobId, void* results);
	void deleteResults(int jobId);
	bool jobFinished(int jobId);
	void removeJobById(int jobId);

	//*TaskQueue::addJob(std::bind(&Some_Class::Some_Method, &Some_object, ...Args)); 
	//*Function must return a pointer and is assumed to be threadsafe
	int addJob(std::function<void*()> f);


	template<typename T>
	T getJobResults(int jobId)
	{
		T * p_results = (T*)TaskQueue::getInstance()->getResults(jobId);
		T result = *p_results;
		TaskQueue::getInstance()->deleteResults(jobId);
		delete p_results;
		return result;
	}
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
		auto& jobId_Job = taskQueue->consumeJob();
		int jobId = jobId_Job.first;
		auto job = jobId_Job.second;
		try
		{
			void* result = (void*)job();
			taskQueue->storeResults(jobId, result);
		}
		catch (...)
		{
			std::cout << "EARLY RETURN" << std::endl;
		}
	}
}
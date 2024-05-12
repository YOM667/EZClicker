#pragma once

#include <thread>
#include <iostream>
class Thread
{
public:
	Thread() : running(false), thread(nullptr)
	{

	}
	~Thread() 
	{
		if (thread != NULL)
		{
			if (thread->joinable())
			{
				std::cout << "thread detach" << std::endl;
				thread->detach();
			}
			delete thread;
			thread = nullptr;
		}
	}
	void start()
	{
		try
		{
			thread = new std::thread(&Thread::threadEntry, this);
		}
		catch (...)
		{
			throw "thread start error";
		}

	}
	bool isAlive() const
	{
		return running;
	}
	void join()
	{
		if (thread->joinable())
		{
			thread->join();
		}
	}
	void detach()
	{
		thread->detach();
	}
	void threadEntry()
	{
		running = true;
		try
		{
			run();
		}
		catch (std::exception& ex)
		{
			running = false;
			throw ex;
		}
		catch (...)
		{
			running = false;
			throw;
		}
		running = false;
	}
protected:
	virtual void run() = 0;
protected:
	bool running;
	std::thread* thread;
};
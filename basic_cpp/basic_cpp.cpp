#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include <mutex>

#include <csignal>

#include "core.h"


// ----------------------------------------------------------------------------------------------- //

#define NUMBER_OF_THREADS 3
#define MIN_SLEEP_TIME 1
#define MAX_SLEEP_TIME 5

// ----------------------------------------------------------------------------------------------- //

typedef std::vector<std::thread> ThreadPool;
typedef typename on::core::lock_guard< std::mutex> LockGuardMtxType;

// ----------------------------------------------------------------------------------------------- //

//time related
std::random_device randDev;
std::uniform_int_distribution<> dist(MIN_SLEEP_TIME, MAX_SLEEP_TIME);


std::atomic_bool m_stop{ false };
std::atomic_bool m_wakeup{ false };
int m_threadToWakeup;
std::mutex m_printMtx;

std::mutex m_threadMtx;
std::condition_variable m_startTimeFlag;

ThreadPool threadPool;

// ----------------------------------------------------------------------------------------------- //

void int_signal_handler(int signal_num)
{
	m_stop = true;
	m_startTimeFlag.notify_all();
	for (auto it = threadPool.begin(); it != threadPool.end(); ++it) {
		it->join();
	}
	exit( signal_num );
}

// ----------------------------------------------------------------------------------------------- //

///@brief output message to console, output is synchronized
void output_to_console(const std::string &str) {
	LockGuardMtxType LockGuard(m_printMtx);
	std::cout << str << std::endl;
}

// ----------------------------------------------------------------------------------------------- //

///@brief spawn working threads
void spawnThreads( void )
{

	for (int i = 1; i <= NUMBER_OF_THREADS; ++i)
	{
		//capture the thread id by value for each thread
		threadPool.emplace_back([i]() {

			std::string threadIdStr{ "thread" + std::to_string(i) };
			std::string startingStr{ threadIdStr + ": starting, waiting." };
			std::string workingStr{ threadIdStr + ": signal received, doing work..." };
			std::string doneStr{ threadIdStr + ": done with work, signal next thread." };

			output_to_console( startingStr );
			std::unique_lock< std::mutex> threadLck(m_threadMtx);

			while (!m_stop)
			{
				m_startTimeFlag.wait(
					threadLck, 
					[&]() {
						//avoid spurious wakeups
						return m_threadToWakeup == i || m_stop == true;
					}
				);

				if (m_stop)
				{
					break;
				}

				output_to_console( workingStr );
				std::this_thread::sleep_for(std::chrono::seconds{ dist(randDev) });
				output_to_console( doneStr );

				//next thread to run
				m_threadToWakeup++;
				m_threadToWakeup = (m_threadToWakeup > NUMBER_OF_THREADS) ? 1 : m_threadToWakeup;
				
				m_startTimeFlag.notify_one();
			}
		}
		);
	}

	std::this_thread::sleep_for(std::chrono::seconds{ 1 });
}

// ----------------------------------------------------------------------------------------------- //

int main(void)
{
	//signal(SIGINT, int_signal_handler);

	output_to_console( "main: starting all threads.");
	spawnThreads();

	output_to_console( "main: starting thread 1." );
	m_threadToWakeup = 1;
	m_startTimeFlag.notify_one();

	for (auto it = threadPool.begin(); it != threadPool.end(); ++it) {
		it->join();
	}

	return 0;
}

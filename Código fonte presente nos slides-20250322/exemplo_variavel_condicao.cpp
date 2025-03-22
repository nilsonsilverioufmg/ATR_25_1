#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

std::mutex m;
std::condition_variable cv;
bool first_thread_printed = false;

void f()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	{
		std::unique_lock<std::mutex>  lock(m);
		std::cout << "First Thread - Ok" << std::endl;
		first_thread_printed = true;
		cv.notify_all();
	}
}

void g(int id)
{
	std::unique_lock<std::mutex> lock(m);
	while (!first_thread_printed)
	{
		std::cout << "Thread " << id << " - Waiting..." << std::endl;
		cv.wait(lock);
	}
	std::cout << "Thread " << id << " - Ok" <<  std::endl;
}

int main()
{
	std::thread first_thread(f);

	std::vector<std::thread> other_threads;
	for (unsigned i =0; i < 5; ++i)
	{
		other_threads.push_back(std::thread(g, i+1));
	}
	first_thread.join();
	for (unsigned i =0; i < 5; ++i)
	{
		other_threads[i].join();
	}
	return(0);

}

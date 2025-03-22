// Solução para o problema de Race condition usando um mutex
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <mutex>
#include <thread>

static const int NUMTHREADS = 2;
static const int N = 10000;

int counter;

std::mutex m;

void increment()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	for (int i=0; i< N; ++i)
	{
		// Protocolo de entrada
		m.lock();

		counter++;

		// Protocolo de saida
		m.unlock();
	}
}

void decrement()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	for (int i=0; i< N; ++i)
	{
		// Protocolo de entrada
		m.lock();

		counter--;

		// Protocolo de saida
		m.unlock();
	}
}


int main()
{
	std::vector<std::thread> threads;

	counter = 0;

	for (int i=0; i < NUMTHREADS; ++i)
	{
		if (i%2==0)
			threads.push_back(std::thread(increment));
		else
			threads.push_back(std::thread(decrement));
	}
	for (int i=0; i< NUMTHREADS; ++i)
	{
		threads[i].join();
	}
	std::cout << "Counter: " << counter << std::endl;
	return(0);
}

// Solução para o problema de Race condition usando instrução atômica
#include <thread>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <atomic>

static const int NUMTHREADS = 4;
static const int N = 10000000;

int counter;

std::atomic_flag lock_stream = ATOMIC_FLAG_INIT;

void increment()
{
	for (int i=0; i< N; ++i)
	{
		// Protocolo de entrada
		while (lock_stream.test_and_set());

		counter++;

		// Protocolo de saida
		lock_stream.clear();
	}
}

void decrement()
{
	for (int i=0; i< N; ++i)
	{
		// Protocolo de entrada
		while (lock_stream.test_and_set());

		counter--;

		// Protocolo de saida
		lock_stream.clear();
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

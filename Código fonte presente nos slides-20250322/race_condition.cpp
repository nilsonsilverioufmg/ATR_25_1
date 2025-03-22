/*	Exemplo de Race Condition
	=========================
	Varias threads são criadas e executam um loop (for)
	incrementando e decrementando a variável compartilhada "counter"
	O valor esperado para "counter" no final do programa seria 0 (seu valor inicial)
*/
#include <thread>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <mutex>

static const int NUMTHREADS = 4;
static const int N = 1000000;

static int counter;

void increment()
{
	for (int i=0; i< N; ++i)
	{
		counter++;
	}
}

void decrement()
{
	for (int i=0; i< N; ++i)
	{
		counter--;
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

// Solução para o problema de Race condition usando um mutex
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <thread>
#include <atomic>


int counter;

std::atomic<int> turn;
volatile bool p1wantstoenter = false;
volatile bool p2wantstoenter = false;

int N = 100000;

void p1()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	for (int i=0; i<N; ++i)
	{
		// Protocolo de Entrada
		p1wantstoenter = true;
		turn = 2; 
		while (p2wantstoenter && turn == 2);

		counter++; // Seção crítica

		// Protocolo de saída
		p1wantstoenter = false;
	}
}

void p2()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	for (int i=0; i<N; ++i)
	{
		// Protocolo de Entrada
		p2wantstoenter = true;
		turn = 1; 
		while (p1wantstoenter && turn == 1);

		counter--; // Seção crítica

		// Protocolo de saída
		p2wantstoenter = false;
	}
}


int main()
{
	counter = 0;
	std::thread t1(p1);
	std::thread t2(p2);
	t1.join();
	t2.join();
	std::cout << "Counter: " << counter << std::endl;
	return(0);
}

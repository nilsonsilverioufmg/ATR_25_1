// Solução do problema do produtor consumidor utilizando variáveis de condição

#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <chrono>
#include <array>
#include <vector>
#include <cassert>
#include "semaforo.hpp"


// Numero de threads produtoras e consumidoras a serem criadas no main()
static const unsigned NUM_PRODUCERS = 1;
static const unsigned NUM_CONSUMERS = 1;


//  =========================  Circular buffer  ============================================
// Código que implementa um buffer circular
static const unsigned BUFFER_SIZE = 10;
static std::array<int, BUFFER_SIZE> buffer;
static int counter = 0;
static unsigned in = 0, out = 0;

void add_buffer(int i)
{
  buffer[in] = i;
  in = (in+1) % BUFFER_SIZE;
  counter++;
}

int get_buffer()
{
  int v;
  v = buffer[out];
  out = (out+1) % BUFFER_SIZE;
  counter--;
  return v ;
}
//  ==========================================================================================


static std::mutex m;
static semaforo full_pos(0);
static semaforo empty_pos(BUFFER_SIZE);


// Altere esse valor caso queira que o produtor e consumidor durmam SLEEP_TIME milissegundos
// após cada iteração 
static const unsigned SLEEP_TIME = 500; //ms 

// Producer
void producer_func(const unsigned id)
{
	int i = 100;
	while (true)		
	{
		// Decrementa o semaforo que controla o número de posições vazias no buffer
		// (verifica se o buffer esta cheio e, caso esteja, espera por notificação de espaço disponível)
		empty_pos.wait();
		
		// O buffer não está mais cheio, então produz um elemento			
		{
			std::lock_guard<std::mutex> lock(m); // m.lock é invocado no construtor de lock
			add_buffer(i);
			std::cout << "Producer " << id << " - Produced: " << i << " - Buffer size: " << counter << std::endl;
		}	// Fim de escopo lock é destruído e m.unlock é invocado
		i++;
	
		// Incrementa o semafro que controla o número de dados disponíveis no buffer
		// (caso o buffer esteja vazio e existam um ou mais consumidores esperando, um consumidor será acordado)
		//
		{
			std::lock_guard<std::mutex> lock(m); // m.lock é invocado no construtor de lock
			std::cout << "Producer " << id << " - Notifying that there is data available" << std::endl;
		}

		full_pos.signal();
		
		// (Opicional) dorme por SLEEP_TIME milissegundos
		if (SLEEP_TIME > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

		// Verifica a integridade do buffer, ou seja, verifica se o número de elementos do buffer (size)
		// é menor ou igual a BUFFER_SIZE
		assert(counter <= BUFFER_SIZE);		
	} 
}

// Consumer
void consumer_func(const unsigned id)
{
	while (true)
	{
		// Decrementa o semaforo que controla o número de posições cheias no buffer
		// (verifica se o buffer está vazio e, caso esteja, espera por notificação de dado disponível)
		full_pos.wait();

		// O buffer não está mais vazio, então consome um elemento
		{
			std::lock_guard<std::mutex> lock(m); // m.lock é invocado no construtor de lock
			int i = get_buffer();
			std::cout << "Consumer " << id << " - Consumed: " << i << " - Buffer size: " << counter << std::endl;
		}  // Fim de escopo lock é destruído e m.unlock é invocado
	
		// Incrementa o semaforo que controla o número de posições livres no buffer
		// (caso o buffer esteja cheio e existam um ou mais produtores esperando, um  produtor será acordado)
		{
			std::lock_guard<std::mutex> lock(m); // m.lock é invocado no construtor de lock
			std::cout << "Consumer " << id << " - Notifying that there is a free position" << std::endl;
		}

		empty_pos.signal();		


		// (Opicional) dorme por SLEEP_TIME milissegundo
		if (SLEEP_TIME > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

		// Verifica a integridade do buffer, ou seja, verifica se o número de elementos do buffer (size)
		// é maior ou igual a zero
		assert(counter >= 0);
	}  
}

int main()
{
	// Cria NUM_PRODUCER thread produtoras e NUM_CONSUMER threads consumidoras
	std::vector<std::thread> producers;
	std::vector<std::thread> consumers;

	for (unsigned i =0; i < NUM_PRODUCERS; ++i)
	{
		producers.push_back(std::thread(producer_func, i));
	}
	for (unsigned i =0; i < NUM_CONSUMERS; ++i)
	{
		consumers.push_back(std::thread(consumer_func, i));
	}

	consumers[0].join();
}

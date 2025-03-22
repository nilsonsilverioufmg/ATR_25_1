#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <algorithm>

static const int NUM_ASSENTOS = 30;
static const int NUM_CLIENTES = 10000;

std::vector<int> assento_reservado(NUM_CLIENTES, -1);

int sample_random_integer(int N) {
    // Create a random device and seed the generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define a uniform integer distribution from 0 to N
    std::uniform_int_distribution<> dist(0, N);
    
    // Sample a random integer
    return dist(gen);
}

void reserva_assento(const int id, std::vector<bool>& assentos) {
	for (int i = 0; i < 100; ++i) {

    int num_cycles = sample_random_integer(100000);
    for (int j = 0; j < num_cycles; ++j); 

	  int candidato = sample_random_integer(NUM_ASSENTOS-1);
		if (assentos[candidato] == false) {
			assentos[candidato] = true;
      assento_reservado[id] = candidato;
      std::cout << "cliente " << id << " - assento: " << candidato << std::endl;
      break;
		}
	}
}

int main() {
	std::vector<bool> assentos(NUM_ASSENTOS, false);
	std::vector<std::thread> clientes;

	for (int i = 0; i < NUM_CLIENTES; ++i) {
		clientes.emplace_back(reserva_assento,i, std::ref(assentos));
	}
	
	for (int i = 0; i < NUM_CLIENTES; ++i) {
		clientes[i].join();
	}

  std::sort(assento_reservado.begin(), assento_reservado.end());
  for (int i = 0; i < NUM_CLIENTES; ++i) {
    if (assento_reservado[i] != -1)
      std::cout << "cliente " << i << " - assento: " << assento_reservado[i] << std::endl;
  }

	return 0;
}

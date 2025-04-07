#include <iostream>
#include <fstream>
#include <stack>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

bool retorno_walk = false;
int contador = 0;

int tempo_intervalo = 250;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	 
	FILE *arquivo;
	int result, i;
	char Str[50];

	arquivo = fopen(file_name, "r");

	if (arquivo == NULL)
	{
		printf("Problemas na CRIACAO do arquivo\n");
	}else{
		printf("Maze.txt aberto\n");
	}

	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols

	fscanf(arquivo, "%d %d", &num_rows, &num_cols);

	printf("colunas: %d linhas %d\n", num_cols, num_rows);

	maze = (char**)malloc(num_rows * sizeof(char*));
    for (i = 0; i < num_rows; ++i) {
        maze[i] = (char*)malloc(num_cols * sizeof(char));
    }
	
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			fscanf(arquivo, " %c", &maze[i][j]);
			if(maze[i][j] == 'e'){
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
	}

	return initial_pos;
}

int verificar_o_em_matriz(){
	contador = 0;

	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			if(maze[i][j] == 'o') contador++;
		}
	}
	return contador;
}


// Função que imprime o labirinto
void print_maze() {
	while(true){
		contador = 0;

		for (int i = 0; i < num_rows; ++i) {
			for (int j = 0; j < num_cols; ++j) {
				if(maze[i][j] == 'o') contador++;
				printf("%c", maze[i][j]);
			}
			printf("\n");
		}
		//printf("\ncontador: %d\n", contador);
		//printf("retorno: %d\n", retorno_walk);

		for(int k = 0; k < 4; k++){
			contador = verificar_o_em_matriz();
		}

		if(retorno_walk) return;
		if(contador == 0 && retorno_walk == false) return;

		std::this_thread::sleep_for(std::chrono::milliseconds(tempo_intervalo));
		system("clear");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
void walk(pos_t pos) {
	if(retorno_walk){
		return;
	}

	maze[pos.i][pos.j] = 'o';
	std::this_thread::sleep_for(std::chrono::milliseconds(tempo_intervalo));
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
		
		if(pos.i >= 0 && pos.i < num_rows && pos.j >= 0 && pos.j < num_cols){
			if(pos.j + 1 < num_cols){
				if(maze[pos.i][pos.j+1] == 's'){
					retorno_walk = true;
				}else if(maze[pos.i][pos.j + 1] == 'x'){
					pos_t next_pos;
					next_pos.i = pos.i;
					next_pos.j = pos.j + 1;
					//valid_positions.push(next_pos);
					std::thread t(walk, next_pos);
					t.detach();
				}
			}
			if(pos.j - 1 >= 0){
				if(maze[pos.i][pos.j-1] == 's'){
					retorno_walk =  true;
				}else if(maze[pos.i][pos.j-1] == 'x'){
					pos_t next_pos;
					next_pos.i = pos.i;
					next_pos.j = pos.j - 1;
					//valid_positions.push(next_pos);
					std::thread t(walk, next_pos);
					t.detach();
				}
			}
			if(pos.i + 1 < num_rows){
				if(maze[pos.i +1][pos.j] == 's'){
					retorno_walk =  true;
				}else if(maze[pos.i + 1][pos.j] == 'x'){
					pos_t next_pos;
					next_pos.i = pos.i + 1;
					next_pos.j = pos.j;
					//valid_positions.push(next_pos);
					std::thread t(walk, next_pos);
					t.detach();
				}
			}
			if(pos.i - 1 >= 0){
				if(maze[pos.i -1][pos.j] == 's'){
					retorno_walk =  true;
				}else if(maze[pos.i - 1][pos.j] == 'x'){
					pos_t next_pos;
					next_pos.i = pos.i - 1;
					next_pos.j = pos.j;
					//valid_positions.push(next_pos);
					std::thread t(walk, next_pos);
					t.detach();
				}
			}
		}

		maze[pos.i][pos.j] = '.';

}

int main(int argc, char* argv[]) {
	
	if(argv[1] != nullptr){		
		//const char * nome_do_arquivo = "../data/maze2.txt";
		//std::cout << nome_do_arquivo << std::endl;
		//pos_t initial_pos = load_maze(nome_do_arquivo);
		
		//carregar o labirinto com o nome do arquivo recebido como argumento
		printf("nome do arquivo: ");
		printf("%s", argv[1]);
		pos_t initial_pos = load_maze(argv[1]);

		if(maze != nullptr){
			printf("Posição inicial: %d %d\n", initial_pos.i, initial_pos.j);
			std::thread printThread(print_maze);
			
			walk(initial_pos);


			
			while(true){
				if(retorno_walk){
					break;
				}
				if(!retorno_walk and contador==0){//Não encontrou a saída e acabou as threads
					break;
				}
			}

			printThread.join();

			
			if(retorno_walk){
				printf("\n\nA saida foi encontrada\n\n");
				return 0;
			}
			if(!retorno_walk and contador==0){//Não encontrou a saída e acabou as threads
				printf("\n\nA saida NÃO foi encontrada\n\n");
				return 0;
			}
			
		}
	}

	return 0;
}

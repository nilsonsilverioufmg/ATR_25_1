#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>

// Representação do labirinto
using Maze = std::vector<std::vector<char>>;

// Estrutura para representar uma posição no labirinto
struct Position {
    int row;
    int col;
};

// Variáveis globais
Maze maze;
int num_rows;
int num_cols;
std::stack<Position> valid_positions;

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return {-1, -1};
    }

    // Lê o número de linhas e colunas
    file >> num_rows >> num_cols;

    // Redimensiona a matriz 'maze'
    maze.resize(num_rows, std::vector<char>(num_cols));

    Position start_pos = {-1, -1};  // Posição inicial

    // Lê o conteúdo do labirinto e encontra a posição inicial
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            file >> maze[i][j];
            if (maze[i][j] == 'e') {
                start_pos = {i, j};
            }
        }
    }

    file.close();
    return start_pos;
}

// Função para imprimir o labirinto
void print_maze() {
    system("clear"); // limpa o console
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    return (row >= 0 && row < num_rows && col >= 0 && col < num_cols && (maze[row][col] == 'x' || maze[row][col] == 's'));
}

// Função principal para navegar pelo labirinto
bool walk(Position pos) {
    // Marca a posição atual como visitada
    if (maze[pos.row][pos.col] != 's') {
        maze[pos.row][pos.col] = '.';  // Marca como explorada, exceto se for a saída
    }

    // Imprime o estado atual do labirinto
    print_maze();

    // Aguarda um pequeno tempo para visualização
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Verifica se a posição atual é a saída
    if (maze[pos.row][pos.col] == 's') {
        return true;
    }

    // Verifica as posições adjacentes (cima, baixo, esquerda, direita)
    Position directions[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // cima, baixo, esquerda, direita
    for (const auto& dir : directions) {
        int new_row = pos.row + dir.row;
        int new_col = pos.col + dir.col;

        if (is_valid_position(new_row, new_col)) {
            valid_positions.push({new_row, new_col});
        }
    }

    // Enquanto houver posições válidas, continua a exploração
    while (!valid_positions.empty()) {
        Position next_pos = valid_positions.top();
        valid_positions.pop();

//        std::cout << "Explorando posição: (" << next_pos.row << ", " << next_pos.col << ")" << std::endl;  // Depuração

        // Chama walk recursivamente para a próxima posição
        if (walk(next_pos)) {
            return true;  // Se encontrou a saída, propaga a vitória
        }
    }

    // Se não encontrou a saída, retorna false
    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_labirinto>" << std::endl;
        return 1;
    }

    Position initial_pos = load_maze(argv[1]);
    if (initial_pos.row == -1 || initial_pos.col == -1) {
        std::cerr << "Posição inicial não encontrada no labirinto." << std::endl;
        return 1;
    }

    bool exit_found = walk(initial_pos);

    if (exit_found) {
        std::cout << "Saída encontrada!" << std::endl;
    } else {
        std::cout << "Não foi possível encontrar a saída." << std::endl;
    }

    return 0;
}

#include <complex>
#include <fstream>
#include <array>
#include <iostream>
#include <stdexcept>

// Given a file, validates that every line is a valid sudoku
const int BOARD_SIZE = 9;
const int CELL_SIZE = 3;
using Board = std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE>;

bool is_valid(const Board& board) {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        std::array<bool, BOARD_SIZE> row_set = {};
        for (int c = 0; c < BOARD_SIZE; ++c) {
            int num_index = board[r][c] - 1;
            if (num_index < 0 || num_index > 9 || row_set[num_index]) return false;
            row_set[num_index] = true;
        }
    }

    for (int c = 0; c < BOARD_SIZE; ++c) {
        std::array<bool, BOARD_SIZE> col_set = {};
        for (int r = 0; r < BOARD_SIZE; ++r) {
            int num_index = board[r][c] - 1;
            if (num_index < 0 || num_index > 9 || col_set[num_index]) return false;
            col_set[num_index] = true;
        }
    }

    for (int r = 0; r < BOARD_SIZE; r += 3) {
        for (int c = 0; c < BOARD_SIZE; c += 3) {
            std::array<bool, BOARD_SIZE> cell_set = {};
            for (int dr = 0; dr < CELL_SIZE; ++dr) {
                for (int dc = 0; dc < CELL_SIZE; ++dc) {
                    int num_index = board[r+dr][c+dc] - 1;
                    if (num_index < 0 || num_index > 9 || cell_set[num_index]) return false;
                    cell_set[num_index] = true;
                }
            }
        }
    }

    return true;
}

Board parse(std::string& line) {
    if (line.length() != BOARD_SIZE * BOARD_SIZE) {
        throw std::invalid_argument("Invalid input length: must be exactly " + std::to_string(BOARD_SIZE * BOARD_SIZE) + " characters.");
    }
    // assumes length 81
    Board board;
    for (int i = 0; i < line.length(); ++i) {
        board[i / BOARD_SIZE][i % BOARD_SIZE] = line[i] - '0';
    }

    return board;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Please input the file path as the second argument" << std::endl;
        return 1;
    }

    std::string path = argv[1];
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open file " << path << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        Board board = parse(line);
        if (!is_valid(board)) {
            std::cout << line << std::endl;;
            std::cout << "false" << std::endl;
            return 1;
        }
    }

    std::cout << "true" << std::endl;
    return 0;
}

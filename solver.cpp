#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <cstdlib>
#include <optional>
#include <algorithm> // For fill algorithm
#include <bitset>
#include <gperftools/profiler.h>

const int BOARD_SIZE = 9;
const int CELL_SIZE = 3;

// hashmap for each row to check if it contains
// for each row, true if cell is in place
// index is val - 1

using CellGroup = std::array<bool, BOARD_SIZE>;
using SudokuGroup = std::array<CellGroup, BOARD_SIZE>;
using Board = std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE>;

struct SudokuBoard {
    Board board;
    SudokuGroup rows;
    SudokuGroup cols;
    SudokuGroup cells;
};

std::ostream& operator<<(std::ostream& out, const Board& board) {
    for (const auto& row : board) {
        for (const auto& num : row) {
            out << num << ' ';
        }
        out << '\n';
    }
    out << std::endl;
    return out;
}

int write(const std::vector<Board>& boards, std::string path) {
    std::ofstream file(path, std::ios::trunc |std::ios::out);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    for (const auto& board : boards) {
        for (const auto& row : board) {
            for (const auto& num : row) {
                file << num;
            }
        }
        file << std::endl;
    }

    file.close();
    return 0;
}

int read(std::vector<Board>& boards, std::string path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        Board board;
        std::array<int, 9> row;
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                row[c] = line[r * 9 + c] - '0';
            }
            board[r] = row;
        }
        boards.push_back(board);
    }

    file.close();

    return 0;
}

bool valid(const SudokuBoard& sudoku_board, int r, int c, int num_index) {
    // Determines num placed in (r,c) is a valid board
    if (sudoku_board.rows[r][num_index] == true) return false;
    if (sudoku_board.cols[c][num_index] == true) return false;

    int cell_row = (r / CELL_SIZE),
        cell_col = (c / CELL_SIZE);

    if (sudoku_board.cells[3 * cell_row + cell_col][num_index] == true) return false;
    return true;
}

bool solve_board(SudokuBoard& sudoku_board, int i) {
    if (i == BOARD_SIZE * BOARD_SIZE) {
        return true;
    }
    int r = i / BOARD_SIZE, c = i % BOARD_SIZE;
    if (sudoku_board.board[r][c] != 0) {
        return solve_board(sudoku_board, i + 1);
    }

    for (int num_index = 0; num_index <= 8; ++num_index) {
        if (!valid(sudoku_board, r, c, num_index)) continue;
        int cell_row = (r / CELL_SIZE),
            cell_col = (c / CELL_SIZE);

        sudoku_board.board[r][c] = num_index + 1;
        sudoku_board.rows[r][num_index] = true;
        sudoku_board.cols[c][num_index] = true;
        sudoku_board.cells[3 * cell_row + cell_col][num_index] = true;
        if (solve_board(sudoku_board, i + 1)) {
            return true;
        }
        sudoku_board.rows[r][num_index] = false;
        sudoku_board.cols[c][num_index] = false;
        sudoku_board.cells[3 * cell_row + cell_col][num_index] = false;
    }

    return false;
}

Board solve(Board& board) {
    SudokuBoard sudoku_board;
    sudoku_board.board = board;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        std::ranges::fill(sudoku_board.rows[i], false);
        std::ranges::fill(sudoku_board.cols[i], false);
        std::ranges::fill(sudoku_board.cells[i], false);
    }

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            int val_index = sudoku_board.board[r][c] - 1;
            if (val_index != -1) {
                int cell_row = (r / CELL_SIZE), cell_col = (c / CELL_SIZE);
                sudoku_board.rows[r][val_index] = true;
                sudoku_board.cols[c][val_index] = true;
                sudoku_board.cells[3 * cell_row + cell_col][val_index] = true;
            }
        }
    }

    solve_board(sudoku_board, 0);

    return sudoku_board.board;
}

int main() {
    ProfilerStart("solver.prof");
    std::vector<Board> boards;
    boards.reserve(10e6);
    read(boards, "problems.txt");
    for (Board& board : boards) {
        board = solve(board);
    }
    write(boards, "solutions.txt");

    ProfilerStop();
    return 0;
}

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <optional>

const int BOARD_SIZE = 9;
const int CELL_SIZE = 3;

using SudokuBoard = std::vector<std::vector<int> >;

std::ostream& operator<<(std::ostream& out, const SudokuBoard& board) {
    for (const auto& row : board) {
        for (const auto& num : row) {
            out << num << ' ';
        }
        out << '\n';
    }
    out << std::endl;
    return out;
}

int write(const std::vector<SudokuBoard>& boards, std::string path) {
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

int read(std::vector<SudokuBoard>& boards, std::string path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        SudokuBoard board;
        std::vector<int> row;
        row.reserve(9);
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                row.push_back(line[r * 9 + c] - '0');
            }
            board.push_back(row);
            row.clear();
        }
        boards.push_back(board);
    }

    file.close();

    return 0;
}
bool valid(const SudokuBoard& board, int r, int c, int num) {
    // Determines num placed in (r,c) is a valid board
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[r][i] == num) return false;
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i][c] == num) return false;
    }

    int cell_row = (r / CELL_SIZE) * CELL_SIZE,
        cell_col = (c / CELL_SIZE) * CELL_SIZE;
    for (int dr = 0; dr < CELL_SIZE; ++dr) {
        for (int dc = 0; dc < CELL_SIZE; ++dc) {
            if (board[cell_row + dr][cell_col + dc] == num) return false;
        }
    }

    return true;
}

bool solve_board(SudokuBoard& board, int i) {
    if (i == BOARD_SIZE * BOARD_SIZE) {
        return true;
    }
    int r = i / BOARD_SIZE, c = i % BOARD_SIZE;
    if (board[r][c] != 0) {
        return solve_board(board, i + 1);
    }
    for (int num = 1; num <= 9; ++num) {
        if (!valid(board, r, c, num)) continue;
        board[r][c] = num;
        if (solve_board(board, i + 1)) {
            return true;
        }
        board[r][c] = 0; // Don't need this since we only go forward
    }

    return false;
}

std::optional<SudokuBoard> solve(SudokuBoard& board) {
    // Solves a sudoku game
    if (solve_board(board, 0)) {
        return board;
    } else {
        return std::nullopt;
    }
}

int main() {
    std::vector<SudokuBoard> boards;
    // read(boards, "tiny.txt");
    read(boards, "problems.txt");
    for (SudokuBoard& board : boards) {
        solve(board);
    }
    write(boards, "solutions.txt");
    return 0;
}

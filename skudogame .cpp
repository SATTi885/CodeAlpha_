#include <iostream>

using namespace std;

class SudokuSolver {
private:
    static const int SIZE = 9;
    int grid[SIZE][SIZE];

    // Check if a number can be placed at grid[row][col]
    bool isSafe(int row, int col, int num) {
        // Check row
        for (int x = 0; x < SIZE; x++) {
            if (grid[row][x] == num)
                return false;
        }

        // Check column
        for (int x = 0; x < SIZE; x++) {
            if (grid[x][col] == num)
                return false;
        }

        // Check 3x3 subgrid
        int startRow = row - row % 3;
        int startCol = col - col % 3;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (grid[startRow + i][startCol + j] == num)
                    return false;
            }
        }

        return true;
    }

    // Find an empty cell
    bool findEmptyCell(int &row, int &col) {
        for (row = 0; row < SIZE; row++) {
            for (col = 0; col < SIZE; col++) {
                if (grid[row][col] == 0)
                    return true;
            }
        }
        return false;
    }

public:
    // Constructor
    SudokuSolver(int initialGrid[SIZE][SIZE]) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                grid[i][j] = initialGrid[i][j];
            }
        }
    }

    // Backtracking function
    bool solve() {
        int row, col;

        // If no empty cell remains, puzzle is solved
        if (!findEmptyCell(row, col))
            return true;

        // Try numbers 1 to 9
        for (int num = 1; num <= 9; num++) {
            if (isSafe(row, col, num)) {
                grid[row][col] = num;

                // Recursively solve remaining puzzle
                if (solve())
                    return true;

                // Backtrack
                grid[row][col] = 0;
            }
        }

        return false;
    }

    // Display the Sudoku grid
    void display() {
        for (int i = 0; i < SIZE; i++) {
            if (i % 3 == 0 && i != 0)
                cout << "-------------------------\n";

            for (int j = 0; j < SIZE; j++) {
                if (j % 3 == 0 && j != 0)
                    cout << "| ";

                cout << grid[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    int puzzle[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    SudokuSolver solver(puzzle);

    cout << "Original Sudoku Puzzle:\n";
    solver.display();

    if (solver.solve()) {
        cout << "\nSolved Sudoku Puzzle:\n";
        solver.display();
    } else {
        cout << "\nNo solution exists.\n";
    }

    return 0;
}
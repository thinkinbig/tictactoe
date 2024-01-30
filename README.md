# TicTacToe

TicTacToe is a classic two-player game played on a 3x3 grid. The objective of the game is to get three of your own symbols (either X or O) in a row, either horizontally, vertically, or diagonally. Players take turns placing their symbols on the grid until one player achieves the winning condition or the game ends in a draw.

This README provides an overview of the TicTacToe game, its rules, and instructions on how to play. It also includes information on how to set up the game and run it on your local machine.

Let's get started!

# How to Run
Here are the short building instructions for your project:

1. Navigate to the root directory of your project in a terminal.
2. Create a new directory for the build files. This is typically called build:
   ```shell
   mkdir build
   ```
3. Navigate into the build directory:
   ```shell
   cd build
   ```
4. Run cmake to generate the build files. Replace .. with the path to the CMakeLists.txt file if it's not in the parent directory:
   ```shell
   cmake ..
   ```
5. Run make to build the project:
   ```shell
   make
   ```
After these steps, the tictactoe executable should be in the build directory. You can run it with `./tictactoe`.
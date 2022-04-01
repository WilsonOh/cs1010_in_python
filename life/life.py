import numpy as np
import time
import os
import sys


board = list[list[bool]]

FPS = 20


def read_initial_state(file: str) -> tuple[board, int]:
    with open(file) as f:
        row, _, cycles = list(map(int, f.readline().strip().split()))
        return [[True if x == '#' else False for x in f.readline().strip()]
                for _ in range(row)], cycles


def get_next_generation(game: board) -> board:
    return [[True if is_alive(game, row_idx, col_idx) else False
             for col_idx, _ in enumerate(row)]
            for row_idx, row in enumerate(game)]


def is_alive(game: board, row: int, col: int) -> bool:
    new_game = np.array(game, dtype=bool)
    curr = game[row][col]
    alive = np.sum(new_game[row - 1: row + 2, col - 1: col + 2]) - curr
    if curr:
        return 2 <= alive <= 3
    return alive == 3


def simulate(state: board, cycles: int) -> None:
    for _ in range(cycles):
        try:
            os.system('clear')
            print_board(state)
            state = get_next_generation(state)
            time.sleep(1/FPS)
        except KeyboardInterrupt:
            print("Ended Prematurely")
            break


def print_board(game: board):
    for row in game:
        for item in row:
            print('#' if item else '.', end='')
        print()


def main():
    if len(sys.argv) < 2:
        print(f"USAGE: python3 {sys.argv[0]} inputs/life.<1-6>.in")
        return
    init_state, cycles = read_initial_state(sys.argv[1])
    simulate(init_state, cycles)


if __name__ == "__main__":
    main()

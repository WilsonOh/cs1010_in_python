import os
import time
import sys


RED   =  "\x1b[31m"
GREEN  = "\x1b[32m"
YELLOW = "\x1b[33m"
BLUE  =  "\x1b[34m"
MAGENTA =  "\x1b[35m"
CYAN  =  "\x1b[36m"
RESET =   "\x1b[0m"


class Maze:
    def __init__(self):
        self.maze = self.init_maze()
        self.maze_copy = [x[:] for x in self.maze]
        self.scully_prev_pos = None
        self.counter = 0

    def init_maze(self):
        m, _ = input().split()
        return [list(input()) for _ in range(int(m))]

    def find_scully(self):
        for x, row in enumerate(self.maze):
            if '@' in row:
                return x, row.index('@')
        return (0, 0)

    def find_empty(self, prev_pos=None):
        x, y = self.find_scully()

        if x == len(self.maze) - 1 or y == len(self.maze[0]) - 2 or x == 0 or y == 0:
            return True

        if self.maze[x - 1][y] == '.' and self.maze_copy[x - 1][y] == '.':  # up
            self.move(x - 1, y)
            if self.find_empty((x, y)):
                return True

        if self.maze[x][y + 1] == '.' and self.maze_copy[x][y + 1] == '.':  # right
            self.move(x, y + 1)
            if self.find_empty((x, y)):
                return True

        if self.maze[x + 1][y] == '.' and self.maze_copy[x + 1][y] == '.':  # down
            self.move(x + 1, y)
            if self.find_empty((x, y)):
                return True

        if self.maze[x][y - 1] == '.' and self.maze_copy[x][y - 1] == '.':  # left
            self.move(x, y - 1)
            if self.find_empty((x, y)):
                return True

        if prev_pos is not None:
            self.move(*prev_pos)

        return False

    def move(self, x, y):
        x0, y0 = self.find_scully()
        self.maze[x0][y0] = '.'
        self.maze_copy[x0][y0] = '#'
        self.maze[x][y] = '@'
        self.counter += 1
        self.print_maze()

    def print_maze(self):
        if sys.stdout.isatty():
            os.system('clear')
        for row in self.maze:
            for char in row:
                if sys.stdout.isatty():
                    if char == '.':
                        print(BLUE, end='')
                    elif char == '#':
                        print(RED, end='')
                    else:
                        print(YELLOW, end='')
                print(char, end='')
            if sys.stdout.isatty():
                print(RESET)
            else:
                print()

        print(self.counter)
        if sys.stdout.isatty():
            time.sleep(0.3)




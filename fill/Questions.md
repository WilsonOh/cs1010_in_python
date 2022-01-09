# Question 1: Fill (20 marks)
-----------------------------

One of the basic operation in drawing software is the
fill operation (aka bucket fill).  You will implement this
operation in this assignment.

An image can be considered as a 2D array of pixels.
Each pixel has a color, represented by three integers,
corrseponding to the red, green, and blue component of
the color.   We say that two pixels are adjacent to each
other if they share a common edge.  So a pixel can have
at most four adjacent pixels.

We denote the top left pixel to be (0,0) and the indices
increases towards the right and down.

An image can be divided into one of more segments.
A segment is a set of pixels (i) with the same color, and
(ii) given any two pixels p and q in a segment, we can
find a series of adjacent pixels in the segment going from
p to q.

The fill operation takes in as an input the (x,y)
coordinate of a pixel p, and a color c to fill.  The
operation then replaces the color of all pixels belong to
the same segment as p with the given color c.

Write a program, `fill.c`, that reads from standard input
two positive integers m and n in the first line.  m refers
to the number of rows, and n is the number of columns.

Then read 3mn integers from the standard inputs.
Each tuple of 3 integers refer to the color of a pixel.
So the first three integers refer to the color of pixel
at (0, 0), the next three integers refer to the color of
pixel at (0, 1), etc.

The next input is a positive integer q, which is the number
of fill operations.  Following this, there are q lines.
Each line has two integers x, y, and three integers, r,
g, and b, corresponding to a color.  Each line specifies
a fill operation, to fill the segment pixel (x, y) is in
with the color (r, g, b).

The program shall prints to standard output, the image
after executing all the given fill operations.  The image
should be printed in the following format:

- The first line is a string "P3"
- The second line contains n and m, the width and height 
  of the image.
- The third line is the number 255.
- The remaining lines contains the color of each pixels,
  one color (i.e., three integers) per line.

You should use recursion to implement the fill operation.

Your solution must take no more than O(mnq) time.

You can visualize the resulting image using the `viu` command:

```
./fill < inputs/fill.3.in | ~cs1010/bin/viu -``
```

Note that `viu` might rescale you image if your terminal 
is too small.

### Tips

Similar to `social`, your code would be simpler if you
separate the representation of the image from the 
underlying 2D array.  Writing functions to retrieve, 
update, and compare the color of a pixel of an image
would make your code much simpler and easier to read,
write, and debug.

### Grading Criteria

The grading criteria for this question is:

|              | Marks |
|--------------|-------|
| Correctness  | 10    |
| Efficiency   | 10    |

Your solution must take no more than O(mnq) time to obtain
the full efficiency marks.

# Question 2: Maze (20 marks)
-----------------------------

Agent Scully woke up and found herself in the dark.  She
figured out that she is in a maze.  She has to find her way
out if there is one!

The maze can be simplified as a grid of (m x n) cells. Each
cell can be of the following:

1. '#' denotes a segment of a wall.  No one can pass through
   the walls.

2. '.' denotes an empty space

3. '@' denotes where Scully is at currently. It is also an
   empty space

Anytime Scully reaches a border cell (a cell in either the
top-most or bottom-most row or left-most or right-most
column), she escapes the maze and can go save her partner
Agent Mulder.  She can only move from one empty space to
another adjacent cell in one step.  Two cells are adjacent
if they share a common edge.

Scully took CS1010, and she got a concrete plan to seek a
way out by trial and error.  She follows **strictly** the
following strategy to find a way through the maze starting
from her initial position.  At each time step,

1. She looks for an empty adjacent cell that has never been
visited yet, in the sequence of up/right/down/left to the
current cell she is at.  If there is an empty adjacent cell,
she moves to that cell.  The cell she moves to is now
visited.

2. If no empty, unvisited, adjacent cell exists, she
backtracks on the path that she comes from, moving one step
back, and repeat 1 again.

In this way, Scully systematically explores the maze,
with no repetitive visit of any cell more than once except
when she backtracks.  She will stop when successfully
escaped the maze, or finds that there is no way out after
backtracking all the way back to the original position.  She
is completely trapped within the maze and now must wait for
Agent Mulder to come and free her.

Write a program `maze.c`, that reads from standard input.
First, read two positive integers m and n, followed by m
lines of n characters in each line that represents the maze
setup.  One and only one `@` will be present in the maze
setup.

The program then prints, to standard output, an animation of
$k$ iterations. The output should only contain $m$ rows with
$n$ characters in each row, with an additional row at last.
Similarly, you must use `#` to represent a wall, a `.` to
represents empty space, and `@` to represent where Scully is
at.  After printing the maze, your program prints the number
of steps that Scully has made.

You should use recursion to explore the maze and look for a
way out.

Here is an example.  The following is the starting position
of Scully and the maze.

```
###########
#.#.....#.#
#.#####.#.#
#.#...#.#.#
#.#.#.....#
#.#.##.#.##
#@#.#..#..#
#...#.#.#..
###########
0
```

Scully firstly moves five steps up:
```
###########
#@#.....#.#
#.#####.#.#
#.#...#.#.#
#.#.#.....#
#.#.##.#.##
#.#.#..#..#
#...#.#.#..
###########
5
```

At this point, Scully is stuck since there is no more
adjacent empty cell that has not been visited.  Scully then
backtracks:
```
###########
#.#.....#.#
#.#####.#.#
#.#...#.#.#
#.#.#.....#
#.#.##.#.##
#@#.#..#..#
#...#.#.#..
###########
10
```

Scully then moves down:
```
###########
#.#.....#.#
#.#####.#.#
#.#...#.#.#
#.#.#.....#
#.#.##.#.##
#.#.#..#..#
#@..#.#.#..
###########
11
```

Then right:
```
###########
#.#.....#.#
#.#####.#.#
#.#...#.#.#
#.#.#.....#
#.#.##.#.##
#.#.#..#..#
#..@#.#.#..
###########
13
```

Then up:
```
###########
#.#.....#.#
#.#####.#.#
#.#@..#.#.#
#.#.#.....#
#.#.##.#.##
#.#.#..#..#
#...#.#.#..
###########
17
```

Then right (two steps) and then down (two steps) and then
right (two steps):

```
###########
#.#.....#.#
#.#####.#.#
#.#...#.#.#
#.#.#..@..#
#.#.##.#.##
#.#.#..#..#
#...#.#.#..
###########
22
```

Then Scully moves up and left, and she is stuck again.

```
###########
#.#@....#.#
#.#####.#.#
#.#...#.#.#
#.#.#.....#
#.#.##.#.##
#.#.#..#..#
#...#.#.#..
###########
29
```

At this point she backtracks:

```
###########
#.#.....#.#
#.#####.#.#
#.#...#.#.#
#.#.#..@..#
#.#.##.#.##
#.#.#..#..#
#...#.#.#..
###########
36
```

Moves right, and up, and stuck again!

```
###########
#.#.....#@#
#.#####.#.#
#.#...#.#.#
#.#.#.....#
#.#.##.#.##
#.#.#..#..#
#...#.#.#..
###########
41
```
She backtracks again,

```
###########
#.#.....#.#
#.#####.#.#
#.#...#.#.#
#.#.#...@.#
#.#.##.#.##
#.#.#..#..#
#...#.#.#..
###########
45
```

This time she found her way out!

```
###########
#.#.....#.#
#.#####.#.#
#.#...#.#.#
#.#.#.....#
#.#.##.#.##
#.#.#..#..#
#...#.#.#.@
###########
50
```

It took her a total of 50 steps to exit the maze.

### Grading Criteria

The grading criteria for this question is:

|              | Marks |
|--------------|-------|
| Correctness  | 10    |
| Efficiency   | 10    |

Your solution must take no more than O(mn) time to obtain
the full efficiency marks.

Hint: You need to strictly follow the described strategy and
sequence of exploration. Do not forget to print the initial
matrix and the final matrix.

### Animation on Screen

We have provided the code to draw the maze in the skeleton
file.

## Sample Run

You can use the sample program `~cs1010/as08/maze` on the given
inputs to view the animation.

Note that how the maze is printed on the standard output is
different from when it is written to a file.  We have removed
the escape code (which you have encoutered in the problem life) 
in the output file to avoid confusion.

# Task 9: areas by kind

The program receives one argument: the path of a file of shapes, one per line:

- `square S`, of area S×S;
- `rect W H`, of area W×H;
- `tri B H`, a triangle of base B and height H, of area half of B×H rounded down;
- `diamond P Q`, a rhombus of diagonals P and Q, of area half of P×Q rounded down.

Every number is one or more decimal digits, and the words of a line are separated by exactly one
space. The file is UTF-8 text, and it is promised, with nothing to check, that no number in it is
above 1,000,000. Each shape's area is rounded down on its own, before it is added to any total, and
every total fits in a signed 64-bit integer.

Print, for each kind that appears at least once and in the order `square`, `rect`, `tri`, `diamond`,
a line with the kind, one space, how many shapes of that kind there are, one space and the sum of
their areas; then a line `total T`, T being the sum of every area. Exit with status 0. If a line is
not one of the four forms, print `bad line N`, where N is its number counting from 1, print nothing
else, and exit with status 1; the first bad line decides. A line ends at a line feed, and the file's
last line may or may not end with one; a carriage return is an ordinary character, and an empty
line is bad. An empty file prints `total 0`. If the program does not receive exactly one argument,
or the file cannot be read, print nothing and exit with status 2.

Example. A file holding

```
rect 2 3
tri 3 3
square 4
tri 1 1
```

makes the program print:

```
square 1 16
rect 1 6
tri 2 4
total 26
```

canary heroes-task:6ffb0de00cd966b847c3de2a0e42545a

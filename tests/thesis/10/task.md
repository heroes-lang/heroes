# Task 10: merged intervals

The program receives one argument: the path of a file of intervals, one per line, each two whole
numbers A and B separated by one space, with A not greater than B, standing for every integer from A
to B, both included. A whole number is an optional `-` followed by one or more decimal digits. The
file is UTF-8 text, and it is promised, with nothing to check, that every number in it lies between
-1,000,000,000 and 1,000,000,000, both included.

Merge the intervals: two intervals merge when they share an integer or when one ends exactly one
before the other begins, so `1 3` and `4 6` merge into `1 6`, and merging goes on until no two
intervals of the result would merge. Print the result one interval per line, as A, one space and B,
in ascending order of A, each number in its shortest decimal form with a `-` only before a negative
one, so `007` prints `7` and `-0` prints `0`. The lines of the file may come in any order. Exit with
status 0.

If a line is not two whole numbers separated by one space, or its A is greater than its B, print
`bad line N`, where N is its number counting from 1, print nothing else, and exit with status 1; the
first bad line decides. A line ends at a line feed, and the file's last line may or may not end with
one; a carriage return is an ordinary character, and an empty line is bad. An empty file prints
nothing. If the program does not receive exactly one argument, or the file cannot be read, print
nothing and exit with status 2.

Example. A file holding

```
5 8
1 3
4 4
10 12
```

makes the program print:

```
1 8
10 12
```

canary heroes-task:206f0310e2faa9d046f3e828ba75eca5

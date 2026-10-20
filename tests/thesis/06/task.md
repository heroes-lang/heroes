# Task 6: a grid turned

The program receives two arguments: the path of a file holding a grid, and a number of turns K. The
grid is the file's lines, top to bottom, each line a row read from left to right. A line ends at a
line feed, and the file's last line may or may not end with one. The file holds ASCII characters
only, and a carriage return is an ordinary character of the row it stands in. K is one or more
decimal digits; where it is, its value is promised to fit in a signed 64-bit integer, with nothing
to check.

Turn the grid a quarter turn clockwise K times and print the result, one row per line. A quarter
turn clockwise makes the first column, read from the bottom row up, the first row.

Exit with status 0. An empty file prints nothing. If a row is empty, or the rows do not all hold the
same number of characters, print `ragged` and exit with status 1. If the program does not receive
exactly two arguments, K is not one or more decimal digits, or the file cannot be read, print
nothing and exit with status 2; these are checked before anything the file holds.

Example. A file holding

```
abc
def
```

turned 1 time prints:

```
da
eb
fc
```

canary heroes-task:7d13386c88380fa1be22b495120c9c2a

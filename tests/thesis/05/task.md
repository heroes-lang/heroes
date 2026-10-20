# Task 5: a stack calculator

The program receives one argument: the path of a file of commands, one per line. It keeps a stack
of whole numbers, empty at the start, and runs the commands in order, each line when its turn comes:

- `push N` puts the whole number N on the stack, N being an optional `-` followed by one or more
  decimal digits;
- `pop` removes the number on top;
- `add`, `sub` and `mul` remove the two numbers on top and put back their sum, difference or
  product, where `sub` takes the number that was on top from the one beneath it;
- `print` prints the number on top on a line of its own and leaves it where it is.

A line is a command only if it is exactly one of these words in lowercase, `push` followed by one
space and its number, every other word alone. An empty line is not a command, and a carriage return
is an ordinary character. The file is UTF-8 text. A line ends at a line feed, and the file's last
line may or may not end with one. Every number the program reads or computes fits in a signed
64-bit integer.

Exit with status 0 after the last command. If a line is not a command, print `line N: bad command`,
where N is the line's number counting from 1, and exit with status 1; if a command needs more
numbers than the stack holds, print `line N: stack underflow` and exit with status 1. What earlier
`print` commands printed stays printed, and nothing after the first such line runs. An empty file
prints nothing. If the program does not receive exactly one argument, or the file cannot be read,
print nothing and exit with status 2.

Example. A file holding

```
push 10
push 3
sub
print
push 7
mul
print
```

makes the program print:

```
7
49
```

canary heroes-task:3624c2b16cd4d263e962a781b73b54fc

# Task 2: a ledger's balances

The program receives one argument: the path of a file whose every line holds a name, one space,
and a whole number that may be negative, such as `anna 120` or `bob -45`. Sum the numbers per name.
A name is one or more ASCII letters, and two names are the same only if their bytes are. A whole
number is an optional `-` followed by one or more decimal digits, so `007` is seven and `+5` is not
a whole number. The file is UTF-8 text. A line ends at a line feed, and the file's last line may or
may not end with one; a carriage return is an ordinary character, so a line ending in one is not
well formed, and neither is an empty line. Every number fits in a signed 64-bit integer, and so
does every sum a name's numbers reach when they are added in the file's order.

Print one line per name, the name, one space, its total, names in ascending byte order. Exit with
status 0. If any line is not exactly a name, one space and a whole number, print `bad line N`, where
N is that line's number counting from 1, print nothing else, and exit with status 1; the first bad
line decides. An empty file prints nothing and exits 0. If the program does not receive exactly one
argument, or the file cannot be read, print nothing and exit with status 2.

Example. A file holding

```
anna 120
bob -45
anna 30
```

makes the program print:

```
anna 150
bob -45
```

canary heroes-task:6d60697987d82b4da87af6a22f9de038

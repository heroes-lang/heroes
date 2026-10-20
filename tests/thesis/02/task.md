# Task 2: a ledger's balances

The program receives one argument: the path of a file whose every line holds a name, one space,
and a whole number that may be negative, such as `anna 120` or `bob -45`. Sum the numbers per name.

Print one line per name, the name, one space, its total, names in ascending byte order. Exit with
status 0. If any line is not exactly a name, one space and a whole number, print `bad line N`, where
N is that line's number counting from 1, print nothing else, and exit with status 1; the first bad
line decides. An empty file prints nothing and exits 0.

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

# Task 1: word counts

The program receives one argument: the path of a file of UTF-8 text. A word is a maximal run of
ASCII letters; letters are compared without regard to case, and a word is printed in lower case.
Count how many times each distinct word appears in the file.

Print one line per distinct word, the word, one space, its count, ordered by count from largest to
smallest and, for equal counts, by word in ascending byte order. Exit with status 0. If the program
does not receive exactly one argument, or the file cannot be read, print nothing and exit with
status 2.

Example. A file holding `The cat and the hat.` makes the program print:

```
the 2
and 1
cat 1
hat 1
```

canary heroes-task:01b983460de1b0fd28e2619ac80fd734

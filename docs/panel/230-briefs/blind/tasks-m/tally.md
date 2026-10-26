# A program to write: a tally

The program is run as a command, with the arguments given, by somebody who reads what it prints and
the status it exits with.

The program receives one argument: the path of a file whose every line holds a whole number, an
optional `-` followed by one or more decimal digits. A line ends at a line feed, and the file's last
line may or may not end with one. It prints `N lines, total T` followed by a line feed, where `N` is
how many lines the file holds and `T` their sum, and exits with status 0; an empty file prints `0
lines, total 0`. If a line is not a whole number, it prints `bad line K`, where `K` is that line's
number counting from 1, prints nothing else, and exits with status 1; the first bad line decides. If
the program does not receive exactly one argument, or the file cannot be read, it prints nothing and
exits with status 2.

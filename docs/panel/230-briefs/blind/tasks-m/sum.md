# A program to write: a sum

The program is run as a command, with the arguments given, by somebody who reads what it prints and
the status it exits with.

The program receives one argument: the path of a file whose every line holds a whole number, an
optional `-` followed by one or more decimal digits, such as `12` or `-7`; `007` is seven, and `+5`
is not a whole number. A line ends at a line feed, and the file's last line may or may not end with
one. Print the sum of the numbers followed by a line feed, and exit with status 0; an empty file
prints `0`. If the program does not receive exactly one argument, if the file cannot be read, or if
a line is not a whole number, print nothing on standard output and exit with a status other than 0.

# Task 2: an indented listing

The program receives exactly one argument: the path of a text file. It numbers the file's lines
from 0. For each line it prints, on a line of its own, as many spaces as the line's number followed
by the part of the line after its first `:`, or by the whole line when it has no `:`. A final
newline at the end of the file does not make an extra empty line. If the program does not receive
exactly one argument, or the file cannot be read, it prints nothing and exits with code 2;
otherwise it exits with code 0.

Example: for a file holding the three lines `a:one`, `two` and `c:th:ree`, the output is

```
one
 two
  th:ree
```

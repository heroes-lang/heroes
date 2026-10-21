# Task 1: a heading and its underline

The program receives exactly two arguments: a text, and a whole number k written in decimal digits.
It prints the first k bytes of the text on one line, and on the next line exactly k `=` characters.
If the program does not receive exactly two arguments, or k is not written only with the digits 0
to 9, or k is larger than the text's length in bytes, it prints nothing and exits with code 2.
Otherwise it exits with code 0.

Example: with the arguments `hello world` and `5`, the output is

```
hello
=====
```

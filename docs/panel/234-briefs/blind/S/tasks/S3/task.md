# The task

Write a Heroes program that reads the path of a text file from `args()[0]` and a text from `args()[1]`. It prints two lines: first the number of words in the file, a word being a run of characters between spaces, tabs and line ends; then how many times the text `args()[1]` occurs in the file, the occurrences not overlapping. Print nothing else.

Use the package `core/strings` (see `brief.md`).

## Example

With this file, and `args()[1]` being `the`:

```
the cat and the hat
the end
```

the program prints:

```
7
3
```

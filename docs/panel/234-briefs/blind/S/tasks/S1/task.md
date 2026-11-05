# The task

Write a Heroes program that reads the path of a text file from `args()[0]` and, for each line of the file that starts with `ERROR `, prints the part of the line after its first `: `. A line that starts with `ERROR ` and holds no `: ` prints `?` instead. Lines are separated by `\n`, and the last line may have no `\n` after it. Print nothing else.

Use the package `core/strings` (see `brief.md`).

## Example

With this file:

```
INFO start: ok
ERROR disk: full
ERROR no colon here
WARN x: y
ERROR net: timeout: retry
```

the program prints:

```
full
?
timeout: retry
```

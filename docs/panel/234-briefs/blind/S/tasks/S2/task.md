# The task

Write a Heroes program that reads the path of a text file from `args()[0]`. Each line of the file is a setting `key = value`. Skip a line that is empty or holds only spaces and tabs, and a line whose first character that is not a space or a tab is `#`. For every other line, print `key|value`, where `key` is the text before the line's first `=` and `value` the text after it, each without the spaces and tabs at its two ends; a line with no `=` prints `bad: N` instead, `N` being its line number counting from 1 (skipped lines are counted). After the last line, print `keys: K`, `K` the number of `key|value` lines printed. Lines are separated by `\n`, and the last line may have no `\n` after it. Print nothing else.

Use the package `core/strings` (see `brief.md`).

## Example

With this file:

```
# settings
name = heroes

 # indented comment
version=0.6
broken line
path = /usr/local = x
```

the program prints:

```
name|heroes
version|0.6
bad: 6
path|/usr/local = x
keys: 3
```

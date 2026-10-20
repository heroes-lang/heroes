# Task 7: a setting looked up

The program receives two arguments: the path of a configuration file and a key, a section name, one
`.` and an entry's name, such as `server.port`.

A line of the file ends at a line feed, and the file's last line may or may not end with one; a
carriage return is an ordinary character. Each line is exactly one of:

- an empty line, or a line whose first character is `#`, which is ignored;
- a section header, `[`, a section name and `]`, which opens that section;
- an entry, a name, `=` and a value, the value being every character after the first `=`, possibly
  none.

A section name and an entry's name are each one or more of the characters `a` to `z`, `A` to `Z`,
`0` to `9` and `_`, compared byte for byte. An entry belongs to the section most recently opened,
and an entry before any section header is a bad line. A section may be opened more than once, and an
entry whose section and name were set before replaces the earlier value.

The whole file is checked before the key is looked up. If a line is none of the three, print `bad
line N`, where N is the first such line's number counting from 1, and exit with status 1. Otherwise,
if the key names an entry, print its value and exit with status 0; if it names none, print `not
found` and exit with status 3. If the program does not receive exactly two arguments, the key is not
a section name, one `.` and a name, or the file cannot be read, print nothing and exit with status
2; these are checked before any line of the file. The file is UTF-8 text.

Example. A file holding

```
# the server
[server]
port=8080
host=example.org
[client]
retries=3
```

with the key `server.host` prints `example.org`.

canary heroes-task:e4d82b993c17d25a94e54072845d2515

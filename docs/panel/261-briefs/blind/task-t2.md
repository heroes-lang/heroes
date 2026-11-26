# Task: a table of command handlers

Write one program, `main.hero`, in the language `spec.md` describes, from that document alone. It runs commands from
a fixed list, one per line, against a small key-value store.

The store holds a text under each key, and counts the commands that changed it. The commands, each a name and its
words after one space:

- `SET <key> <value>`: keeps `<value>` under `<key>`; answers `OK`.
- `GET <key>`: answers the value, or `(nil)` where the key holds none.
- `DEL <key>`: removes the key; answers `1`, or `0` where it held nothing (which changes nothing).
- `INCR <key>`: reads the value as a whole number written in decimal digits (a missing key reads as 0), adds 1, keeps
  the result and answers it; answers `ERR not a number` where the value is not one, changing nothing.
- `KEYS`: answers every key, in ascending byte order, separated by one space.
- `COUNT`: answers how many commands have changed the store so far.
- Any other name: answers `ERR unknown <name>`.

**How it must be built.** The commands are listed in one place, a table that **binds each command's name to the
function that runs it**, so that adding a command is one entry in that table and one function. The program finds a
command's function by its name in that table and calls it; it must not choose the function by comparing the name in
a chain of `if` or in a `match`. Each command's function receives the store and the command's words, and changes the
store only where its command does.

The commands, in this order, each answered by one line:

1. `SET a 1`
2. `SET b hello`
3. `GET a`
4. `INCR a`
5. `INCR b`
6. `GET c`
7. `DEL b`
8. `KEYS`
9. `COUNT`
10. `FLY`

Write the program whole, in the structure `brief.md` gives. Then say what it prints, line by line, and whether you
expect it to compile as written; and for each function in the table, why each of its parameters is written the way it
is.

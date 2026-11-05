# The task

Write a Heroes program whose arguments, `args()`, are names, some of them repeated; a name may hold any character but a line end, an apostrophe included. Create, in an in-memory SQLite database, a table whose name column is `UNIQUE`, and insert every argument into it; an insert the `UNIQUE` constraint refuses is a duplicate, and the program goes on with the next argument. Then print `added: A` and `duplicates: D`, the number of rows inserted and the number of inserts refused. Print nothing else.

Use the package `core/database/sql` (see `brief.md` and `card.md`), with a database held in memory that the program opens and closes. The program reads no file: its data are its arguments, `args()`.

## Example

With these arguments, one per line:

```
ann
bob
ann
```

the program prints:

```
added: 2
duplicates: 1
```

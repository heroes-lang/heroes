# The task

Write a Heroes program whose first argument, `args()[0]`, is an integer, the threshold, and whose further arguments come in pairs, a name and then an age written in decimal. Insert every pair into a table of an in-memory SQLite database, the age in an `INTEGER` column; then print, one per line, the names whose age is greater than the threshold, in the order SQL's `ORDER BY name` gives, computed by one SQL query. A name may hold any character but a line end, an apostrophe included. Print nothing else.

Use the package `core/database/sql` (see `brief.md` and `card.md`), with a database held in memory that the program opens and closes. The program reads no file: its data are its arguments, `args()`.

## Example

With these arguments, one per line:

```
30
ann
31
bob
25
carl
40
```

the program prints:

```
ann
carl
```

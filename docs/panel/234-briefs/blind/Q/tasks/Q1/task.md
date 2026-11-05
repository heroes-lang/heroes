# The task

Write a Heroes program whose arguments, `args()`, are integers written in decimal, each with an optional leading `-`. Insert every argument into an `INTEGER` column of a table of an in-memory SQLite database; then print two lines computed by one SQL query over that table: `count: N`, the number of rows, and `sum: S`, their sum, `0` when there are no rows. Print nothing else.

Use the package `core/database/sql` (see `brief.md` and `card.md`), with a database held in memory that the program opens and closes. The program reads no file: its data are its arguments, `args()`.

## Example

With these arguments, one per line:

```
3
5
-2
10
```

the program prints:

```
count: 4
sum: 16
```

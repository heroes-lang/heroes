# Task 8: accounts and transfers

The program receives one argument: the path of a file of operations, one per line, run in order on
a set of accounts that starts empty:

- `open NAME AMOUNT` opens an account called NAME holding AMOUNT;
- `deposit NAME AMOUNT` adds AMOUNT to NAME's account;
- `withdraw NAME AMOUNT` takes AMOUNT from NAME's account;
- `transfer FROM TO AMOUNT` takes AMOUNT from FROM's account and adds it to TO's.

A NAME is one or more ASCII letters, compared byte for byte; an AMOUNT is one or more decimal
digits, so `007` is seven; the words of a line are separated by exactly one space. A withdrawal or a
transfer whose AMOUNT is larger than the balance it takes from is rejected: it changes nothing, and
the program counts it. A balance may become exactly zero. The file is UTF-8 text, and it is
promised, with nothing to check, that every AMOUNT and every balance fits in a signed 64-bit
integer.

A line is bad if it is not one of the four forms, if it opens an account that is already open, if it
names an account that is not open, or if a transfer's FROM and TO are the same account. A line ends
at a line feed, and the file's last line may or may not end with one; a carriage return is an
ordinary character, and an empty line is bad.

Exit with status 0 after the last line, having printed one line per account, its name, one space and
its balance, names in ascending byte order, then a line `rejected K` with the number of rejected
operations. If a line is bad, print `bad line N`, where N is its number counting from 1, print
nothing else, and exit with status 1; the first bad line decides. An empty file prints `rejected 0`.
If the program does not receive exactly one argument, or the file cannot be read, print nothing and
exit with status 2.

Example. A file holding

```
open ann 100
open bob 5
transfer ann bob 30
withdraw bob 50
deposit bob 1
```

makes the program print:

```
ann 70
bob 36
rejected 1
```

canary heroes-task:ff4695a1b932e8ad107004ca43384882

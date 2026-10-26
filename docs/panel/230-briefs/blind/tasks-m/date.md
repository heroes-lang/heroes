# A program to write: a day of the year

The program is run as a command, with the arguments given, by somebody who reads what it prints and
the status it exits with.

The program receives one argument, a date written `YYYY-MM-DD` with exactly four, two and two
decimal digits, such as `2024-03-01`, and prints which day of its year it is, followed by a line
feed: `1` for January 1. A year is a leap year when it is divisible by 4 and not by 100, or divisible
by 400. An argument that is not written so, or a date the calendar does not hold, such as
`2023-02-29` or `2023-13-01`, is an error, and the program reports it.

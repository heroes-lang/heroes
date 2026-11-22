# Task

Write `main.hero`, a whole program, in which one thread keeps a count per name and four other threads
ask it to add to those counts while they run.

- The **keeper** is a thread of its own. It alone holds the counts, a map from a name to an `i64`,
  every count starting at 0, and it answers requests one at a time.
- Each of four **workers**, numbered 0 to 3, is a thread of its own. Worker `w` sends the keeper 100
  requests, `i` from 0 to 99, one after another. Request `i` names `["red", "green", "blue"][(w + i) % 3]`
  and an amount `i % 5 + 1`. The keeper adds the amount to that name's count and replies with the
  name's count after the addition; the worker waits for that reply before it sends its next request.
  When it has sent all 100, the worker's thread answers the largest reply it received.
- `main` starts the keeper and the four workers. Once all four workers have answered, it makes the
  keeper stop, gets the final counts from it, and prints one line per name, in the order `red`,
  `green`, `blue`, then the largest of the four workers' answers:

      <name>: <count>
      largest: <n>

  and the program ends normally, not by an abort, with every thread it started waited for.

Give `main.hero` whole, then say whether `heroes build main.hero` accepts it, every line the program
writes, and how it ends.

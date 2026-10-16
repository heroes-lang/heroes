## C (`counter.h`)

Keep two counters, each a `struct counter` initialised with `counter_init`, in a record of your
own, `Stats`, together with the number of events counted so far. Write a function that sets up a
new counter for its caller, a function that increments a counter's `n` while holding its lock
(`counter_lock` before, `counter_unlock` after), and a function that gives a counter's value
doubled, using `counter_value`. From the program's entry point, set
up a `Stats`, increment its first counter three times and its second twice, counting each increment
as an event, then print the first counter's doubled value, the second's, and the number of events,
one per line.

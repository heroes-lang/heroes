# Task t1: four small programs, written from the specification alone

You have the specification of the language Heroes (`spec.md`). Nothing else is yours: no other file of this folder but
those this task names, no tool but Read and Write. Write every file whole, under its path, as the structure `brief.md`
gives asks. Each job stands alone, in a folder of its own.

The language provides four C headers this task uses. Each function below takes nothing and answers a C `int64_t`:

- `hero_time.h` declares `int64_t hero_time_monotonic_ns(void);`: nanoseconds on a clock that never goes back, or -1
  where the clock cannot be read.
- `hero_os.h` declares `int64_t hero_os_pid(void);`: the running process's id.
- `hero_net.h` declares `int64_t hero_net_stop_asked(void);`: 1 once something asked the process to stop, else 0.
- `hero_activation.h` declares `int64_t hero_activation_count(void);`: how many descriptors a service manager handed
  the process when it started, 0 for a process started by hand.

## W1, a package over the clock

In the folder `w1/`, write a package named `ticks` in `w1/ticks/`, whose module `clock` offers `mark() -> i64`, the
clock's reading now, and `elapsed_ns(since: i64) -> i64`, the nanoseconds from a reading `mark()` gave to now, both
written over `hero_time_monotonic_ns`; and a program `w1/main.hero`, with whatever else the program needs beside it to
use the package from the folder `w1/ticks/`, that takes one reading with `mark()`, then calls `elapsed_ns` with it
twice and prints `true` if the second answer is at least the first, else `false`.

## W2, a package over the process id

In the folder `w2/`, write a package named `ident` in `w2/ident/`, whose module `me` offers `pid() -> i64` over
`hero_os_pid`; and a program `w2/main.hero`, with whatever else the program needs beside it to use the package from
the folder `w2/ident/`, that prints `true` if `pid()` is above 0, else `false`.

## W3, a program with no package

In the folder `w3/`, write a program `w3/main.hero`, using no package, that prints `stop asked: ` followed by `true` if
`hero_net_stop_asked()` answers 1, else `false`.

## W4, a second program with no package

In the folder `w4/`, write a program `w4/main.hero`, using no package, that prints `handed: ` followed by the number
`hero_activation_count()` answers.

## For each job, then

- **prediction**: whether the compiler accepts what you wrote as it stands, every line the program prints, and how it
  ends; and where the compiler would refuse something you might have written otherwise in this job, what it would say
  and which file it would name;
- **confidence**: 1 (a guess) to 5 (the specification says so).

Then, once, in the structure `brief.md` gives: every place where the specification left you a choice, the choice you
made, and what the other would produce.

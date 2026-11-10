# Task

Write a program in one folder, as the files it needs:

- a module `money` that offers a function `cents(euros: i64) -> i64`, returning the euros in cents,
  computed through a helper `scale(n: i64) -> i64` that multiplies by 100;
- the helper `scale` must be usable by `money` and by no other module of the program, so that a
  module of the program that names it is refused by the compiler, not merely discouraged;
- `main.hero`, which prints `money.cents(3)`, that is `300`, when built with `heroes build main.hero`;
- `probe.hero`, a second root, which tries to call the helper `scale` directly, and which the
  compiler must refuse when built with `heroes build probe.hero`.

Give every file, whole, with its path, and say what the compiler does with each root.

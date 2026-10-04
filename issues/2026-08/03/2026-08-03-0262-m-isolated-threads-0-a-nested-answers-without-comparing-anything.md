- [ ] **M-isolated-threads.0** | **A nested `hero_array_eq` answers `true` without comparing anything. Why is that not a bug?** Read `hero_array_eq` in `runtime/parts/array.c` from `if (hero_eq_running)` to the end, and say which one of these makes it sound: that `true` is the identity of `&&`, that the outermost call drains the queue before it returns, or that both are needed. Then say what `hero_eq_len = base` on the way out is for, given the answer is already known by then

    **Where to look:** runtime/parts/array.c (the comment above `HeroEqWork`) · docs/panel/076
    **Why it matters:** this is how a language with no recursion limit compares a value of unbounded depth, and it is the state that becomes per-thread the day Part 7.13 lands

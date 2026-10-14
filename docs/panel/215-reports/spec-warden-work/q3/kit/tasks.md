# Three programs

`a2.hero` and `c.hero` each use one C header from this folder; every function those headers name is
defined in them, so nothing else needs to be linked. `u.hero` uses the system's `sys/utsname.h`.

## A2 (`union.h`)

Build a `struct tagged` whose `kind` is 2 and whose union `via` is zero. Pass it to `tg_kind` and
print the number `tg_kind` returns.

## U (`sys/utsname.h`)

Print the operating system's name. The header, `sys/utsname.h`:

```c
struct utsname {
    char sysname[256];
    char nodename[256];
    char release[256];
    char version[256];
    char machine[256];
};
int uname(struct utsname *name);   /* fills *name, returns 0 on success */
```

## C (`counter.h`)

Keep two counters, each a `struct counter` initialised with `counter_init`, in a record of your
own, `Stats`, together with the number of events counted so far. Write a function that sets up a
new counter for its caller, a function that increments a counter's `n` while holding its lock
(`counter_lock` before, `counter_unlock` after), and a function that gives a counter's value
doubled, using `counter_value`, without changing the counter. From the program's entry point, set
up a `Stats`, increment its first counter three times and its second twice, counting each increment
as an event, then print the first counter's doubled value, the second's, and the number of events,
one per line.

# Three programs

Each program uses one C header from this folder; every function a header names is defined in that
header, so nothing else needs to be linked.

## A1 (`sockpath.h`)

Build a `struct sock_path` whose `family` is 1, whose `path` holds the text `/tmp/heroes.sock`, and
whose every other byte is zero. Pass it to `sp_send` and print the number `sp_send` returns.

## A2 (`union.h`)

Build a `struct tagged` whose `kind` is 2 and whose union `via` is zero. Pass it to `tg_kind` and
print the number `tg_kind` returns.

## B (`counter.h`)

Create a `struct counter` and initialise it with `counter_init`. Write a function that increments the
counter's `n` while holding its lock (`counter_lock` before, `counter_unlock` after). Call that
function three times from the program's entry point, then print `counter_value`.

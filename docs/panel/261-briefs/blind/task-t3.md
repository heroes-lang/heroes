# Task: a state machine

Write one program, `main.hero`, in the language `spec.md` describes, from that document alone. It runs a turnstile
through a fixed list of events.

The turnstile keeps its state, `locked` or `open`, the coins it has taken and the people who have passed; it starts
`locked`, with none of either. Two events, `coin` and `push`:

- `locked` and `coin`: it takes the coin and opens.
- `locked` and `push`: nothing changes.
- `open` and `coin`: it takes the coin and stays open.
- `open` and `push`: one person passes and it locks.

**How it must be built.** The four pairs of a state and an event are listed in one place, a table that **binds each
pair to the function that handles it**, so that adding a state or an event is entries in that table and their
functions. The program finds a pair's function in that table and calls it; it must not choose the function by
comparing the state or the event in a chain of `if` or in a `match`. Each function receives the turnstile and changes
it only where its pair does.

The events, in this order: `push`, `coin`, `coin`, `push`, `push`, `coin`. After each, print one line:
`<event> -> <state> coins <coins> passed <passed>`.

Write the program whole, in the structure `brief.md` gives. Then say what it prints, line by line, and whether you
expect it to compile as written; and for each function in the table, why each of its parameters is written the way it
is.

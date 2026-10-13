# Panel 213: where the blind seat's samples come from

Kept outside `samples/`, which the seat reads, so it is not told which frame is which.

- `program.hero` is the program, a bug at its line 11 (`winner @ p.name` outside the `if`); the seat is given the symptom (`samples/symptom.txt`) and not the program, since the source alone shows the bug (critic, pass 1).
- `frame-today.txt` is lldb-2103.0.34.103's real output, `frame variable` at `program.hero:12`, the program built at `-O0`, 2026-10-12; its first line, the stop, is written by hand from the stop lldb printed. It is `samples/frame-2.txt`.
- `frame-names-only.txt` is route E alone, written by hand from it: the author's names for the author's bindings and fields, every value as C shows it, nothing hidden. It is `samples/frame-1.txt`.
- `frame-all-routes.txt` is every route together, written by hand: the author's names, Heroes values, and only the bindings in scope at line 12 (`p` is not, since the loop has ended). It is `samples/frame-3.txt`.

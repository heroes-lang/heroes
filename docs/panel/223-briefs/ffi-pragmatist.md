# Panel 223, ffi-pragmatist

Read `00-shared.md` first. Your seat: the C a real binding needs (veto on ABI breakage).

- Write and compile, on this Mac and in `heroes-linux-arm64:latest`, the C that route A writes for
  `struct bf` and for a struct of a real header with bit-fields (find one: `grep -rnE ':[[:space:]]*[0-9]+[[:space:]]*;'`
  over the SDK's or the container's `/usr/include`, and name the one you pick), with every named
  bit-field kept bit for bit across the lend (a test that sets each to a value and reads it back in
  C after the lend). Then the same for route B.
- Say whether reading and writing back a byte a bit-field holds can change what C reads of that
  bit-field under any ABI clang targets here (`-target` for the four CI legs), and whether a
  `volatile` or `_Atomic` bit-field changes the answer (C11 § 6.7.2.1).
- Your verdict per route: approve / object / veto, the condition, one falsifiable prediction.

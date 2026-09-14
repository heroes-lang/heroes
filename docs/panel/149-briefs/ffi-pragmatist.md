# Panel 149 — brief for the ffi-pragmatist

Read `docs/panel/149-briefs/00-shared.md` first. It carries every measurement
already taken; do not repeat one, extend it.

## Your seat

design.md §1.11 and §4.19: there is no standard library, everything comes from C.
You judge against the C a real binding would need, and you compile it. You have a
veto on ABI breakage.

## The task, and it is a writing task before it is a judging one

**Write the C, and write the Heroes binding against it, and compile both.** The
compiler builds from the seed in about 3.4 s:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. Build in a copy of
the tree. `heroes build <file>.hero -o <bin>` needs `HEROES_RUNTIME=<repo>/runtime`
set, or it cannot find the runtime.

Three real headers, not contrivances, and the question each one answers:

1. **`getaddrinfo` / `struct addrinfo`** (`<netdb.h>` on this Mac). It is the
   canonical case: a struct handed back through an out-parameter, carrying
   pointers the caller must release with `freeaddrinfo`, and one of them
   (`ai_next`) points at another `struct addrinfo`. Write the binding as Heroes
   would have to declare it. Report what the current vocabulary forces you to
   write — `ptr`, a handle, a `partial` record — and whether the proposed rule
   would fire on it, correctly or spuriously. **This is the case the defect says
   is real; confirm or falsify that.**

2. **A struct carrying TWO handles of two types.** Find one in a header that is
   actually installed on this machine, and say which header and which struct. If
   you cannot find one, say so in your own words and say what you searched — a
   negative claim rests on the searcher's vocabulary. Then judge R3 against what
   you found: is refusing such a returned type a refusal a binding author would
   hit, or one nobody reaches?

3. **A fixed array of handles.** `Slot[4]` is legal today and the shared brief
   measured it reaching the runtime unmarked. Find, or fail to find, a real
   header whose struct holds an array of pointers the caller must free. Then
   answer the question the compiler seat is also asked: if C fills two of four
   and leaves two null, can any static count be right? What would the binding
   author have to write instead?

## The questions that are yours

- **Does the widened rule refuse a correct binding?** That is the objection that
  killed the inferred form at panel 148 and brought `borrows` into the language.
  Find the binding the widened rule would refuse wrongly, or report that you
  looked and did not.
- **`ai_next` is a self-referential pointer.** `struct addrinfo` contains a
  `struct addrinfo *`. Heroes refuses a record containing itself by value, but
  a `ptr` field is legal. Does the reachability walk have to follow a `ptr`? It
  cannot — a `ptr` is opaque — so say plainly what the rule therefore cannot see,
  and whether that hole is worse than the one being closed.
- **ABI.** Does anything proposed change the emitted C for a binding that
  already compiles? Check the three shipped bindings under `examples/` and say
  which, if any, would newly need a mark.

## Predict something falsifiable

One prediction, with the command that would settle it.

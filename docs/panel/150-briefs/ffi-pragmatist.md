# Panel 150 — brief for the ffi-pragmatist

Read `docs/panel/150-briefs/00-shared.md` first, including its three required
sittings. You judge against the C a real binding needs, and you compile it. Veto
on ABI breakage.

**Your last sitting's work decided panel 149.** You compiled raylib's `Font`
against the installed header and it withdrew a seat's veto by that seat's own
condition. Do that again.

## Q1 — the `ptr` producer

1. **`getaddrinfo` is your case and you already proved it.** At panel 149 you
   measured that `struct addrinfo` has no typedef in this Mac's `netdb.h`, so the
   handle form does not compile and `ptr` is the ONLY spelling that works. Write
   that binding again against the real header, with `freeaddrinfo(ai: ptr
   consumes)`, and show what a type-keyed Q1 rule would demand of it. Would the
   demand be right?
2. **Now find where the same rule is WRONG.** Every `ptr` is one Heroes type, so
   a group that consumes one `ptr` demands a mark on every `ptr` producer in it.
   Find a real installed header where one group has both an owning `ptr`
   producer and a borrowing one, or a `ptr` that is not memory at all — an
   opaque token, an index, a file descriptor cast. Compile it. **If the rule
   would refuse a correct binding, that is the panel-148 objection returning and
   you should say so.**
3. **`ai_next` again.** A `ptr` has no pointee, so the walk stops dead. Say what
   a Q1 rule still cannot see, and whether marking the outer `ptr` is worth
   anything when the chain behind it is invisible.

## Q2 — the fixed array

4. **Write the C both ways and compile both.** A struct holding an array of
   owned pointers with ONE destructor for the whole thing, and one where each
   element has its own release. Which does C actually ship? You scanned 3400
   headers last time and found `jpeglib.h` and `<net/route.h>`; say whether
   either offers a per-element release, and what you searched for.
5. **The question that decides it**: is a program that releases the ELEMENTS of a
   C composite ever correct, or is it always a binding error? If it is always an
   error, Q2 is a refusal and not a count.

## ABI

6. Does anything proposed change the emitted C for a binding that compiles today?
   Read `examples/` rather than guessing; the shared brief measured zero `ptr`
   producers there and you should confirm it.

## Predict something falsifiable

One prediction with the command that would settle it.

# Panel 176 — ffi-pragmatist

Read `00-shared.md` first. Your seat holds design.md §1.11 and §4.19 and a veto
on ABI breakage or categorically harder bindings. Work in
`<scratchpad>/176-ffi-pragmatist/`. The Linux images are `heroes-linux` and
`heroes-linux-arm64`, Docker was running when this was written; mount YOUR
directory, not the trunk. The Windows box is off.

## What to write and compile

1. **The census of transfers, from real headers.** Your panel 175 census found
   releasers by name; it could not see transfers, because they are named for
   what they do to the receiver (`_add`, `_set0`, `_push0`, `_append`). In the
   macOS SDK, Homebrew and a Linux container, find consuming calls that TRANSFER
   a handle into another value, and say for each whether the header's own text
   says so. Say how you searched and what the search cannot see. Then do the
   same for **reference-adding** calls (`_ref`, `_retain`, `_up_ref`,
   `CFRetain`): how often does a real acquisition have one?
2. **Bind the three routes for real.** For each of V1, V2 and V3 in the shared
   brief, and R1 for references, write the binding a real library would need —
   json-c's `json_object_object_add` and `json_object_get`/`json_object_put`,
   OpenSSL's `SSL_set0_rbio`, and one reference-counted library of your choice
   (GObject, CoreFoundation or OpenSSL's `X509_up_ref`). Count the lines and the
   declarations each route costs a binding author. A route that multiplies
   declarations is §1.11's glue, and your seat is the one that says where the
   line is.
3. **Question 3 in C.** For `sqlite3_bind_text`, write each mode's call in C and
   say which Heroes spelling, under each candidate answer to Question 2, is
   TRUE of the call it makes. The measured wrong answer (`lent_static.hero`) is
   what a false one costs.
4. **Question 4 in C.** Find real functions whose out-parameter is
   `const char **` and whose documentation says the caller frees what it gets.
   If there are none, `owned` there is always false and a refusal is right; if
   there are, the emitter must keep the qualifier. Say which, with the headers.

Give a verdict per route, the cost, one falsifiable prediction with the
milestone at which it is checkable, and the condition under which you would
change your mind.

# Panel 176 — shared brief: what a declaration says a C call does with a handle

2026-09-23, M-agreed-retention, after step 3 (`a747e5a2`). Full panel. The tree
is frozen at `a747e5a2` from the moment these briefs exist until the synthesis
is written.

**Every seat works in its own directory, `<scratchpad>/176-<seat>/`**, where
`<scratchpad>` is the path your prompt gives you: copy the trunk there from HEAD
(`git archive HEAD`, plus this untracked directory), build your own compiler
inside it from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`, about three seconds), set `HEROES_RUNTIME=<your copy>/runtime`, and
never use the trunk's `./heroes`, build in the trunk, or read another seat's
directory while it works. Your report goes to `docs/panel/176-reports/<seat>.md`.

The reproducers are in this directory, and every number below was produced by
running them from here on 2026-09-23 while this brief was written, with the
compiler built from the seed at `a747e5a2`. Programs over json-c and OpenSSL
need `--include`/`--library` pointing at Homebrew:
`--include /opt/homebrew/include --library /opt/homebrew/lib` for json-c and
`--include /opt/homebrew/opt/openssl@3/include --library /opt/homebrew/opt/openssl@3/lib`
for OpenSSL (`heroes build` takes them; `heroes check` does not, and exits 2).

## Where panel 175 left this

Panel 175 (`docs/panel/175-a-consuming-call-is-three-things-and-the-runtime-speaks-after-it-listens.md`)
fixed the SHAPE of defect 075's repair and sent the rest here. The shape: the
runtime's set of live handles remembers, per handle, the set of releaser NAMES
its acquiring call's mark allows (`acquires sqlite3_close | sqlite3_close_v2`,
because nine real pairs of releasers are interchangeable for one acquisition,
measured by panel 175's ffi-pragmatist), compared by content (a function's
address differs per translation unit), checked BEFORE C runs, the newest mark
winning when C hands an address out again. **It cannot land alone**, and this
sitting exists because of why.

## Question 1 — `consumes` is three things, and only the author can say which

Today `consumes` after a parameter says *the call ends that value's life*. Three
different C contracts are written with it:

| reproducer | what the C call does | today | under route A (panel 175's prototype) |
|---|---|---|---|
| `popen_fopen.hero` | ends a `popen` stream with `fclose` — **a wrong release** | 0, 0 B | 134, *given back to `fclose` … `acquires pclose`* |
| `vk_cross.hero` + `vk.h` | a Vulkan-shaped crossing between two releasers that take a context | 0, 0 B | 134 |
| `xfer_cj.hero` + `cj.h` | cJSON's add-to-object: **hands the child into the parent**, which frees it | 0, 0 B | **134**, a correct program refused |
| `xfer_jsonc.hero` | real json-c 0.19 `json_object_object_add`, *"transferring ownership"* (`json_object.h:381`) | 0, prints `add 0 fields 1` | **134** |
| `xfer_ssl.hero` | real OpenSSL 3 `SSL_set0_rbio`, *"transfers ownership of rbio to ssl"* (its `SSL_set_bio(3)` page, as quoted by panel 175's critic; not re-read here) | 0 | **134** |
| `refcount.hero` + `rc.h` | a reference-counted object, `obj_ref` then two `obj_unref` — **drops one reference** | **134**, 396 B, *given back that were never taken*: a correct program refused TODAY (defect 079) | 134 |

Three runs each, `heroes build -O0` then the binary; the route A column was run
with panel 175's completeness critic's build of the compiler-engineer's
prototype (`<scratchpad>/175-completeness-critic/heroes-A` against
`…/A/runtime`).

**Why no rule over the declarations can separate them**, measured by panel
175's critic and re-derived here: in each program the consuming functions no
`acquires` names are exactly the functions in question — `fclose`,
`buf_destroy_pooled`, `cJSON_AddItemToObject`, `json_object_object_add`,
`SSL_set0_rbio`. A wrong release and a correct transfer have the same standing.
Three candidate rules were built as compilers and each fails a real library
shape (panel 175's `completeness-critic.md` § 1). `selfhost/check/acquiring.hero:257`
already says what follows: *no C header states which, so the binding author
states it*, exactly as panel 148 made the author state `acquires` or `borrows`
on the producer side.

**And the reference is the third meaning.** `getter_wrong.hero` + `g.h` is a
getter WRONGLY marked `acquires` (it returns a pointer it keeps): today 0, and
the second close its own message invites, `getter_wrong_repaired.hero`, is
**134, 396 B, caught before C**. A reference count per address (panel 175's
ffi-pragmatist's `runtimeA2`) makes `refcount.hero` run, and makes
`getter_wrong_repaired` a raw libmalloc double free, **133 with zero bytes**
(measured by panel 175's critic, § 3.7). So "begins a life" and "adds a
reference" are two declarations too.

**How much of the world is transfer-shaped**, by name only and so an upper bound
on nothing: json-c's `json_object.h` has **12** creators named `json_object_new_*`
and **5** adders (`json_object_object_add`, `_add_ex`, `json_object_array_add`,
`_put_idx`, `_insert_idx`); OpenSSL 3's headers carry **124** distinct function
names in its `set0`/`add0`/`push0`/`own0` convention (`grep -ohE
'\b[A-Za-z0-9_]*_(set0|add0|push0|own0)[A-Za-z0-9_]*\s*\(' …/openssl/*.h | sort -u`;
panel 175's critic reported 130 declarations by its own command). **In this
tree**, no program in `examples/` has a consuming function that transfers
(panel 175's brief: every handle type there has exactly one consuming function).

**The routes, enumerated from panel 175's critic § 5 and from the producer-side
precedent**, and a route nobody listed is the most valuable thing a seat can
bring:

- **V1 — a second consuming word.** `consumes` keeps its meaning and names a
  RELEASE, which must be in the acquiring mark's set; a transfer is marked with
  another word. The spelling is this sitting's to test, not the brief's.
- **V2 — transfers inside the set.** No new word: the acquiring mark names every
  call that may end the life, releasers and transfers alike
  (`acquires cJSON_Delete | cJSON_AddItemToObject`). Over json-c that is 12
  creators each naming up to 6 calls.
- **V3 — the transfer names its receiver.** The consuming parameter says which
  other parameter takes the life over, and the runtime moves the obligation
  there.
- **R1 — a reference is its own producer mark**, beside `acquires` and
  `borrows`, so the runtime counts per address only where a declaration says a
  reference was added.

## Question 2 — two declarations of one C function

A C function may be declared in two modules of one program, and nothing relates
the two. Run from this directory:

| reproducer | the two modules disagree on | `check` | `run` |
|---|---|---|---|
| `xmod-lent/` | `lent` against unmarked | 0 | 0 |
| `xmod-owned/` | `owned my_free` against `owned other_free` | 0 | 0 |
| `xmod-xacquires/` | `acquires h_close` against `acquires h_close2` | 0 | 0, and route A leaves it at 0 because each module keeps its own promise (panel 175) |

Two records over one tag are already refused program-wide (`duplicate_tag`,
`selfhost/check/decls.hero:334`, `one_tag_one_type`), so the checker already
sees one resolved program. **The one legal repeat in the tree** is
`tests/golden/surface-fixtures/twoarity/`, `printf` at two arities with both
formats `lent`, which is panel 094's refutation of a rename clause and must stay
legal. A census of 423 `extern` function declarations in 187 files found no
other C function declared twice inside one program (M-agreed-retention step 1).

## Question 3 — one C function, a contract chosen per call

`sqlite3_bind_text`'s fifth argument decides what SQLite does with the text:
`SQLITE_STATIC` (`nullptr`) keeps the caller's bytes, `SQLITE_TRANSIENT` copies
them, a function frees them with it. One Heroes declaration reaches the two
pointer modes or the function mode and never all three (`ptr_transient.hero`
`check` 0; `fn_fn.hero` `check` 0; the crossed spellings `type_mismatch`; two
declarations in one module `declared_twice`, `twice_one_module.hero`). **And the
declaration an author is likeliest to write is a wrong answer at exit 0**:
`lent_static.hero` declares `text: cstr lent`, true under TRANSIENT, passes
`nullptr`, and prints `stored=XXXX…` — the bytes of a later allocation — three
of three, where it bound `hello, sqlite`. The route panel 094 left open, one
module per mode, is exactly what a rule answering Question 2 would forbid.

## Question 4 — defect 078, a mark that says what the header's type denies

`s4_cstr_owned.hero` declares `@out: cstr owned free_out` over a C
`fill_out(const char **out)`: `check` 0, `build` exit **2**, `internal error:
compiling the generated C failed`, clang's own sentence inside it (*passing
'char **' to parameter of type 'const char **' discards qualifiers*). The same
cell without `owned` builds. `.claude/rules/c-boundary.md` names the one class
of clang failure that is the author's own `extern`'s and not the compiler's, and
this is that class reported as the other. Whether `owned` on a `const char **`
cell is ever true, and what the diagnostic says, is the question.

## What is not this sitting's

Defect 077 (a handle given back after C reused its address frees the new one)
is a question about what identifies one life, and gets its own sitting. Defects
080 to 082 are repaired in a lane on panel 175's route E and wait for the
Windows box. Report either only if you find a reason this sitting's resolution
changes it.

## Unrun, and named

Windows, for every route: the box is off. Any reader-side effect of any
spelling, except what the llm-ergonomist measures here. What a real C++ or
Vulkan binding would need beyond `vk.h`'s declaration shape.

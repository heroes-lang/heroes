# Panel 176 — llm-ergonomist

Your input is **the specification and this file, and nothing else**. Both are
handed to you as copies OUTSIDE the repository, so that nothing but them enters
your context; open no other file. Your verdict is an experiment. You hold a veto
on a non-local construct. Write your report where your prompt says.

The specification as handed to you is variant **X**. Each task below names the
variants it compares; every variant is X with the change it names in section
13. Do each task once per variant it names, and write down what you produced
before comparing.

## The variants

- **V1** adds, after the `consumes` sentence: *`transfers` after a parameter
  says the call hands that handle's life to another value, which C ends; the
  program owes nothing more for it, and no `acquires` names the call.* and the
  `consumes` sentence reads *… says the call ends that value's life with one of
  the calls its `acquires` named …*.
- **V2** rewrites the `acquires` sentence: *`acquires cJSON_Delete |
  cJSON_AddItemToObject` after a result … names every call that may end that
  handle's life, a release or a call that hands it into another value; giving it
  to any other aborts.*
- **V3** adds: *`consumes into obj` after a parameter says the call hands that
  value's life to the parameter `obj`, and `obj`'s own release then ends both.*
- **R1** adds, beside `borrows`: *`retains obj_unref` after a result says the
  call adds one more reference to a handle already live, so the program owes one
  more `obj_unref`, and the handle's life ends at the last.*
- **A2** adds, at the end of section 13's first paragraph about groups: *Every
  declaration of one C function in a program carries the same marks on the
  parameters and results they share.*

## The tasks

1. **A child into a parent** (X, V1, V2, V3). cJSON: `cJSON *cJSON_CreateObject(void)`
   ended by `void cJSON_Delete(cJSON *)`, and `cJSON_AddItemToObject(cJSON
   *object, const char *string, cJSON *item)`, after which `object` owns `item`
   and deleting `object` deletes both. Write the binding and a program that
   makes a parent and a child, adds the child, and deletes the parent. Then say,
   from the document alone, what happens if a later edit also deletes the child.
2. **The wrong closer** (X, V1, V2, V3). The same as panel 175's first task: a
   stream opened with `popen` (ended by `pclose`) and a file opened with `fopen`
   (ended by `fclose`), both `FILE *`, passed to ONE function `finish` that
   closes whatever it gets. What happens when it runs?
3. **Two references** (X, R1). `obj *obj_new(void)`, `obj *obj_ref(obj *)`
   which adds a reference and returns the same pointer, and `void obj_unref(obj
   *)` which frees at the last. A program takes a second reference and gives
   both back. What do you write, and what does the document say happens?
4. **One function, two modules** (X, A2). Module `a` declares `keep(s: cstr
   lent)`; module `b` declares `keep(s: cstr)`, the same C function. What does
   the document say about the program? And `printf` declared in one module with
   `(format: cstr lent, value: i64)` and in another with `(format: cstr lent,
   value: cstr lent)`?
5. **Three modes of one call** (X, A2). `sqlite3_bind_text(stmt, i, text, n,
   destructor)`: `SQLITE_STATIC` is a null destructor and SQLite keeps `text`;
   `SQLITE_TRANSIENT` makes SQLite copy it before returning; a function makes
   SQLite free it with that function. Write the bindings a program needs to use
   the first two modes, and say under each variant whether you can.
6. **Three questions scored today** (X), registered as predictions in the
   specification's budget ledger: (a) bind `void *make(void)` whose result the
   program must give back to `void release(void *)` — which spelling do you
   choose for the result? (b) bind `malloc`/`free` beside
   `sqlite3_malloc`/`sqlite3_free` so that a block from one family cannot reach
   the other's free — what do you write? (c) mark a C function `void
   eat(void *p)` that frees what it is handed — what do you write on `p`?

Report, per task and per variant: what you wrote, whether it is correct by the
document, and which sentence decided it. Give a verdict on each variant against
X (approve / object / veto), one falsifiable prediction with the milestone at
which it is checkable, and your condition.

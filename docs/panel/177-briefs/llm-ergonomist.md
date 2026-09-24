# Panel 177 — llm-ergonomist

You read **only** two files: `heroes-spec.md` in your directory, which is the
language's specification as it stands, and this brief. Nothing else — no other
file, no repository. Your verdict is an experiment: write what a first-time
reader of the spec writes, then say where you hesitated. Write your report to
`REPORT.md` in your directory.

The variants below are **label-stripped**: each is a replacement or an addition
to § 13 of the spec, and nothing tells you which one anybody prefers.

---

## Part A — three forms that are not in the spec yet

Read these as if they were added to § 13, after the sentence on `consumes`.
**Both variants α and β include all of this common text**:

> `acquires` may name several releasers, `acquires sqlite3_close |
> sqlite3_close_v2`, and any one of them ends the life. `transfers
> json_object_put` after a parameter says the call hands that value's life into
> another value, which will end it with `json_object_put`: the program owes
> nothing for it afterwards, and the handle must have been acquired with
> `json_object_put` among its releasers, or the call aborts before C runs.
> `retains json_object_put` after a result or a parameter says the call adds a
> reference to a handle: the program owes one more release for it, the
> reference must share a releaser with the life it joins, and a handle the
> program does not hold begins its life there.

**Variant α** adds:

> A transfer that happens only when the call succeeds names the result that
> means success after it: `val: Json transfers json_object_put on 0`. On any
> other result the value is still the program's, and it still owes its release.

**Variant β** adds:

> A call whose transfers happen only when it succeeds names the result that
> means success after its result type: `-> i32 when 0`. On any other result
> every value it was handed to transfer is still the program's, and it still
> owes its release.

### Task A1 — a child added to a parent, and the add can fail

The real header (json-c 0.19), verbatim except where ` * ...` stands for its
paragraph on a previous value under `key` and its three `@param` lines:

```c
/** Add an object field to a json_object of type json_type_object
 *
 * The reference count of `val` will *not* be incremented, in effect
 * transferring ownership that object to `obj`, and thus `val` will be
 * freed when `obj` is.  (i.e. through `json_object_put(obj)`)
 *
 * If you want to retain a reference to the added object, independent
 * of the lifetime of obj, you must increment the refcount with
 * `json_object_get(val)` (and later release it with json_object_put()).
 *
 * Since ownership transfers to `obj`, you must make sure
 * that you do in fact have ownership over `val`.  For instance,
 * json_object_new_object() will give you ownership until you transfer it,
 * whereas json_object_object_get() does not.
 * ...
 * @return On success, <code>0</code> is returned.
 * 	On error, a negative value is returned.
 */
int json_object_object_add(struct json_object *obj, const char *key,
                           struct json_object *val);
struct json_object *json_object_new_object(void);
struct json_object *json_object_new_int(int32_t i);
int json_object_put(struct json_object *obj);
```

Write the `extern` binding and a `main` that makes an object and an int, adds
the int under a key, and ends every life correctly **including when the add
fails**. Do it once under α and once under β. Then say: what did you conclude
the header means for `val` when the add fails, and from which words?

### Task A2 — a second reference, returned

```c
/** Increment the reference count of json_object, thereby taking ownership of it. */
struct json_object *json_object_get(struct json_object *obj);
```

Bind it, take a second reference to an object you made, and release both.

### Task A3 — a second reference, through a parameter

From OpenSSL 3's manual page: *"X509_up_ref() increments the reference count of
a."* and *"X509_up_ref() returns 1 for success and 0 for failure."*

```c
int X509_up_ref(X509 *a);
X509 *X509_new(void);
void X509_free(X509 *a);
```

Bind the three, take a second reference, release both.

### Task A4 — a closer the binding did not name

```c
FILE *popen(const char *command, const char *mode);
int pclose(FILE *stream);
int fclose(FILE *stream);
```

A program binds `popen(…) -> File acquires pclose` and ends the stream with
`fclose`. Under the common text, what happens, and when? A colleague says the
fix is to mark `fclose`'s parameter `transfers pclose`. What does the program do
then, and is your colleague right?

---

## Part B — a handle used after its life ended

The program, over a small JSON library whose `Delete` frees a node and its
children and whose `AddItemToObject` links `item` under `object`:

```
extern "cj.h"
    record Json tag cJSON
    function cJSON_CreateObject() -> Json acquires cJSON_Delete
    function cJSON_AddItemToObject(object: Json, string: cstr lent, item: Json consumes) -> i64
    function cJSON_Delete(item: Json consumes)

function main()
    b = cJSON_CreateObject()
    cJSON_Delete(item: b)
    c = cJSON_CreateObject()
    _ = cJSON_AddItemToObject(object: b, string: "c".cstr(), item: c)
    print("wrote into a deleted object")
```

Three label-stripped versions of the one sentence in § 13 that reads, today,
*"Giving one back twice aborts, unless C has since reused its address."*:

- **I** — the sentence as it stands.
- **II** — *"A handle handed to `consumes` may not be used again in the function
  that handed it; using it is an error."*
- **III** — *"Handing a handle to any call after the call that ended its life
  aborts before C runs, unless C has since reused its address."*

### Task B1

Under each of I, II and III: what do you expect `heroes check` and running the
program to do? Would you have written this program, and if you had, what would
tell you?

### Task B2

A loop that makes a node, uses it and deletes it on every turn, with the
binding declared before the loop. Write it. Under II, is it an error? Under
III, when does it abort, if ever?

---

## What to return

Your `verdict` per form (the common text, α, β, and I/II/III), an `experiment`
section with what you wrote first for each task, `hesitation_points`, an
`argument`, a falsifiable `prediction`, and the `condition` under which you
would change your verdict. **One measured limit**: a first-try rate needs many
fresh attempts per variant, and you are one reader; say what your single
attempt can and cannot show.

# Panel 155 — llm-ergonomist brief

You judge the objective: LLM comprehension and locality. **Your verdict is an
experiment, not an opinion.**

**Read only `spec/heroes-spec.md` and this file.** Do not read `design.md`, do
not read the repository, do not read the other briefs. Your value to this panel
is that you are the only seat whose input is what a reader of the language
actually gets.

## The language today, on the point at issue

Two sentences in `spec/heroes-spec.md` bear on this, and you should find them
yourself rather than take them from here. One is in § 13 about a `partial`
record. One is in § 3's table and § 10 about maps. § 9 states what generics are.

## The change under consideration, as a behaviour and not as a diff

Today, a function written with a type parameter — `function tally<K>(k: K)` —
may build a map keyed on `K` inside its body, and a caller may then call it with
a `f64`. That program compiles and runs. Written out directly, without the
generic, `m: {f64: i64}` is a compile error.

The same asymmetry exists for `==` on a `partial` record from an `extern` group:
written directly it is a compile error; passed through
`function same<A>(x: A, y: A) -> bool` whose body is `return x == y`, it
compiles, and the program stops at run time with
`panic: a partial record has no structural equality`.

The proposal makes the CALL a compile error in both cases: `tally(1.5)` and
`same(x: a, y: b)` would be refused, with the message pointing at the call.

## Three tasks. Do them before forming a verdict

Write the programs. You may not compile them — that is deliberate, and it is the
condition the other seats do not work under.

1. **The blind pair.** Here are two programs, A and B. For each, say whether it
   compiles under the specification as written, and if not, which sentence
   refuses it and where the error points.

   Program A:
   ```
   function tally<K>(k: K) -> i64
       m: {K: i64} @ {}
       m[k] @ 1
       return len(m)

   function main()
       print(tally(1.5))
   ```

   Program B:
   ```
   function main()
       m: {f64: i64} @ {}
       m[1.5] @ 1
       print(len(m))
   ```

   Then: if your two answers differ, is the difference derivable from the
   specification, or did you infer it? Quote the sentence you used.

2. **The repair.** Assume program A is refused at the line `print(tally(1.5))`.
   You are the model that wrote it and you have the diagnostic. Write the
   repaired program. Then say what you had to know that the message would have
   to tell you, and whether `tally`'s own text would have told you.

3. **The third task, and answer it as a reader rather than as a judge.** Write a
   function that counts how many times each value appears in a list, generic
   over the value's type, and then call it with a list of floats. Do it from the
   specification alone. Then say whether you expected that program to compile,
   and how long it took you to become sure.

## What your verdict is asked to answer

- Under the change, is a generic function's body still readable on its own? Or
  does a reader now need to know every call site to know whether a body is
  legal — which is the non-locality your seat has a veto over.
- Which of the two worlds produces fewer wrong programs that a model would
  write and believe: the one where the call is refused, or the one where it
  compiles and aborts at run time with a named message?
- Is there a third spelling of the rule the proposal has not considered which a
  reader would find more predictable?
- Does the specification, as written, already answer the question? If it does,
  say which sentence and whether the compiler agrees with it.

Your verdict owes a falsifiable prediction and the condition under which you
would change your vote. You hold a veto on non-local constructs; say plainly
whether you are casting it.

Write your report to `docs/panel/155-reports/llm-ergonomist.md`.

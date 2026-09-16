# Panel 158 — llm-ergonomist brief

**Read only `spec/heroes-spec.md` and this file.** Not design.md, not the
repository, not the other briefs. You are the only seat judging what a reader of
the language actually gets.

## The situation, without the repository

A map whose values may fail: `m: {str: i64?}`. You read a key out of it.

The specification says, in § 10, that `m[k]` gives a `V?` with code
`missing_key`. Here `V` is itself `i64?`.

## Four tasks, and write your answers out

1. **From the document alone**: what is the type of `m["a"]` above, and how do
   you get the `i64` out of it? Write the program. Say how long you were unsure.
2. **Write the type down.** Give `function take(…)` a parameter of that type.
   Does the specification let you? Quote the sentence you used, or say there is
   none.
3. A compiler tells you: `error[type_mismatch]: expected i64?, found i64??`.
   You have never seen `i64??` in the document. **What do you do next?** Be
   concrete about your first action.
4. § 11 gives `find(xs, f)` returning *the first it accepts, else `not_found`*.
   Apply it to a `[i64?]`. What comes back, and can you write that type down?

## What your verdict is asked to answer

- Is a type the compiler computes and the syntax cannot write acceptable, and
  does your answer change if it can never be produced accidentally?
- Four repairs are on the table: refuse the map declaration `{K: V?}`; flatten
  `m[k]` so a failed value and a missing key become one thing; make `T??`
  legal and writable; or leave it and add a sentence. **Rank them for a reader**
  and say what each costs you.
- If the answer is a sentence, propose it. Every word is paid for.
- You hold a veto on non-local constructs. Say plainly whether you cast it.

Your verdict owes a falsifiable prediction and your condition. Write to
`docs/panel/158-reports/llm-ergonomist.md`.
